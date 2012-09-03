/**
 * @file read_fulltype.cpp
 * @brief Source implementing a utility funciton to read in a type.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <Parser/bodies.h>
#include <API/context.h>
#include <API/compile_settings.h>
#include <API/AST.h>
#include <API/error_reporting.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <Parser/parse_context.h>
#include <System/builtins.h>
#include <cstdio>
using namespace jdip;
using namespace jdi;

full_type jdip::read_fulltype(lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr) {
  full_type ft = read_type(lex, token, scope, cp, herr);
  if (ft.def)
    jdip::read_referencers(ft.refs, ft, lex, token, scope, cp, herr);
  return ft;
}

full_type jdip::read_type(lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr)
{
  definition* inferred_type = NULL; // This is the type we will use if absolutely no other type is given
  definition* overridable_type = NULL;
  long int rflags = 0; // Return flags.
  long int swif = 0; // Swap-in flags: or'd in when a flag is determined not to be the main type.
  definition *rdef = NULL;
  ref_stack rrefs;
  
  if (token.type != TT_DECLARATOR) {
    if (token.type != TT_DECFLAG) {
      if (token.type == TT_CLASS or token.type == TT_STRUCT or token.type == TT_ENUM or token.type == TT_UNION) {
        if (cp)
          rdef = token.type == TT_ENUM? (definition*)cp->handle_enum(scope,token,0): 
            token.type == TT_UNION? (definition*)cp->handle_union(scope,token,0) : (definition*)cp->handle_class(scope,token,0);
        else {
          token = lex->get_token_in_scope(scope, herr);
          if (token.type != TT_DECLARATOR)
            token.report_error(herr, "Existing class name must follow class/struct token at this point");
        }
      }
      else if (token.type == TT_DEFINITION)
      {
        rdef = read_qualified_definition(lex, scope, token, cp, herr);
        if (!rdef) {
          token.report_errorf(herr, "Expected type name here before %s");
          return NULL;
        }
        if (!(rdef->flags & DEF_TYPENAME)) {
          if (rdef->flags & DEF_TEMPLATE) {
            if (((definition_template*)rdef)->def == scope)
              rdef = scope;
            else if (scope->flags & DEF_TEMPSCOPE and ((definition_template*)rdef)->def == scope->parent)
              rdef = scope->parent;
            else {
              token.report_error(herr, "Invalid use of template `" + rdef->name + "'");
              return NULL;
            }
          }
          else {
            token.report_error(herr, "Expected type name here; `" + rdef->name + "' does not name a type");
            return NULL;
          }
        }
      }
      else if (token.type == TT_ELLIPSIS) {
        rdef = builtin_type__va_list;
        token = lex->get_token_in_scope(scope, herr);
      }
      else if (token.type == TT_TYPENAME) {
        token = lex->get_token_in_scope(scope, herr);
        if (not(rdef = handle_hypothetical(lex, scope, token, DEF_TYPENAME, herr)))
          return full_type();
      }
      else {
        if (token.type == TT_IDENTIFIER or token.type == TT_DEFINITION)
          token.report_error(herr,"Type name expected here; `" + token.content.toString() + "' does not name a type");
        else
          token.report_errorf(herr,"Type name expected here before %s");
        return full_type();
      }
    }
    else {
      typeflag *const tf = ((typeflag*)token.def);
      if (tf->usage & UF_PRIMITIVE) {
        (tf->usage == UF_PRIMITIVE? rdef : overridable_type) = tf->def,
        swif = tf->flagbit;
      }
      else {
        if (tf->usage & UF_STANDALONE)
          inferred_type = tf->def;
        rflags = tf->flagbit;
      }
      token = lex->get_token_in_scope(scope,herr);
    }
  }
  else {
    rdef = token.def;
    token = lex->get_token_in_scope(scope,herr);
  }
  
  // Read any additional type info
  typeloop: while (token.type == TT_DECLARATOR or token.type == TT_DECFLAG)
  {
    if (token.type == TT_DECLARATOR) {
      if (rdef) {
        if (token.def->flags & (DEF_CLASS | DEF_ENUM | DEF_UNION | DEF_TYPED))
          break;
        token.report_error(herr,"Two types named in expression");
        FATAL_RETURN(full_type());
      }
      rdef = token.def;
      rflags |= swif;
    }
    else {
      typeflag *const tf = ((typeflag*)token.def);
      if (tf->usage & UF_PRIMITIVE) {
        if (tf->usage == UF_PRIMITIVE) {
          if (rdef)
            token.report_error(herr,"Two types named in expression");
          rdef = tf->def;
          rflags |= swif;
        } else {
          overridable_type = tf->def,
          rflags |= swif,
          swif = tf->flagbit;
        }
      }
      else if (tf->usage & UF_STANDALONE_FLAG)
        inferred_type = tf->def,
        rflags |= tf->flagbit;
    }
    token = lex->get_token_in_scope(scope, herr);
  }
  if (rdef == NULL) {
    if (token.type == TT_CLASS or token.type == TT_STRUCT or token.type == TT_UNION or token.type == TT_ENUM)
    {
      if (cp) // FIXME: I don't have any inherited flags to pass here. Is that OK?
        rdef = (token.type == TT_UNION? (definition*)cp->handle_union(scope,token,0) :
                token.type == TT_ENUM?  (definition*)cp->handle_enum(scope,token,0)  :  (definition*)cp->handle_class(scope,token,0));
      else {
        token = lex->get_token_in_scope(scope, herr);
        goto typeloop;
      }
    }
    else if (token.type == TT_DEFINITION and token.def->flags & (DEF_SCOPE | DEF_TEMPLATE)) {
     rdef = read_qualified_definition(lex, scope, token, cp, herr);
    }
    else if (token.type == TT_TYPENAME) {
      //if (!cp) { token.report_error(herr, "Cannot use dependent type in this context"); return full_type(); }
      token = lex->get_token_in_scope(scope);
      rdef = handle_hypothetical(lex, scope, token, DEF_TYPENAME, herr);
    }
    if (rdef == NULL)
    {
      rdef = overridable_type;
      if (rdef == NULL)
        rdef = inferred_type;
    }
  }
  
  return full_type(rdef, rrefs, rflags);
}

#include <General/debug_macros.h>
int jdip::read_referencers(ref_stack &refs, const full_type& ft, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr)
{
  #ifdef DEBUG_MODE
  static int number_of_times_GDB_dropped_its_ass = 0;
  number_of_times_GDB_dropped_its_ass++;
  #endif
  
  for (;;)
  {
    switch (token.type)
    {
      case TT_LEFTBRACKET:
        return read_referencers_post(refs, lex, token, scope, cp, herr);
      
      case TT_LEFTPARENTH: { // Either a function or a grouping
        token = lex->get_token_in_scope(scope, herr);
        
        // Check if we're a constructor.
        if (((ft.def == scope) // If the definition is this scope
            or ((scope->flags & DEF_TEMPSCOPE) and ft.def == scope->parent) // Or we're in a template and the definition is the parent of this scope
            or ((ft.def->flags & DEF_TEMPLATE) and ((definition_template*)ft.def)->def == scope) // Or our definition is the template whose scope we are in
          ) and (token.type != TT_OPERATOR or token.content.len != 1 or *token.content.str != '*'))
        {
          if (read_function_params(refs, lex, token, scope, cp, herr)) return 1;
          ref_stack appme; int res = read_referencers_post(appme, lex, token, scope, cp, herr);
          refs.append_c(appme); return res;
        }
        
        ref_stack nestedrefs;
        read_referencers(nestedrefs, ft, lex, token, scope, cp, herr); // It's safe to recycle ft because we already know we're not a constructor at this point.
        if (token.type != TT_RIGHTPARENTH) {
          token.report_errorf(herr, "Expected right parenthesis before %s to close nested referencers");
          FATAL_RETURN(1);
        }
        token = lex->get_token_in_scope(scope, herr);
        ref_stack appme; int res = read_referencers_post(appme, lex, token, scope, cp, herr);
        refs.append_c(appme); refs.append_nest_c(nestedrefs);
        return res;
      }
      
      case TT_DEFINITION:
      case TT_DECLARATOR: {
        definition *d = token.def;
        token = lex->get_token_in_scope(scope);
        if (token.type == TT_SCOPE) {
          if (!(d->flags & DEF_SCOPE)) {
            token.report_error(herr, "Cannot access `" + d->name + "' as scope");
            return 1;
          }
          token = lex->get_token_in_scope((definition_scope*)d, herr);
          if (token.type != TT_DEFINITION and token.type != TT_DECLARATOR) {
            token.report_errorf(herr, "Expected qualified-id before %s");
            return 1;
          }
          return 0;
        }
        else if (token.type == TT_LESSTHAN and d->flags & DEF_TEMPLATE) {
          definition_template* temp = (definition_template*)d;
          arg_key k(temp->params.size());
          if (read_template_parameters(k, temp, lex, token, scope, cp, herr))
            return 1;
          if (token.type != TT_GREATERTHAN) {
            token.report_error(herr, "Expected closing triangle brackets to template parameters before %s");
            return 1;
          }
          token.type = TT_DEFINITION;
          token.def = temp->instantiate(k);
          return 0;
        }
        refs.name = d->name;
        ref_stack appme; int res = read_referencers_post(appme, lex, token, scope, cp, herr);
        refs.append_c(appme); return res;
      }
      case TT_IDENTIFIER: {// The name associated with this type
        refs.name = token.content.toString();
        token = lex->get_token_in_scope(scope);
        ref_stack appme; int res = read_referencers_post(appme, lex, token, scope, cp, herr);
        refs.append_c(appme); return res;
      }
      
      case TT_OPERATORKW: {
          refs.name = read_operatorkw_name(lex, token, scope, herr);
          ref_stack appme; int res = read_referencers_post(appme, lex, token, scope, cp, herr);
          refs.append_c(appme); return res;
      } break;
      
      
      case TT_OPERATOR: // Could be an asterisk or ampersand
        if ((token.content.str[0] == '&' or token.content.str[0] == '*') and token.content.len == 1) {
          refs.push(token.content.str[0] == '&'? ref_stack::RT_REFERENCE : ref_stack::RT_POINTERTO);
          break;
        } goto default_; // Else overflow
      
      case TT_DECFLAG: {
          typeflag* a = ((typeflag*)token.def);
          if (a->flagbit == builtin_flag__const || a->flagbit == builtin_flag__volatile || a->flagbit == builtin_flag__restrict) {
            // TODO: Give RT_POINTERTO node a bool/volatile flag; to denote that the pointer is const or volatile; set it here.
            token = lex->get_token_in_scope(scope, herr);
            continue;
          }
        } goto default_;
      
      case TT_ELLIPSIS:
          token.report_error(herr, "`...' not allowed as general modifier");
        goto default_;
      
      case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_EXTERN: case TT_UNION: 
      case TT_NAMESPACE: case TT_TEMPLATE: case TT_TYPENAME: case TT_TYPEDEF: case TT_USING: case TT_PUBLIC:
      case TT_PRIVATE: case TT_PROTECTED: case TT_COLON: case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_SCOPE:
      case TT_LEFTBRACE: case TT_RIGHTBRACE: case TT_LESSTHAN: case TT_GREATERTHAN: case TT_TILDE: case TT_ASM: case TT_SIZEOF: case TT_ISEMPTY: case TT_DECLTYPE:
      case TT_COMMA: case TT_SEMICOLON: case TT_STRINGLITERAL: case TT_CHARLITERAL: case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
      case TT_NEW: case TT_DELETE: case TT_ENDOFCODE: case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID: default: default_:
      #include <User/token_cases.h>
        return 0;
    }
    token = lex->get_token_in_scope(scope, herr);
  }
}
  
int jdip::read_referencers_post(ref_stack &refs, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr)
{
  #ifdef DEBUG_MODE
  static int number_of_times_GDB_dropped_its_ass = 0;
  number_of_times_GDB_dropped_its_ass++;
  #endif
  
  for (;;)
  {
    switch (token.type)
    {
      case TT_LEFTBRACKET: { // Array bound indicator
        AST ast;
        token = lex->get_token_in_scope(scope, herr);
        if (token.type != TT_RIGHTBRACKET) {
          if (ast.parse_expression(token,lex,precedence::comma+1,herr))
            return 1; // This error has already been reported, just return empty.
          if (token.type != TT_RIGHTBRACKET) {
            token.report_errorf(herr,"Expected closing square bracket here before %s");
            return 1;
          }
          render_ast(ast, "ArrayBounds");
          value as = ast.eval();
          size_t boundsize = (as.type == VT_INTEGER)? as.val.i : ref_stack::node_array::nbound;
          refs.push_array(boundsize);
        }
        else
          refs.push_array(ref_stack::node_array::nbound);
      } break;
      
      case TT_LEFTPARENTH: // Function parameters
        token = lex->get_token_in_scope(scope, herr);
        read_function_params(refs, lex, token, scope, cp, herr);
      continue;
      
      case TT_OPERATOR: // Could be an asterisk or ampersand
        if ((token.content.str[0] == '&' or token.content.str[0] == '*') and token.content.len == 1) {
          refs.push(token.content.str[0] == '&'? ref_stack::RT_REFERENCE : ref_stack::RT_POINTERTO);
          break;
        } goto default_; // Else overflow
      
      case TT_DECFLAG: {
          typeflag* a = ((typeflag*)token.def);
          if (a->flagbit == builtin_flag__const || a->flagbit == builtin_flag__volatile || a->flagbit == builtin_flag__restrict) {
            // TODO: Give RT_POINTERTO node a bool/volatile flag; to denote that the pointer is const or volatile; set it here.
            token = lex->get_token_in_scope(scope, herr);
            continue;
          }
        } goto default_;
      
      case TT_ELLIPSIS:
          token.report_error(herr, "`...' not allowed as general modifier");
        goto default_;
      
      case TT_LESSTHAN:
        goto default_;
      
      case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_EXTERN: case TT_UNION: case TT_DECLARATOR: case TT_IDENTIFIER:
      case TT_NAMESPACE: case TT_TEMPLATE: case TT_TYPENAME: case TT_TYPEDEF: case TT_USING: case TT_PUBLIC: case TT_DEFINITION: 
      case TT_PRIVATE: case TT_PROTECTED: case TT_COLON: case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_SCOPE: case TT_OPERATORKW:
      case TT_LEFTBRACE: case TT_RIGHTBRACE: case TT_GREATERTHAN: case TT_TILDE: case TT_ASM: case TT_SIZEOF: case TT_ISEMPTY: case TT_DECLTYPE:
      case TT_COMMA: case TT_SEMICOLON: case TT_STRINGLITERAL: case TT_CHARLITERAL: case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
      case TT_NEW: case TT_DELETE: case TT_ENDOFCODE: case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID: default: default_:
      #include <User/token_cases.h>
        return 0;
    }
    token = lex->get_token_in_scope(scope, herr);
  }
}

int jdip::read_function_params(ref_stack &refs, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr)
{
  ref_stack::parameter_ct params;
  
  // Navigate to the end of the function parametr list
  while (token.type != TT_RIGHTPARENTH)
  {
    if (token.type == TT_ENDOFCODE)
      return 1;
    full_type a = read_fulltype(lex,token,scope,cp,herr);
    if (!a.def) {
      token.report_errorf(herr, "Expected type-id for function parameters before %s");
      FATAL_RETURN(1);
    }
    ref_stack::parameter param; // Instantiate a parameter
    param.swap_in(a); // Give it our read-in full type (including ref stack, which is costly to copy)
    param.variadic = cp? cp->variadics.find(param.def) != cp->variadics.end() : false;
    if (token.type == TT_OPERATOR) {
      if (token.content.len != 1 or *token.content.str != '=') {
        token.report_errorf(herr, "Unexpected operator at this point; expected '=' or ')' before %s");
        FATAL_RETURN(1);
      }
      else {
        param.default_value = new AST;
        token = lex->get_token_in_scope(scope, herr);
        param.default_value->parse_expression(token, lex, scope, precedence::comma+1, herr);
      }
    }
    params.throw_on(param);
    
    if (token.type != TT_COMMA) {
      if (token.type == TT_RIGHTPARENTH) break;
      token.report_errorf(herr,"Expected comma or closing parenthesis to function parameters before %s");
      FATAL_RETURN(1);
    }
    token = lex->get_token_in_scope(scope, herr);
  }
  
  // Push our function information onto the reference stack
  refs.push_func(params);
  if (token.type != TT_RIGHTPARENTH) {
    token.report_error(herr,"Expected closing parenthesis to function parameters");
    FATAL_RETURN(1);
  }
  
  // If there's no other special garbage being tacked onto this, then we are not a pointer-to function,
  // and we are not an array of functions, and we aren't a function returning a function.
  // Ergo, the function can be implemented here. FIXME: Make sure parser allows implementing function returning function pointer.
  token = lex->get_token_in_scope(scope, herr); // Read in our next token to see if it's a brace or extra info
  while (token.type == TT_DECFLAG) { // It is legal to put the flags throw and const here.
    typeflag *tf = (typeflag*)token.def;
    token = lex->get_token_in_scope(scope, herr);
    if (tf == builtin_typeflag__throw) {
      if (token.type == TT_LEFTPARENTH) {
        token = lex->get_token_in_scope(scope, herr);
        if (token.type != TT_RIGHTPARENTH) {
          if (!read_fulltype(lex, token, scope, cp, herr).def) {
            token.report_error(herr, "Expected type to throw() statement");
            FATAL_RETURN(1);
          }
          else if (token.type != TT_RIGHTPARENTH) {
            token.report_errorf(herr, "Expected closing parentheses to throw() clause before %s");
            FATAL_RETURN(1);
          }
          else
            token = lex->get_token_in_scope(scope, herr);
        }
        else
          token = lex->get_token_in_scope(scope, herr);
      }
    }
  }
  return 0;
}
