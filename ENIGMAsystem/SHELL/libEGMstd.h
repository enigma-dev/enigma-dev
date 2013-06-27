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

#ifndef __libegmstd_h
#define __libegmstd_h

using std::string;

string toString(int);
string toString(long);
string toString(short);
string toString(unsigned);
string toString(unsigned long);
string toString(unsigned short);
string toString(long long n);
string toString(unsigned long long n);
string toString(char);
string toString(char*);
string toString(float);
string toString(double);

string toString();
string toString(const string& str);
string toString(const string& str, size_t pos, size_t n = string::npos);
string toString(const char *s, size_t n);
string toString(const char *s);
string toString(size_t n, char c);

string toString(const struct var&);
string toString(const struct variant&);

#endif
