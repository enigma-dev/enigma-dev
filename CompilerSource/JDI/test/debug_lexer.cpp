/* Copyright (C) 2011 Josh Ventura
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
*/

#include "debug_lexer.h"
using namespace std;

void debug_lexer::clear() { tokens.clear(); }
debug_lexer& debug_lexer::operator<< (jdip::token_t t) { tokens.push_back(t); return *this; }
jdip::token_t debug_lexer::get_token(jdi::error_handler *) {
  if (tokens.size() > 0) {
    jdip::token_t res = tokens[0]; tokens.pop_front();
    return res;
  }
  return jdip::token_t(jdip::TT_ENDOFCODE,0,0,0);
}
debug_lexer::~debug_lexer() {}
