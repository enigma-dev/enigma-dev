/**
 * @file user_tokens.h
 * @brief API header offering stable functions for users to create tokens.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include <System/token.h>

namespace jdi {
  /** Create a token representing a decimal literal.
      @param val       A string representation of the value of the token.
      @param len       The length of the string pointed to by val.
      @param filename  The filename from which this value was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_dec_literal(const char* val, int len, const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing a hexadecimal literal.
      @param val       A string representation of the value of the token, starting with 0x.
      @param len       The length of the string pointed to by val.
      @param filename  The filename from which this value was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_hex_literal(const char* val, int len, const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing an octal literal.
      @param val       A string representation of the value of the token, starting with 0.
      @param len       The length of the string pointed to by val.
      @param filename  The filename from which this value was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_oct_literal(const char* val, int len, const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing an operator.
      @param type      The internal token type for the operator, such as TT_PLUS.
      @param op        A string representation of the operator, such as "+" or "==".
      @param len       The length of the string pointed to by op.
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_operator(jdi::TOKEN_TYPE type, const char* op, int len, const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing an operator.
      @param def       The definition 
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_from_definition(definition* def, const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing an opening parenthesis.
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_opening_parenth(const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing a closing parenthesis.
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_closing_parenth(const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing a colon symbol.
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_colon(const char* filename = "<no file>", int line = -1, int pos = -1);
  /** Create a token representing an identifier.
      This will not be coerced for type or checked against keyword tables.
      @param name      The name of the identifier.
      @param len       The length of the string pointed to by val.
      @param filename  The filename from which this operator was read, if any.
      @param line      The line number in that file.
      @param pos       The position in that line.
  **/
  jdi::token_t create_token_identifier(const char* name, int len, const char* filename = "<no file>", int line = -1, int pos = -1);
}
