/**
 * @file read_expression.cpp
 * @brief Source implementing the utility function to read an expression
 *        and parse for a value.
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
**/

#include <Parser/bodies.h>
#include <API/AST.h>
//#include <>
using namespace jdi;
using namespace jdip;

value jdip::context_parser::read_expression(token_t &token, TOKEN_TYPE closing_token, definition_scope *scope)
{
  token = read_next_token( scope);
  while (token.type != TT_COMMA and token.type != closing_token)
  {
    if (token.type == TT_ENDOFCODE)
      return value();
    token = read_next_token( scope);
  }
  return value();
}
