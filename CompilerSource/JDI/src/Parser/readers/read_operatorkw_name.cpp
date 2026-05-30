/**
 * @file read_operatorkw_name.cpp
 * @brief Source handling reading operator function names.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2013 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/


#include <Parser/context_parser.h>
#include <API/compile_settings.h>

using std::string;

string jdi::context_parser::read_operatorkw_name(token_t &token, definition_scope *scope)
{
  string res;
  token = lex->get_token_in_scope(scope);
  if (token.gloss_type() == GTT_OPERATORMISC || token.gloss_type() == GTT_ANGLE
      || token.gloss_type() == GTT_ARITHMETIC || token.gloss_type() == GTT_EQUAL
      || token.gloss_type() == GTT_RELATIVE_ASSIGN) {
    res = "operator" + token.content.toString();
    token = lex->get_token_in_scope(scope);
  }
  else if (token.type == TT_LEFTBRACKET) {
    res = "operator[]";
    token = lex->get_token_in_scope(scope);
    if (token.type != TT_RIGHTBRACKET) {
      token.report_error(herr, "Expected closing bracket for `operator[]' definition");
      FATAL_RETURN("");
    }
    else
      token = lex->get_token_in_scope(scope);
  }
  else if (token.type == TT_LEFTPARENTH) {
    res = "operator()";
    token = lex->get_token_in_scope(scope);
    if (token.type != TT_RIGHTPARENTH) {
      token.report_error(herr, "Expected closing parenthesis for `operator()' definition");
      FATAL_RETURN("");
    }
    else
      token = lex->get_token_in_scope(scope);
  }
  else if (token.type == TT_COMMA) {
    res = "operator,";
    token = lex->get_token_in_scope(scope);
  }
  else if (token.type == TT_NEW) {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_LEFTBRACKET) {
      token = lex->get_token_in_scope(scope);
      if (token.type != TT_RIGHTBRACKET) {
        token.report_error(herr, "Expected closing bracket for `operator new[]' definition");
        FATAL_RETURN("");
      }
      else
        token = lex->get_token_in_scope(scope);
      res = "operator new[]";
    }
    else
      res = "operator new";
  }
  else if (token.type == TT_DELETE) {
    token = lex->get_token_in_scope(scope);
    if (token.type == TT_LEFTBRACKET) {
      token = lex->get_token_in_scope(scope);
      if (token.type != TT_RIGHTBRACKET) {
        token.report_error(herr, "Expected closing bracket for `operator new[]' definition");
        FATAL_RETURN("");
      }
      else
        token = lex->get_token_in_scope(scope);
      res = "operator delete[]";
    }
    else
      res = "operator delete";
  }
  else {
    token.report_errorf(herr, "Unexpected %s following `operator' keyword; does not form a valid operator");
    FATAL_RETURN("");
  }
  return res;
}
