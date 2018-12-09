/**
 * @file read_template_parameters.cpp
 * @brief Source implementing a function to handle class and structure declarations.
 * 
 * @section License
 * 
 * Copyright (C) 2012 Josh Ventura
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

#include <API/AST.h>
#include <Parser/bodies.h>
#include <API/compile_settings.h>

using namespace jdi;

int jdip::read_template_parameters(arg_key &argk, definition_template *temp, lexer *lex, token_t &token, definition_scope *scope, context_parser *cp, error_handler *herr)
{
  argk.mirror(temp);
  size_t args_given = 0;
  for (;;++args_given)
  {
    token = lex->get_token_in_scope(scope, herr);
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type == TT_SEMICOLON || token.type == TT_LEFTBRACE) {
      token.report_errorf(herr, "Expected closing triangle bracket to template parameters before %s");
      break;
    }
    
    if (token.type == TT_COMMA) continue;
    
    if (args_given < temp->params.size() and temp->params[args_given]->flags & DEF_TYPENAME) {
      full_type ft = read_fulltype(lex, token, scope, cp, herr);
      if (ft.def)
        argk[args_given].ft().swap(ft);
    } else {
      AST a;
      a.set_use_for_templates(true);
      a.parse_expression(token, lex, scope, precedence::comma+1, herr);
      if (args_given < temp->params.size()) {
        argk.put_value(args_given, a.eval());
        if (argk[args_given].val().type != VT_INTEGER) {
          token.report_error(herr, "Expression must give integer result");
          FATAL_RETURN(1); argk[args_given].val() = long(0);
        }
      }
    }
    
    if (token.type == TT_GREATERTHAN)
      break;
    if (token.type != TT_COMMA) {
      token.report_errorf(herr, "Comma expected here before %s");
      break;
    }
  }
  if (args_given > temp->params.size()) {
      token.report_error(herr, "Too many template parameters provided to `" + temp->toString(0,0) + "'");
      FATAL_RETURN(1);
  }
  int bad_params = 0;
  for (size_t i = 0; i < temp->params.size(); ++i)
    if ((argk[i].type == arg_key::AKT_FULLTYPE and !argk[i].ft().def)
    or  ((argk[i].type == arg_key::AKT_VALUE and (argk[i].val().type == VT_NONE))))
      ++bad_params;
  if (bad_params) {
    token.report_error(herr, "Insufficient parameters to `" + temp->toString(0,0) + "'; " + value((long)bad_params).toString() + " more required" );
    for (size_t i = 0; i < temp->params.size(); ++i)
      if ((argk[i].type == arg_key::AKT_FULLTYPE and !argk[i].ft().def)
      or  ((argk[i].type == arg_key::AKT_VALUE and (argk[i].val().type == VT_NONE))))
        token.report_error(herr, "Missing parameter " + value((long)i).toString() + ": parameter is not defaulted");
    FATAL_RETURN(1);
  }
  return 0;
}
