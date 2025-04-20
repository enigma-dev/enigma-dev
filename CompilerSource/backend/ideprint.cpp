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

#include "ideprint.h"
#include "JavaCallbacks.h"

ideprint::ideprint(void(*ftu)(const char*)): f(ftu) {}

#include <iostream>

// Now specialize it 
// Declare functions it'll call
void ide_dia_add_direct(const char* x) {
  std::cout << x; fflush(stdout); ide_dia_add(x);
}
void ide_dia_add_debug(const char* x) {
  std::cout << x; fflush(stdout); ide_dia_add(x);
}
// Link them together
ideprint user(ide_dia_add_direct);
ideprint edbg(ide_dia_add_debug);
