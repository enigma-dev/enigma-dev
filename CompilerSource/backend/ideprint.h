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

#include <string>
#include <cstdint>
#define flushl '\n'
// The MinGW team is the most thoughtless, incompetent bunch of inconsistent morons who ever lived

struct ideprint {
  void (*f)(const char*);
  ideprint &operator<< (std::string);
  ideprint &operator<< (const char* x);
  ideprint &operator<< (void* x);
  ideprint &operator<< (int16_t x);
  ideprint &operator<< (int32_t x);
  ideprint &operator<< (int64_t x);
  ideprint &operator<< (uint16_t x);
  ideprint &operator<< (uint32_t x);
  ideprint &operator<< (uint64_t x);
  ideprint &operator<< (char x);
  ideprint &operator<< (unsigned char x);
  ideprint &operator<< (double x);
  ideprint(void (*f)(const char*));
};

void ide_dia_add_direct(const char*);
void ide_dia_add_debug(const char*);

extern ideprint user;
extern ideprint edbg;
