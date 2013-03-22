/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include <string>
#include <stdio.h>

#include "libEGMstd.h"

using namespace std;

string toString(int n)                { char buf[12]; return string(buf,sprintf(buf,"%d",  n)); }
string toString(long n)               { char buf[12]; return string(buf,sprintf(buf,"%ld", n)); }
string toString(short n)              { char buf[12]; return string(buf,sprintf(buf,"%d",  n)); }
string toString(unsigned n)           { char buf[12]; return string(buf,sprintf(buf,"%u",  n)); }
string toString(unsigned long n)      { char buf[12]; return string(buf,sprintf(buf,"%lu", n)); }
string toString(unsigned short n)     { char buf[12]; return string(buf,sprintf(buf,"%u",  n)); }
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
string toString(long long n)          { char buf[32]; return string(buf,sprintf(buf,"%I64d", n)); }
string toString(unsigned long long n) { char buf[32]; return string(buf,sprintf(buf,"%I64u", n)); }
#else
string toString(long long n)          { char buf[32]; return string(buf,sprintf(buf,"%lld", n)); }
string toString(unsigned long long n) { char buf[32]; return string(buf,sprintf(buf,"%llu", n)); }
#endif
string toString(char n)               { char buf[8];  return string(buf,sprintf(buf,"%d", n)); }
string toString(char* n)              { return string(n); }
string toString(float n)              { char buf[32]; return string(buf,sprintf(buf,"%g", n)); }
string toString(double n)             { char buf[32]; return string(buf,sprintf(buf,"%g", n)); }

string toString()                                        { return string ();     }
string toString(const string& str)                       { return string (str);  }
string toString(const string& str, size_t pos, size_t n) { return string (str, pos, n); }
string toString(const char *s, size_t n)                 { return string (s, n); }
string toString(const char *s)                           { return string (s);    }
string toString(size_t n, char c)                        { return string (n, c); }
