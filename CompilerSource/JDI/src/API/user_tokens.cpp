/**
 * @file  user_tokens.cpp
 * @brief API souce implementing stable functions for users to create tokens.
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

#include "user_tokens.h"

using namespace jdip;
namespace jdi {
  jdip::token_t create_token_dec_literal(const char* val, int len, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_DECLITERAL,filename,line,pos), val, len);
  }
  jdip::token_t create_token_hex_literal(const char* val, int len, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_HEXLITERAL,filename,line,pos), val, len);
  }
  jdip::token_t create_token_oct_literal(const char* val, int len, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_OCTLITERAL,filename,line,pos), val, len);
  }
  jdip::token_t create_token_operator(const char* op, int len, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_OPERATOR,filename,line,pos), op, len);
  }
  jdip::token_t create_token_from_definition(definition* def, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_DECLARATOR,filename,line,pos), def);
  }
  jdip::token_t create_token_opening_parenth(const char* filename, int line, int pos) {
    return token_t(token_basics(TT_LEFTPARENTH,filename,line,pos));
  }
  jdip::token_t create_token_closing_parenth(const char* filename, int line, int pos) {
    return token_t(token_basics(TT_RIGHTPARENTH,filename,line,pos));
  }
  jdip::token_t create_token_colon(const char* filename, int line, int pos) {
    return token_t(token_basics(TT_COLON,filename,line,pos));
  }
  jdip::token_t create_token_identifier(const char* name, int len, const char* filename, int line, int pos) {
    return token_t(token_basics(TT_IDENTIFIER,filename,line,pos), name, len);
  }
}
