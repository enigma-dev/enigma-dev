/** Copyright (C) 2008-2013 Josh Ventura
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

// This file contains functions that are used by virtually all components of ENIGMA.
// They are also, directly or otherwise, available to the user.

#ifndef ENIGMA_LIBEGMSTD_H
#define ENIGMA_LIBEGMSTD_H

#include "Universal_System/var4.h"

#include <string>

namespace enigma_user {

std::string toString(int);
std::string toString(long);
std::string toString(short);
std::string toString(unsigned);
std::string toString(unsigned long);
std::string toString(unsigned short);
std::string toString(long long n);
std::string toString(unsigned long long n);
std::string toString(char);
std::string toString(char*);
std::string toString(float);
std::string toString(double);
std::string toString(long double);

std::string toString();
std::string toString(const std::string& str);
std::string toString(const std::string& str, size_t pos, size_t n = std::string::npos);
std::string toString(const char* s, size_t n);
std::string toString(const char* s);
std::string toString(size_t n, char c);

std::string toString(const struct var&);
std::string toString(const struct evariant&);

}  // namespace enigma_user

using enigma_user::toString; // TODO: Factor out

#endif  //ENIGMA_LIBEGMSTD_H
