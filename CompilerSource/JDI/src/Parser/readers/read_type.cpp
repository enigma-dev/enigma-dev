/**
 * @file read_fulltype.cpp
 * @brief Source implementing a utility funciton to read in a type.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2014 Josh Ventura
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

#include <Parser/context_parser.h>
#include <API/context.h>
#include <API/compile_settings.h>
#include <API/AST.h>
#include <API/error_reporting.h>
#include <General/parse_basics.h>
#include <General/debug_macros.h>
#include <Parser/is_potential_constructor.h>
#include <System/builtins.h>
#include <cstdio>
using namespace jdip;
using namespace jdi;

full_type jdip::context_parser::read_fulltype(token_t &token, definition_scope *scope) {
  full_type ft = read_type(token, scope);
  read_referencers(ft.refs, ft, token, scope);
  return ft;
}

full_type jdip::context_parser::read_type(token_t &token, definition_scope *scope)
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
        rdef = token.type == TT_ENUM?  (definition*)handle_enum(scope,token,0): 
               token.type == TT_UNION? (definition*)handle_union(scope,token,0) : (definition*)handle_class(scope,token,0);
      }
      else if (token.type == TT_DEFINITION)
      {
        rdef = read_qualified_definition(token, scope);
        if (!rdef) {
          token.report_errorf(herr, "Expected type name here before %s");
          return NULL;
        }
        if (!(rdef->flags & DEF_TYPENAME)) {
          if (rdef->flags & DEF_HYPOTHETICAL)
            ((definition_hypothetical*)rdef)->required_flags |= DEF_TYPENAME;
          else if (rdef->flags & DEF_TEMPPARAM)
            ((definition_tempparam*)rdef)->must_be_class = true;
          else if ((rdef->name == constructor_name || rdef->name[0] == '~') && rdef->flags & DEF_FUNCTION) {
            // TODO: This block's a little tacky. It replaces ctor/dtor types with void and expects caller to perform this check again; DEF_CTOR/DTOR would speed things up
            full_type res(builtin_type__void);
            res.refs.ndef = rdef;
            return res;
          }
          else {
            token.report_error(herr, "Expected type name here; `" + rdef->name + "' does not name a type (" + flagnames(rdef->flags) + ")");
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
        if (not(rdef = handle_hypothetical(scope, token, DEF_TYPENAME)))
          return full_type();
      }
      else {
        if (token.type == TT_IDENTIFIER)
          token.report_error(herr,"Type name expected here; `" + token.content.toString() + "' is not declared");
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
  else
    rdef = read_qualified_definition(token, scope);
  
  // Read any additional type info
  while (token.type == TT_DECLARATOR or token.type == TT_DECFLAG)
  {
    if (token.type == TT_DECLARATOR) {
      if (rdef) {
        if (token.def->flags & (DEF_CLASS | DEF_ENUM | DEF_UNION | DEF_TYPED))
          break;
        token.report_error(herr,"Two types named in expression");
        FATAL_RETURN(full_type());
      }
      rdef = read_qualified_definition(token, scope);
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
      token = lex->get_token_in_scope(scope, herr);
    }
  }
  if (rdef == NULL) {
    if (token.type == TT_CLASS or token.type == TT_STRUCT or token.type == TT_UNION or token.type == TT_ENUM)
    {
      rdef = (token.type == TT_UNION? (definition*)handle_union(scope,token,0) :
              token.type == TT_ENUM?  (definition*)handle_enum(scope,token,0)  :  (definition*)handle_class(scope,token,0));
    }
    else if (token.type == TT_DEFINITION and token.def->flags & (DEF_SCOPE | DEF_TEMPLATE)) {
      rdef = read_qualified_definition(token, scope);
      if (rdef and !(rdef->flags & DEF_TYPENAME))
      {
        full_type res(overridable_type? overridable_type : inferred_type, rrefs, rflags);
        res.refs.ndef = rdef;
        return res;
      }
    }
    else if (token.type == TT_TYPENAME) {
      //if (!cp) { token.report_error(herr, "Cannot use dependent type in this context"); return full_type(); }
      token = lex->get_token_in_scope(scope);
      rdef = handle_hypothetical(scope, token, DEF_TYPENAME);
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

#include <System/lex_buffer.h>
#include <General/debug_macros.h>

enum parenth_type { PT_FUNCTION, PT_GROUPORINIT, PT_INITIALIZERS, PT_NA };
static parenth_type parenths_type(lexer *lex, definition_scope *scope, lex_buffer &lb, token_t &token, context_parser *cp, error_handler *herr)
{
  if (token.type == TT_LEFTPARENTH)
  {
    bool seen_type = false, seen_comma = false;
    token_t *backt = &lb.push(token = lex->get_token_in_scope(scope, herr));
    
    bool read_next = false;
    for (; token.type != TT_RIGHTPARENTH; backt = &lb.push(read_next? token : (token = lex->get_token_in_scope(scope, herr))))
    {
      read_next = false;
      if (token.type == TT_LEFTPARENTH) {
        if (!seen_type) {
          parenth_type res = parenths_type(lex, scope, lb, token, cp, herr);
          return res == PT_INITIALIZERS || seen_comma? PT_INITIALIZERS : PT_GROUPORINIT;
        }
        break;
      }
      else if (token.type == TT_DECLARATOR or token.type == TT_DECFLAG) {
        seen_type = true;
      }
      else if (token.type == TT_DEFINITION) {
        definition* bd = cp->read_qualified_definition(token, scope);
        if (bd && ((backt->def = bd)->flags & DEF_TYPENAME))
          seen_type = true;
        else if (bd && (bd->flags & DEF_TEMPLATE) && bd->name == scope->name)
          seen_type = true;
        else if (!seen_type)
          return seen_comma? PT_INITIALIZERS : PT_GROUPORINIT;
        read_next = true;
      }
      else if (token.type == TT_IDENTIFIER) {
        if (seen_type)
          return PT_FUNCTION;
        return PT_GROUPORINIT;
      }
      else if (token.type == TT_OPERATOR) {
        if (token.content.len > 1) return PT_INITIALIZERS;
        if (*token.content.str == '=') return PT_FUNCTION;
        if (*token.content.str == '*' or *token.content.str == '&')
          return seen_type? PT_FUNCTION : PT_GROUPORINIT;
        return PT_INITIALIZERS;
      }
      else if (token.type == TT_COMMA)
        seen_comma = true;
      else if (token.type == TT_SCOPE) {
        if (seen_type)
          return PT_GROUPORINIT;
      }
      else
        token.report_errorf(herr, "Unexpected %s in referencers");
    }
    return PT_FUNCTION;
  }
  else {
    while (token.type != TT_ENDOFCODE) {
    lb.push(token = lex->get_token_in_scope(scope, herr));
    if (token.type == TT_LEFTPARENTH) {
      parenths_type(lex, scope, lb, token, cp, herr);
      if (token.type != TT_RIGHTPARENTH) return token.report_errorf(herr, "Expected closing parenthesis before %s"), PT_NA;
    }
    else if (token.type == TT_LEFTBRACKET) {
      parenths_type(lex, scope, lb, token, cp, herr);
      if (token.type != TT_RIGHTBRACKET) return token.report_errorf(herr, "Expected closing bracket before %s"), PT_NA;
    }
    else if (token.type == TT_LEFTBRACE) {
      parenths_type(lex, scope, lb, token, cp, herr);
      if (token.type != TT_RIGHTBRACKET) return token.report_errorf(herr, "Expected closing brace before %s"), PT_NA;
    }
    else if (token.type == TT_RIGHTPARENTH or token.type == TT_RIGHTBRACKET or token.type == TT_RIGHTBRACE)
      return PT_NA;
    }
    return PT_NA;
  }
}

class fix_scope {
  definition *d;
  definition_scope *s, *spp;
  bool chp;
public:
  definition_scope *scope;
  fix_scope(definition *dd, definition_scope *sd): d(dd), s(sd), spp(), chp(false) {
    if (d and d->parent != s)
    {
      if (s->flags & DEF_TEMPLATE) {
        spp = s->parent;
        s->parent = d->parent, chp = true;
        scope = s;
      }
      else
        scope = d->parent;
    }
    else
      scope = s;
  }
  ~fix_scope() { if (chp) s->parent = spp; }
};

int jdip::context_parser::read_referencers(ref_stack &refs, const full_type& ft, token_t &token, definition_scope *scope)
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
        return read_referencers_post(refs, token, scope);
      
      case TT_LEFTPARENTH: { // Either a function or a grouping, or, potentially, a constructor call.
        fix_scope fs(ft.refs.ndef, scope);
        
        lex_buffer lb(lex);
        bool is_func = parenths_type(lex, fs.scope, lb, token, this, herr) == PT_FUNCTION;
        
        lb.reset();
        lex = &lb;
        token = lex->get_token(herr);
        
        if (is_func)
        {
          ref_stack appme;
          bool error = read_function_params(appme, token, fs.scope);
          lex = lb.fallback_lexer;
          if (error)
            return 1;
          int res = read_referencers_post(appme, token, fs.scope);
          refs.append_c(appme);
          return res;
        }
        
        ref_stack nestedrefs;
        read_referencers(nestedrefs, ft, token, fs.scope); // It's safe to recycle ft because we already know we're not a constructor at this point.
        
        lex = lb.fallback_lexer;
        
        if (token.type != TT_RIGHTPARENTH) {
          token.report_errorf(herr, "Expected right parenthesis before %s to close nested referencers");
          FATAL_RETURN(1);
        }
        token = lex->get_token_in_scope(scope, herr);
        ref_stack appme; int res = read_referencers_post(appme, token, scope);
        refs.append_c(appme); refs.append_nest_c(nestedrefs);
        return res;
      }
      
      case TT_DEFINITION:
      case TT_DECLARATOR: {
        definition *pd = token.def;
        definition *d = read_qualified_definition(token, scope);
        if (!d) return 1;
        
        if (token.type == TT_MEMBEROF) {
          if (!(d->flags & DEF_CLASS) && !(d->flags & DEF_TYPENAME)) {
            token.report_error(herr, "Member pointer to non-class `" + d->name + "'");
            return 1;
          }
          refs.push_memptr((definition_class*)d);
          token = lex->get_token(herr);
          if (token.type == TT_IDENTIFIER) {
            refs.name = token.content.toString();
            token = lex->get_token_in_scope(scope, herr);
          }
        }
        else {
          refs.name = d->name;
          if (pd != d)
            refs.ndef = d;
        }
        
        int res;
        ref_stack appme;
        if (pd != d)
        {
          if (scope->flags & DEF_TEMPLATE) {
            definition_scope *sp = scope->parent; scope->parent = d->parent;
            res = read_referencers_post(appme, token, scope);
            scope->parent = sp;
          }
          else
            res = read_referencers_post(appme, token, pd->parent);
        }
        else
          res = read_referencers_post(appme, token, scope);
        refs.append_c(appme); return res;
      }
      case TT_IDENTIFIER: {// The name associated with this type
        refs.name = token.content.toString();
        token = lex->get_token_in_scope(scope);
        ref_stack appme; int res = read_referencers_post(appme, token, scope);
        refs.append_c(appme); return res;
      }
      
      case TT_MEMBEROF:
          token.report_error(herr, "Member pointer (class::*) not presently supported...");
        return 1;
         
      case TT_OPERATORKW: {
          refs.name = read_operatorkw_name(token, scope);
          refs.ndef = scope->look_up(refs.name);
          ref_stack appme; int res = read_referencers_post(appme, token, scope);
          refs.append_c(appme); return res;
      } break;
      
      case TT_ALIGNAS:
      case TT_NOEXCEPT:
        return read_referencers_post(refs, token, scope);
      
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
      case TT_NAMESPACE: case TT_TEMPLATE: case TT_TYPENAME: case TT_TYPEDEF: case TT_USING: case TT_PUBLIC: case TT_FRIEND:
      case TT_PRIVATE: case TT_PROTECTED: case TT_COLON: case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_SCOPE:
      case TT_LEFTBRACE: case TT_RIGHTBRACE: case TT_LESSTHAN: case TT_GREATERTHAN: case TT_TILDE: case TT_ASM: case TT_SIZEOF: case TT_ISEMPTY: case TT_DECLTYPE: case TT_TYPEID:
      case TT_COMMA: case TT_SEMICOLON: case TT_STRINGLITERAL: case TT_CHARLITERAL: case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
      case TT_NEW: case TT_DELETE: case TT_CONST_CAST: case TT_STATIC_CAST: case TT_DYNAMIC_CAST: case TT_REINTERPRET_CAST:
      case TT_ALIGNOF: case TT_CONSTEXPR: case TT_AUTO: case TT_STATIC_ASSERT:
      case TT_ENDOFCODE: case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID: default: default_:
      #include <User/token_cases.h>
        return 0;
    }
    token = lex->get_token_in_scope(scope, herr);
  }
}
  
int jdip::context_parser::read_referencers_post(ref_stack &refs, token_t &token, definition_scope *scope)
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
          if (astbuilder->parse_expression(&ast, token,scope,precedence::comma+1))
            return 1; // This error has already been reported, just return empty.
          if (token.type != TT_RIGHTBRACKET) {
            token.report_errorf(herr,"Expected closing square bracket here before %s");
            return 1;
          }
          render_ast(ast, "ArrayBounds");
          value as = ast.eval(error_context(herr, token));
          size_t boundsize = (as.type == VT_INTEGER)? as.val.i : ref_stack::node_array::nbound;
          refs.push_array(boundsize);
        }
        else
          refs.push_array(ref_stack::node_array::nbound);
      } break;
      
      case TT_LEFTPARENTH: // Function parameters
        token = lex->get_token_in_scope(scope, herr);
        read_function_params(refs, token, scope);
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
      
      case TT_ALIGNAS:
          token.report_error(herr, "Unimplemented: `alignas'");
        return 1;
      
      case TT_NOEXCEPT:
          token.report_error(herr, "Unimplemented: `noexcept'");
        return 1;
      
      case TT_ELLIPSIS:
          token.report_error(herr, "`...' not allowed as general modifier");
        goto default_;
      
      case TT_LESSTHAN:
        goto default_;
      
      case TT_MEMBEROF:
        token.report_error(herr, "Member access (class::*) expected before name in type");
        return 1;
      
      case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_EXTERN: case TT_UNION: case TT_DECLARATOR: case TT_IDENTIFIER:
      case TT_NAMESPACE: case TT_TEMPLATE: case TT_TYPENAME: case TT_TYPEDEF: case TT_USING: case TT_PUBLIC: case TT_FRIEND: case TT_DEFINITION: 
      case TT_PRIVATE: case TT_PROTECTED: case TT_COLON: case TT_RIGHTPARENTH: case TT_RIGHTBRACKET: case TT_SCOPE: case TT_OPERATORKW:
      case TT_LEFTBRACE: case TT_RIGHTBRACE: case TT_GREATERTHAN: case TT_TILDE: case TT_ASM: case TT_SIZEOF: case TT_ISEMPTY: case TT_ALIGNOF:
      case TT_DECLTYPE: case TT_TYPEID: case TT_CONST_CAST: case TT_STATIC_CAST: case TT_DYNAMIC_CAST: case TT_REINTERPRET_CAST:
      case TT_COMMA: case TT_SEMICOLON: case TT_STRINGLITERAL: case TT_CHARLITERAL: case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL:
      case TT_NEW: case TT_DELETE: case TT_STATIC_ASSERT: case TT_CONSTEXPR: case TT_AUTO:
      case TT_ENDOFCODE: case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID: default: default_:
      #include <User/token_cases.h>
        return 0;
    }
    token = lex->get_token_in_scope(scope, herr);
  }
}

int jdip::context_parser::read_function_params(ref_stack &refs, token_t &token, definition_scope *scope)
{
  ref_stack::parameter_ct params;
  
  // Navigate to the end of the function parametr list
  while (token.type != TT_RIGHTPARENTH)
  {
    if (token.type == TT_ENDOFCODE)
      return 1;
    full_type a = read_fulltype(token, scope);
    if (!a.def) {
      token.report_errorf(herr, "Expected type-id for function parameters before %s");
      FATAL_RETURN(1);
    }
    ref_stack::parameter param; // Instantiate a parameter
    param.swap_in(a); // Give it our read-in full type (including ref stack, which is costly to copy)
    param.variadic = ctex->variadics.find(param.def) != ctex->variadics.end();
    if (token.type == TT_OPERATOR) {
      if (token.content.len != 1 or *token.content.str != '=') {
        token.report_errorf(herr, "Unexpected operator at this point; expected '=' or ')' before %s");
        FATAL_RETURN(1);
      }
      else {
        param.default_value = new AST();
        token = lex->get_token_in_scope(scope, herr);
        astbuilder->parse_expression(param.default_value, token, scope, precedence::comma+1);
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
          if (!read_fulltype(token, scope).def) {
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
