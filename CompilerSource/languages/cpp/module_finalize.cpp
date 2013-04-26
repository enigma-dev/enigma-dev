/**
  @file  module_finalize.cpp
  @brief Source implementing the C++ plugin's method for writing the end cap of a game module.
  
  @section License
    Copyright (C) 2010-2013 Josh Ventura
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

#include <cstdio>
#include <languages/lang_CPP.h>

int lang_CPP::resource_writer_cpp::module_finalize(compile_context &) {
  fwrite("\0\0\0\0res0", 8, 1, gameModule);
  fwrite(&resourceblock_start, 4, 1, gameModule);
  fclose(gameModule);
  return 0;
}
