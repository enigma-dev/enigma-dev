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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "value.h"
#include <cstdio>
#include <cstring>
#include <cfloat>
#include <cmath>

namespace jdi {
  value::value():                       real(),  str(),  type(VT_NONE) {}
  value::value(float v):                real(v), str(),  type(VT_DOUBLE) {}
  value::value(double v):               real(v), str(),  type(VT_DOUBLE) {}
  value::value(long double v):          real(v), str(),  type(VT_DOUBLE) {}
  value::value(signed v):               real(v), str(),  type(VT_INTEGER) {}
  value::value(signed long v):          real(v), str(),  type(VT_INTEGER) {}
  value::value(signed long long v):     real(v), str(),  type(VT_INTEGER) {}
  value::value(unsigned v):             real(v), str(),  type(VT_INTEGER) {}
  value::value(unsigned long v):        real(v), str(),  type(VT_INTEGER) {}
  value::value(unsigned long long v):   real(v), str(),  type(VT_INTEGER) {}
  value::value(const VT& t):            real(),  str(),  type(t) {}
  value::value(std::string v):          real(),  str(v), type(VT_STRING) {}
  value::value(const value& v): real(v.real), str(v.str), type(v.type) {}
  
  std::string value::toString() const {
    char buf[128];
    switch (type) {
      case VT_DOUBLE:    sprintf(buf, "%.32g", (double) real);      return buf;
      case VT_INTEGER:   sprintf(buf, "%ld", (unsigned long) real); return buf;
      case VT_STRING:    return str;
      case VT_DEPENDENT: return "(<dependent value>)";
      case VT_NONE:      return "<nothing>";
      default:           return "<ERROR!>";
    }
  }
  
  value &value::operator=(value &&other) {
    type = other.type;
    if (type == VT_STRING) str = other.str;
    else real = other.real;
    return *this;
  }
  
  bool value::operator==(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return fabs(real - other.real) <= DBL_EPSILON;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str == other.str;
    return false;
  }
  bool value::operator!=(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return fabs(real - other.real) > DBL_EPSILON;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str != other.str;
    return true;
  }
  bool value::operator>(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return real > other.real;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str > other.str;
    return false;
  }
  bool value::operator<(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return real < other.real;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str < other.str;
    return false;
  }
  bool value::operator>=(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return real + DBL_EPSILON >= other.real;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str >= other.str;
    return false;
  }
  bool value::operator<=(const value& other) const {
    if ((this->type == VT_DOUBLE || this->type == VT_INTEGER) &&
        (other.type == VT_DOUBLE || other.type == VT_INTEGER)) {
      return real - DBL_EPSILON <= other.real;
    }
    if (type == VT_STRING && other.type == VT_STRING) return str <= other.str;
    return false;
  }
  
  value::operator int()    const { if (type == VT_INTEGER || type == VT_DOUBLE) return (int) real; return 0; }
  value::operator long()   const { if (type == VT_INTEGER || type == VT_DOUBLE) return (long) real; return 0; }
  value::operator double() const { if (type == VT_INTEGER || type == VT_DOUBLE) return (double) real; return 0; }
  value::operator bool()   const { if (type == VT_INTEGER || type == VT_DOUBLE) return real > DBL_EPSILON || real < -DBL_EPSILON; return 0; }
  value::operator std::string_view() const {
    if (type == VT_STRING) return str;
    return "";
  }
}
