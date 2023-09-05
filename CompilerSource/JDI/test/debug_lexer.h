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

#include <API/lexer_interface.h>
#include <deque>

class debug_lexer: public jdi::lexer {
  std::deque<jdip::token_t> tokens;
  public:
  void clear();
  debug_lexer &operator<< (jdip::token_t t);
  jdip::token_t get_token(jdi::error_handler *herr = jdi::def_error_handler);
  ~debug_lexer(); ///< Destruct and free any non-POD or pointer members.
};
