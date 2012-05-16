/**
 * @file  handle_scope.cpp
 * @brief Source implementing a massive delegator which populates a scope.
 * 
 * This file does a huge amount of work.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
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
 * 
**/

#include <Parser/bodies.h>
#include <API/compile_settings.h>

int jdip::context_parser::handle_scope(definition_scope *scope, token_t& token, unsigned inherited_flags)
{
  token = read_next_token(scope);
  for (;;)
  {
    switch (token.type)
    {
      case TT_DECLARATOR: case TT_DECFLAG:
      case TT_CLASS: case TT_STRUCT: case TT_ENUM:
          if (handle_declarators(scope, token, inherited_flags))
            return 1;
          if (token.type != TT_SEMICOLON)
            return (token.report_error(herr, "Expected semicolon here after declaration"), 1);
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
          if (handle_declarators(scope, token, inherited_flags | DEF_EXTERN))
            return 1;
          if (token.type != TT_SEMICOLON)
            return (token.report_error(herr, "Expected semicolon here after declaration"), 1);
        break;
      
      case TT_COMMA:
          token.report_error(herr, "Unexpected comma at this point.");
        return 1;
      
      case TT_SEMICOLON:
          /* Printing a warning here is advisable but unnecessary. */
        break;
      
      case TT_NAMESPACE: if (handle_namespace(scope,token)) return 1; break;
      case TT_LEFTPARENTH:  token.report_error(herr, "Stray opening parenthesis."); return 1;
      case TT_RIGHTPARENTH: token.report_error(herr, "Stray closing parenthesis."); return 1;
      case TT_LEFTBRACKET:  token.report_error(herr, "Stray opening bracket."); return 1;
      case TT_RIGHTBRACKET: token.report_error(herr, "Stray closing bracket."); return 1;
      case TT_LEFTBRACE:    token.report_error(herr, "Expected scope declaration before opening brace."); return 1;
      case TT_RIGHTBRACE:   return 0;
      
      case TT_TYPEDEF:
        token = read_next_token(scope);
        if (handle_declarators(scope,token,inherited_flags | DEF_TYPENAME)) FATAL_RETURN(1); break;
      
      case TT_UNION:
      case TT_TYPENAME:
      
      case TT_ASM:
      
      case TT_IDENTIFIER:
      
      case TT_OPERATOR: case TT_LESSTHAN: case TT_GREATERTHAN:
      case TT_COLON:
      
      case TT_SCOPE:
      case TT_TILDE:
      case TT_STRINGLITERAL:
      
      case TT_DECLITERAL:
      case TT_HEXLITERAL:
      case TT_OCTLITERAL:
      
      case TT_USING: case TT_PUBLIC: case TT_PRIVATE: case TT_PROTECTED:
      case TT_TEMPLATE:
      
      case TTM_CONCAT: case TTM_TOSTRING: case TT_INVALID:
      default:
        token.report_error(herr, "INVALID TOKEN TYPE RETURNED");
        #ifdef DEBUG_MODE
          cout << TOKEN_TYPE_NAME[token.type];
          if (token.type == TT_IDENTIFIER || token.type == TT_OPERATOR) cout << "[" << string((const char*)token.extra.content.str, token.extra.content.len) << "]";
          cout << endl;
        #endif
        break;
      
      case TT_ENDOFCODE:
        return 0;
    }
    token = read_next_token( scope);
  }
}
