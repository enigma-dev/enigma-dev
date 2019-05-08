/**
  @file  compile_includes.h
  @brief Contains general-purpose functions for use in compile.cpp.

  This file seems like a pile of shit and will likely be removed soon in favor of
  just sticking its few methods in the including source.

  @section License
    Copyright (C) 2008-2012 Josh Ventura
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

#ifndef ENIGMA_COMPILE_INCLUDES_H
#define ENIGMA_COMPILE_INCLUDES_H

#include <JDI/src/Storage/definition.h>

extern string tostring(int val);

inline string format_error(string code,string err,int pos)
{
  if (pos == -1)
    return err;

  int line = 1, lp = 1;
  for (int i=0; i<pos; i++,lp++) {
    if (code[i] =='\n')
      line++, lp = 0, i += code[i+1] == '\n';
    else if (code[i] == '\n') line++, lp = 0;
  }
  return "Line " + tostring(line) + ", position " + tostring(lp) + " (absolute " + tostring(pos) + "): " + err;
}

#endif
