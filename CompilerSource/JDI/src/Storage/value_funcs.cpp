/**
 * @file value_funcs.cpp
 * @brief Source implementing functions for operating on values.
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
 * JustDefineIt is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * JustDefineIt. If not, see <http://www.gnu.org/licenses/>.
**/

#include "value_funcs.h"
#include <cstring>
#include <cfloat>
#include <cmath>

static inline jdi::value strdupcat(const char* s1, const char* s2) {
  int l1 = strlen(s1), l2 = strlen(s2)+1;
  char* res = new char[l1+l2];
  memcpy(res,s1,l1), memcpy(res+l1,s2,l2);
  return jdi::value(res);
}
static inline long fcomp(double x, double y) { return fabs(x - y) < DBL_EPSILON; }
static inline long fcomp(double x, long y) { return fabs(x - y) < DBL_EPSILON; }

namespace jdi {
  value values_add(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real + y.real);
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real + (long long) y.real);
    }
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) && y.type == VT_STRING) {
      size_t n = (size_t) x.real;
      if (n >= y.str.length()) return value("");
      return value(y.str.substr(n));
    }
    if (x.type == VT_STRING && (y.type == VT_DOUBLE || y.type == VT_INTEGER)) {
      size_t n = (size_t) y.real;
      if (n >= x.str.length()) return value("");
      return value(x.str.substr(n));
    }
    return value();
  }
  value values_subtract(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real - y.real);
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real - (long long) y.real);
    }
    return value();
  }
  value values_multiply(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real * y.real);
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real * (long long) y.real);
    }
    return value();
  }
  value values_divide(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real / y.real);
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real / (long long) y.real);
    }
    return value();
  }
  value values_modulo(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(fmod(x.real, y.real));
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real % (long long) y.real);
    }
    return value();
  }
  value values_lshift(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real / pow(2, y.real));
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      return value((long long) x.real << (long long) y.real);
    }
    return value();
  }
  value values_rshift(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if (((x.type == VT_DOUBLE  || y.type == VT_DOUBLE) &&
         (x.type == VT_INTEGER || y.type == VT_INTEGER)) ||
        (x.type == VT_DOUBLE && y.type == VT_DOUBLE)) {
      return value(x.real * pow(2, y.real));
    }
    if (x.type == VT_INTEGER && y.type == VT_INTEGER) {
      // TODO: Type promotion / integer truncation.
      // TODO: Sign extension.
      return value((long long) x.real >> (long long) y.real);
    }
    return value();
  }
  value values_bitand(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value((long long) x.real & (long long) y.real);
    }
    return value();
  }
  value values_bitor(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value((long long) x.real | (long long) y.real);
    }
    return value();
  }
  value values_bitxor(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value((long long) x.real ^ (long long) y.real);
    }
    return value();
  }
  
  inline bool known(const value &x) { return x.type == VT_DOUBLE || x.type == VT_INTEGER || x.type == VT_STRING; }
  inline value consolidate_unknowns(const value &x, const value &y) { return x.type == VT_DEPENDENT || y.type == VT_DEPENDENT? value(VT_DEPENDENT) : value(); } 
  
  value values_booland(const value& x, const value& y) { return known(x) && known(y)? value(long(bool(x) && bool(y))) : consolidate_unknowns(x, y); }
  value values_boolxor(const value& x, const value& y) { return known(x) && known(y)? value(long(bool(x) != bool(y))) : consolidate_unknowns(x, y); }
  value values_boolor(const value& x, const value& y)  { return known(x) && known(y)? value(long(bool(x) || bool(y))) : consolidate_unknowns(x, y); }
  
  
  value values_greater(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x > y);
    }
    return value();
  }
  value values_less(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x < y);
    }
    return value();
  }
  value values_greater_or_equal(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x >= y);
    }
    return value();
  }
  value values_less_or_equal(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x <= y);
    }
    return value();
  }
  value values_equal(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x == y);
    }
    return value();
  }
  value values_notequal(const value& x, const value& y) {
    if (x.type == VT_DEPENDENT || y.type == VT_DEPENDENT)
      return value(VT_DEPENDENT);
    if ((x.type == VT_INTEGER || x.type == VT_DOUBLE) &&
        (y.type == VT_INTEGER || y.type == VT_DOUBLE)) {
      return value(x != y);
    }
    return value();
  }
  value values_latter(const value&, const value& y) {
    return y;
  }

  value value_unary_increment(const value& x) {
    if (x.type == VT_DOUBLE) return value(x.real + 1);
    if (x.type == VT_INTEGER) return value((long long) x.real + 1);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_decrement(const value& x) {
    if (x.type == VT_DOUBLE) return value(x.real - 1);
    if (x.type == VT_INTEGER) return value((long long) x.real - 1);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_positive(const value& x) {
    if (x.type == VT_DOUBLE) return value(+x.real);
    if (x.type == VT_INTEGER) return value(+(long long) x.real);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_negative(const value& x) {
    if (x.type == VT_DOUBLE) return value(-x.real);
    if (x.type == VT_INTEGER) return value(-(long long) x.real);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_dereference(const value& x) {
    if (x.type == VT_STRING) return value((long) (x.str.empty() ? 0 : x.str[0]));
    if (x.type == VT_INTEGER) return value();
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_reference(const value&) {
    return value();
  }
  value value_unary_negate(const value& x) {
    if (x.type == VT_DOUBLE) return value(~(long long) x.real);
    if (x.type == VT_INTEGER) return value(~(long long) x.real);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }
  value value_unary_not(const value& x) {
    if (x.type == VT_DOUBLE) return value(!(bool) x);
    if (x.type == VT_INTEGER) return value(!(bool) x);
    if (x.type == VT_DEPENDENT) return value(VT_DEPENDENT);
    return value();
  }

  bool value_boolean(const value& v) {
    switch (v.type) {
      case VT_DOUBLE: case VT_INTEGER: case VT_STRING: return (bool) v;
      case VT_DEPENDENT: case VT_NONE: default: return false;
    }
  }
}
