/**
 * @file  jdi.h
 * @brief Main header for interfacing with JDI.
 * 
 * The sole purpose of this header is to document and include other headers.
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

#include "jdi.h"
#include "System/lex_cpp.h"

namespace jdi {
  void initialize() {
    builtin = new context(0);
    add_gnu_declarators();
    builtin->load_standard_builtins();
    builtin->output_types();
  }
  
  void clean_up() {
    cleanup_declarators();
    jdip::lexer_cpp::cleanup();
    delete builtin;
    builtin = NULL;
  }
}
