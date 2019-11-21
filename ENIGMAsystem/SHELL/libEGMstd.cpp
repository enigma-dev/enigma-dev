/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

// This file contains functions that are used by virtually all components of ENIGMA.
// They are also, directly or otherwise, available to the user.

#include "libEGMstd.h"

#include <stdio.h>
#include <sstream>
#include <string>

namespace {
  ///Many integral types (such as long) vary based on platform and architecture. 
  ///To avoid crashing in string(#), we reserve generously.
  char buf[128];
}  // namespace

namespace enigma_user {
using std::string;

string toString(int n)                { return string(buf,sprintf(buf,"%d",  n)); }
string toString(long n)               { return string(buf,sprintf(buf,"%ld", n)); }
string toString(short n)              { return string(buf,sprintf(buf,"%d",  n)); }
string toString(unsigned n)           { return string(buf,sprintf(buf,"%u",  n)); }
string toString(unsigned long n)      { return string(buf,sprintf(buf,"%lu", n)); }
string toString(unsigned short n)     { return string(buf,sprintf(buf,"%u",  n)); }
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
string toString(long long n)          { return string(buf,sprintf(buf,"%lld", n)); }
string toString(unsigned long long n) { return string(buf,sprintf(buf,"%llu", n)); }
#else
string toString(long long n)          { return string(buf,sprintf(buf,"%lld", n)); }
string toString(unsigned long long n) { return string(buf,sprintf(buf,"%llu", n)); }
#endif
string toString(char n)               { return string(buf,sprintf(buf,"%d", n)); }
string toString(char* n)              { return string(n); }
string toString(float n)              { return string(buf,sprintf(buf,"%g", n)); }
string toString(double n)             { return string(buf,sprintf(buf,"%g", n)); }
string toString(long double n)        { return std::to_string(n); }
string toString(const void *n)        { return string(buf,sprintf(buf,"%p",  n)); }

string toString()                                        { return string ();     }
string toString(const string& str)                       { return string (str);  }
string toString(const string& str, size_t pos, size_t n) { return string (str, pos, n); }
string toString(const char *s, size_t n)                 { return string (s, n); }
string toString(const char *s)                           { return string (s);    }
string toString(size_t n, char c)                        { return string (n, c); }

string toString(const variant &a) {
  if (a.type == enigma_user::ty_real) {
    //Ensure that integral types don't pick up any baggage from being stored
    //  as a double in a var-type.
    double dVal = a.rval.d;
    long lVal = (long)dVal;
    if (dVal == lVal) {
      return toString(lVal);
    }
    return toString(dVal);
  }
  return a.sval();
}

string toString(const var &a) {
  //Arrays (2D and linear) are printed differently.
  if (a.array_height() > 1) {
    std::stringstream res;
    res << "<";
    for (int i = 0; i < a.array_height(); i++) {
      for (int j = 0; j < a.array_len(i); j++) {
        res << toString(const_cast<var &>(a)(i, j));
        if (j + 1 < a.array_len(i)) {
          res << " , ";
        }
      }
      if (i + 1 < a.array_height()) {
        res << " ; ";
      }
    }
    res << ">";
    return res.str();
  } else if (a.array_len() > 1) {
    std::stringstream res;
    res << "[";
    for (int i = 0; i < a.array_len(); i++) {
      res << toString(const_cast<var &>(a)[i]);
      if (i + 1 < a.array_len()) {
        res << " , ";
      }
    }
    res << "]";
    return res.str();
  } else {
    return toString(*a);
  }
}

}  // namespace enigma_user
