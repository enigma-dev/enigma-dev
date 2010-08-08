/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "ideprint.h"
using std::string;
#include "../general/estring.h"
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
ideprint &ideprint::operator<< (long long x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned long x) {
  f(tostring(x).c_str()); return *this;
}
ideprint &ideprint::operator<< (unsigned long long x) {
  f(tostring(x).c_str()); return *this;
}
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
