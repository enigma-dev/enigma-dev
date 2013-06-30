/**
* @file syntax_error.cpp
* @brief Source file of the syntax_error structure used for the plugin function calls.
*
* Write a description about the file here...
*
* @section License
*
* Copyright (C) 2013 Robert B. Colton
*
* This file is a part of the ENIGMA Development Environment.
*
* ENIGMA is free software: you can redistribute it and/or modify it under the
* terms of the GNU General Public License as published by the Free Software
* Foundation, version 3 of the license or any later version.
*
* This application and its source code is distributed AS-IS, WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along
* with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "syntax_error.h"

void syntax_error::set(int x, int y, int a, string s)
{
  error_sstring = s;
  err_str = error_sstring.c_str();
  line = x, position = y;
  absolute_index = a;
}

string error_sstring;
syntax_error ide_passback_error;
