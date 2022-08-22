/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_IDEPRINT_H
#define ENIGMA_IDEPRINT_H

#include "general/estring.h"

#include <string>
#include <cstdint>

#define flushl '\n'
// The MinGW team is the most thoughtless, incompetent bunch of inconsistent morons who ever lived

struct ideprint {
  void (*f)(const char*);
  template<typename T> inline ideprint &operator<< (T x) { f(std::to_string(x).c_str()); return *this; }
  ideprint(void (*f)(const char*));
};

template<> inline ideprint& ideprint::operator<< <const char*>(const char* x) { f(x); return *this; }
template<> inline ideprint& ideprint::operator<< <std::string>(std::string x) { f(x.c_str()); return *this; }
template<> inline ideprint& ideprint::operator<< <void *>(void* x) { f(tostringv(x).c_str()); return *this; }
template<> inline ideprint& ideprint::operator<< <char>(char x) { char a[2]; a[1] = 0; a[0] = x; f(a); return *this; }

void ide_dia_add_direct(const char*);
void ide_dia_add_debug(const char*);

extern ideprint user;
extern ideprint edbg;

#endif
