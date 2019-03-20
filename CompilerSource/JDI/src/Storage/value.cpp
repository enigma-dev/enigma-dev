/**
 * @file value.cpp
 * @brief Source implementing constructors and destructors for \c jdi::value.
 * 
 * @section License
 * 
 * Copyright (C) 2011-2012 Josh Ventura
 * This file is part of JustDefineIt.
 * 
 * JustDefineIt is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3 of the License, or (at your option) any later version.
 * 
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "value.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <cfloat>
#include <cmath>

namespace jdi {
  value::value(): type(VT_NONE) { val.d = 0; }
  value::value(double v): type(VT_DOUBLE)  { val.d = v; }
  value::value(long v):   type(VT_INTEGER) { val.i = v; }
  value::value(const char* v): type(VT_STRING) { val.s = v; }
  value::value(std::string v): type(VT_STRING) {
    char* s = new char[v.length()];
    for (size_t i = 0; i < v.length(); i++)
      s[i] = v[i];
    val.s = s;
  }
  value::value(const value& v): val(v.val), type(v.type) {
    if (type == VT_STRING) {
      size_t l = strlen(val.s) + 1;
      val.s = new char[l];
      memcpy((void*)val.s, v.val.s, l);
    }
  }
  value::~value() { if (type == VT_STRING) delete[] val.s; }
  
  std::string value::toString() {
    switch (type) {
      case VT_DOUBLE:  return std::to_string(val.d);
      case VT_INTEGER: return std::to_string(val.i);
      case VT_STRING:  return (val.s != nullptr ? val.s : "");
      case VT_NONE:    return "<nothing>";
      default:         return "<ERROR!>";
    }
  }
  
  bool value::operator==(const value& other) const { if (type != other.type) return false;
    switch (type) { case VT_DOUBLE: return fabs(val.d - other.val.d) <= DBL_EPSILON; case VT_INTEGER: return val.i == other.val.i; case VT_STRING: return !strcmp(val.s, other.val.s); case VT_NONE: default: return true; }
  }
  bool value::operator!=(const value& other) const { if (type != other.type) return true;
    switch (type) { case VT_DOUBLE: return fabs(val.d - other.val.d) > DBL_EPSILON; case VT_INTEGER: return val.i != other.val.i; case VT_STRING: return strcmp(val.s, other.val.s); case VT_NONE: default: return false; }
  }
  bool value::operator>(const value& other) const { if (type > other.type) return true; if (type < other.type) return false;
    switch (type) { case VT_DOUBLE: return val.d > other.val.d; case VT_INTEGER: return val.i > other.val.i; case VT_STRING: return strcmp(val.s, other.val.s) > 0; case VT_NONE: default: return false; }
  }
  bool value::operator<(const value& other) const { if (type > other.type) return false; if (type < other.type) return true;
    switch (type) { case VT_DOUBLE: return val.d < other.val.d; case VT_INTEGER: return val.i < other.val.i; case VT_STRING: return strcmp(val.s, other.val.s) < 0; case VT_NONE: default: return false; }
  }
  bool value::operator>=(const value& other) const { if (type > other.type) return true; if (type < other.type) return false;
    switch (type) { case VT_DOUBLE: return val.d >= other.val.d; case VT_INTEGER: return val.i >= other.val.i; case VT_STRING: return strcmp(val.s, other.val.s) >= 0; case VT_NONE: default: return true; }
  }
  bool value::operator<=(const value& other) const { if (type > other.type) return false; if (type < other.type) return true;
    switch (type) { case VT_DOUBLE: return val.d <= other.val.d; case VT_INTEGER: return val.i <= other.val.i; case VT_STRING: return strcmp(val.s, other.val.s) <= 0; case VT_NONE: default: return true; }
  }
  
#if __cplusplus >= 201100
  value::operator long long unsigned()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return (long long unsigned)val.d; return 0; }
  value::operator long long int()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return (long long int)val.d; return 0; }
#endif
  value::operator long()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return (long)val.d; return 0; }
  value::operator double() const { if (type == VT_DOUBLE) return val.d; if (type == VT_INTEGER) return val.i; return 0; }
  value::operator bool()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return fabs(val.d) < DBL_EPSILON; return 0; }
  value::operator int()    const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return fabs(val.d) < DBL_EPSILON; return 0; }
  //value::operator const char*() const { if (type == VT_STRING) return val.s; return nullptr; }
  value::operator std::string() { if (type == VT_STRING) return this->toString(); return ""; }
}
