/**
  @file  parser.h
  @brief Catch-it-all header declaring the global, more abstract components of the parser.
  
  @section License
    Copyright (C) 2008-2013 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef _PARSER__H
#define _PARSER__H
#include <parser/object_storage.h>
int compile_parseAndLink(compile_context &ctex);
int compile_parseSecondary(compile_context &ctex);
#endif
