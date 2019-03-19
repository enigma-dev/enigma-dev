/**
 * @file  handle_scope.cpp
 * @brief Source implementing a massive delegator which populates a scope.
 * 
 * This file does a huge amount of work.
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
#include <API/AST.h>
#include <API/compile_settings.h>
#include <System/builtins.h>
#include <System/lex_buffer.h>
#include <Parser/handlers/handle_function_impl.h>
#include <cstdio>

using jdi::definition_scope;

int jdip::context_parser::handle_scope(definition_scope *scope, token_t& token, unsigned inherited_flags)
{
  definition* decl;
  token = read_next_token(scope);
  for (;;)
  {
    switch (token.type)
    {
      case TT_TYPENAME:
      case TT_DECFLAG: case TT_DECLTYPE: case TT_DECLARATOR: case_TT_DECLARATOR:
      case TT_CLASS: case TT_STRUCT: case TT_ENUM: case TT_UNION: case TT_TILDE:
          decl = NULL;
          handle_declarator_block:
          if (handle_declarators(scope, token, inherited_flags, decl)) {
            FATAL_RETURN(1);
            while (token.type != TT_ENDOFCODE and token.type != TT_SEMICOLON and token.type != TT_LEFTBRACE and token.type != TT_RIGHTBRACE)
              token = read_next_token(scope);
          }
          handled_declarator_block:
          if (token.type != TT_SEMICOLON) {
            if (token.type == TT_LEFTBRACE || token.type == TT_ASM) {
              if (!(decl and decl->flags & DEF_FUNCTION)) {
                token.report_error(herr, "Unexpected opening brace here; declaration is not a function");
                FATAL_RETURN(1);
                handle_function_implementation(lex,token,scope,herr);
              }
              else
                ((definition_function*)decl)->implementation = handle_function_implementation(lex,token,scope,herr);
              if (token.type != TT_RIGHTBRACE && token.type != TT_SEMICOLON) {
                token.report_error(herr, "Expected closing symbol to function");
                continue;
              }
            }
            else {
              token.report_errorf(herr, "Expected semicolon before %s following declaration");
              #if FATAL_ERRORS
                return 1;
              #else
                semicolon_bail:
                while (token.type != TT_SEMICOLON && token.type != TT_LEFTBRACE && token.type != TT_RIGHTBRACE && token.type != TT_ENDOFCODE)
                  token = read_next_token(scope);
                if (token.type == TT_LEFTBRACE) {
                  size_t depth = 1;
                  while (token.type != TT_ENDOFCODE) {
                    token = read_next_token(scope);
                    if (token.type == TT_LEFTBRACE) ++depth;
                    else if (token.type == TT_RIGHTBRACE) if (!--depth) break;
                  }
                }
              #endif
            }
          }
        break;
      
      case TT_EXTERN:
          token = read_next_token(scope);
          if (token.type == TT_STRINGLITERAL) {
            token = read_next_token(scope);
            if (token.type == TT_LEFTBRACE) {
              FATAL_RETURN_IF(handle_scope(scope, token, inherited_flags), 1);
              if (token.type != TT_RIGHTBRACE) {
                token.report_error(herr, "Expected closing brace to extern block");
                FATAL_RETURN(1);
              }
              break;
            }
          }
          else if (token.type == TT_TEMPLATE) {
            token = read_next_token(scope);
            if (token.type != TT_CLASS and token.type != TT_STRUCT and token.type != TT_DECLARATOR and token.type != TT_DEFINITION) {
              token.report_errorf(herr, "Expected template specialization following `extern template' directive; %s unhandled");
              FATAL_RETURN(1);
            }
          }
        goto handle_declarator_block;
      
      case TT_COMMA:
          token.report_error(herr, "Unexpected comma at this point.");
        return 1;
      
      case TT_SEMICOLON:
          /* Printing a warning here is advisable but unnecessary. */
        break;
      
      case TT_NAMESPACE: if (handle_namespace(scope,token)) return 1; break;
      case TT_LEFTPARENTH: {
          token.report_error(herr, "Stray opening parenthesis.");
          #if FATAL_ERRORS
            return 1;
          #else
            int bc = 1;
            while (bc) {
              token = read_next_token(scope);
              bc += token.type == TT_LEFTPARENTH;
              bc -= token.type == TT_RIGHTPARENTH;
            }
          #endif
        } break;
      case TT_RIGHTPARENTH: token.report_error(herr, "Stray closing parenthesis."); return 1;
      case TT_LEFTBRACKET:  token.report_error(herr, "Stray opening bracket."); return 1;
      case TT_RIGHTBRACKET: token.report_error(herr, "Stray closing bracket."); return 1;
      case TT_RIGHTBRACE:   return 0;
      case TT_LEFTBRACE: {
          token.report_error(herr, "Expected scope declaration before opening brace.");
          #if FATAL_ERRORS
            return 1;
          #else
            int bc = 1;
            while (bc) {
              token = read_next_token(scope);
              if (token.type == TT_ENDOFCODE) {
                token.report_error(herr, "Expected closing brace before end of code.");
                return 1;
              }
              bc += token.type == TT_LEFTBRACE;
              bc -= token.type == TT_RIGHTBRACE;
            }
          #endif
        } break;
      
      case TT_TYPEDEF:
        token = read_next_token(scope);
        if (handle_declarators(scope,token,inherited_flags | DEF_TYPENAME)) FATAL_RETURN(1); break;
      
      case TT_PUBLIC:
        if (scope->flags & DEF_CLASS) { inherited_flags &= ~(DEF_PRIVATE | DEF_PROTECTED); }
        else token.report_error(herr, "Unexpected `public' token outside class scope.");
        if ((token = read_next_token(scope)).type != TT_COLON)
          token.report_error(herr, "Colon expected following `public' token"); break;
      case TT_PRIVATE:
        if (scope->flags & DEF_CLASS) { inherited_flags &= ~(DEF_PRIVATE | DEF_PROTECTED); inherited_flags |= DEF_PRIVATE; }
        else token.report_error(herr, "Unexpected `private' token outside class scope.");
        if ((token = read_next_token(scope)).type != TT_COLON)
          token.report_error(herr, "Colon expected following `private' token"); break;
      case TT_PROTECTED:
        if (scope->flags & DEF_CLASS) { inherited_flags &= ~(DEF_PRIVATE | DEF_PROTECTED); inherited_flags |= DEF_PROTECTED; }
        else token.report_error(herr, "Unexpected `protected' token outside class scope.");
        if ((token = read_next_token(scope)).type != TT_COLON)
          token.report_error(herr, "Colon expected following `protected' token"); break;
      
      case TT_USING:
          token = read_next_token(scope);
          if (token.type == TT_NAMESPACE) {
            token = lex->get_token(herr);
            if (token.type == TT_IDENTIFIER) {
              definition* d = scope->look_up(token.content.toString());
              if (!d) {
                token.report_errorf(herr, "Expected id to use before %s");
                FATAL_RETURN(1);
              }
              else {
                if (d->flags & DEF_NAMESPACE)
                  scope->use_namespace((definition_scope*)d);
                else
                  token.report_error(herr, "Expected namespace name following `namespace' token");
              }
              token = read_next_token(scope);
            }
            else
              token.report_error(herr, "Expected namespace name following `namespace' token");
          }
          else {
            definition *usedef = read_qualified_definition(lex, scope, token, this, herr);
            if (usedef)
              scope->use_general(usedef->name, usedef);
            else {
              token.report_errorf(herr, "Using directive does not specify an object");
              FATAL_RETURN(1);
            }
            if (token.type != TT_SEMICOLON) {
              token.report_errorf(herr, "Expected semicolon before %s to terminate using directive");
              FATAL_RETURN(1);
            }
          }
        break;
      
      case TT_SCOPE:
          token = read_next_token(global);
        continue;
      case TT_DEFINITION: {
        if (token.def->flags & DEF_NAMESPACE) {
          definition_scope* dscope = (definition_scope*)token.def;
          token = read_next_token(scope);
          if (token.type == TT_SCOPE) {
            token = read_next_token(dscope);
            continue;
          }
          token.report_errorf(herr, "Expected `::' here to access namespace members");
          FATAL_RETURN(1); break;
        }
        if (token.def->flags & DEF_TEMPLATE)
          goto case_TT_DECLARATOR;
      }
      case TT_IDENTIFIER: {
          string tname(token.content.toString());
          if (tname == scope->name and (scope->flags & DEF_CLASS)) {
            token = read_next_token(scope);
            if (token.type != TT_LEFTPARENTH) {
              token.report_errorf(herr, "Expected constructor parmeters before %s");
              break;
            }
            
            full_type ft;
            ft.def = scope;
            token = read_next_token(scope);
            read_function_params(ft.refs, lex, token, scope, this, herr);
            if (handle_declarators(scope,token,ft,inherited_flags | DEF_TYPENAME,decl))
              FATAL_RETURN(1);
            goto handled_declarator_block;
          }
          token.report_error(herr, "Unexpected identifier in this scope; `" + tname + "' does not name a type");
        } break;
      
      case TT_TEMPLATE:
        if (handle_template(scope, token, inherited_flags)) {
          FATAL_RETURN(1);
          goto semicolon_bail;
        }
        break;
      
      case TT_OPERATORKW:
          token = read_next_token(scope);
          if (token.type != TT_DECLARATOR and token.type != TT_DECFLAG and token.type != TT_DECLTYPE) {
            token.report_errorf(herr, "Expected cast type to overload before %s");
            FATAL_RETURN(1);
          }
          else {
            lex_buffer lb(lex);
            while (token.type != TT_LEFTPARENTH and token.type != TT_LEFTBRACE and token.type != TT_SEMICOLON and token.type != TT_ENDOFCODE)
              lb.push(token), token = read_next_token(scope);
            if (token.type != TT_LEFTPARENTH) {
              token.report_error(herr, "Expected function parmeters before %s");
              FATAL_RETURN(1); break;
            }
            
            token.type = TT_ENDOFCODE; lb.push(token);
            token.type = TT_LEFTPARENTH;
            lb.reset(); token_t kick = lb.get_token(herr);
            full_type ft = read_fulltype(&lb, kick, scope, this, herr);
            
            string opname; {
              ref_stack my_func_refs;
              read_referencers_post(my_func_refs, lex, token, scope, this, herr);
              if (my_func_refs.empty() or my_func_refs.top().type != ref_stack::RT_FUNCTION) {
                token.report_error(herr, "Expected function parameters for operator overload");
                return 1;
              }
              opname = "operator " + ft.toString();
              ft.refs.append_c(my_func_refs);
            }
            
            definition_function *const df = new definition_function(opname, scope, ft.def, ft.refs, ft.flags, inherited_flags);
            decl = df;
            
            decpair ins = scope->declare(decl->name, decl);
            if (!ins.inserted) { arg_key k(df->referencers); decl = ((definition_function*)ins.def)->overload(k, df, herr); }
            goto handled_declarator_block;
          }
        break;
      
      case TT_ASM: case TT_SIZEOF: case TT_ISEMPTY:
      case TT_OPERATOR: case TT_ELLIPSIS: case TT_LESSTHAN: case TT_GREATERTHAN: case TT_COLON:
      case TT_DECLITERAL: case TT_HEXLITERAL: case TT_OCTLITERAL: case TT_STRINGLITERAL: case TT_CHARLITERAL:
      case TT_NEW: case TT_DELETE: case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID:
      #include <User/token_cases.h>
      default:
        token.report_errorf(herr, "Unexpected %s in this scope");
        break;
      
      case TT_ENDOFCODE:
        return 0;
    }
    token = read_next_token(scope);
  }
}
