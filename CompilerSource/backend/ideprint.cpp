/** Copyright (C) 2008 Josh Ventura
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

#include "ideprint.h"
using std::string;
#include "general/estring.h"
#include "JavaCallbacks.h"

ideprint &ideprint::operator<< (string x) {
  f(x.c_str()); return *this;
}
ideprint &ideprint::operator<< (const char* x) {
  f(x); return *this;
}
ideprint &ideprint::operator<< (int x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (long x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned long x) {
  f(tostring(x).c_str()); return *this;
}

#if __cplusplus >= 201100
ideprint &ideprint::operator<< (long long x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned long long x) {
  f(tostring(x).c_str()); return *this;
}
#endif

ideprint &ideprint::operator<< (void *x) {
  f(tostringv(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (char x) {
  char a[2]; a[1] = 0; a[0] = x;
  f(a); return *this;
}
ideprint &ideprint::operator<< (unsigned char x) {
  char a[2]; a[1] = 0; a[0] = x;
  f(a); return *this;
}
ideprint &ideprint::operator<< (double x) {
  f(tostringd(x).c_str()); return *this;
}

ideprint::ideprint(void(*ftu)(const char*)): f(ftu) {}

#include <stdio.h>

// Now specialize it 
// Declare functions it'll call
void ide_dia_add_direct(const char* x) {
  printf("%s",x); fflush(stdout); ide_dia_add(x);
}
void ide_dia_add_debug(const char* x) {
  printf("%s",x); fflush(stdout); ide_dia_add(x);
}
// Link them together
ideprint user(ide_dia_add_direct);
ideprint edbg(ide_dia_add_debug);
