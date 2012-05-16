/**
 * @file value.cpp
 * @brief Source implementing constructors and destructors for \c jdi::value.
 * 
 * @section License
 * 
 * Copyright (C) 2011 Josh Ventura
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
  value::value(const value& v): val(v.val), type(v.type) { }
  value::~value() { if (type == VT_STRING) delete[] val.s; }
  
  value::operator long()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return (long)val.d; return 0; }
  value::operator double() const { if (type == VT_DOUBLE) return val.d; if (type == VT_INTEGER) return val.i; return 0; }
  value::operator bool()   const { if (type == VT_INTEGER) return val.i; if (type == VT_DOUBLE) return fabs(val.d) < DBL_EPSILON; return 0; }
  value::operator const char*() const { if (type == VT_STRING) return val.s; return NULL; }
}
