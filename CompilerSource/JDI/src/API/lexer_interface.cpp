/**
 * @file  lexer_interface.cpp
 * @brief Microfile to implement the virtual destructor for \c lexer.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/
#include "lexer_interface.h"
jdi::lexer::~lexer() {}

using namespace jdip;

token_t lexer::get_token_in_scope(jdi::definition_scope *scope, error_handler *herr) {
  token_t res = get_token(herr);
  
  if (res.type == TT_IDENTIFIER) {
    const string name = res.content.toString();
    definition *def = scope->look_up(name);
    if (!def) {
      #ifdef DEBUG_MODE
        res.def = NULL;
      #endif
      return res;
    }
    res.def = def;
    
    if (def->flags & DEF_TYPENAME) {
      res.type = TT_DECLARATOR;
      return res;
    }
    
    res.type = TT_DEFINITION;
  }
  
  return res;
}
