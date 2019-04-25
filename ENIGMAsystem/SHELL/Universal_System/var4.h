/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef ENIGMA_VAR4_H
#define ENIGMA_VAR4_H

// We want var and variant to support a lot of assignment types.
#include "var_te.h"
#include "lua_table.h"

#include <string>

namespace enigma {
  union rvt {
    double d;
    const void * p;
    rvt(double x): d(x) {}
    rvt(const void * x): p(x) {}
    #define var_e 1e-12
  };
}

namespace enigma_user {

struct var;

struct variant
{
  //Default type (-1 or real), changes based on "assume_uninitialized_is_zero" flag.
  //This variable is defined in a compiler-generated class.
  static const int default_type;

  enigma::rvt rval;
  std::string sval;
  int type;

  operator int();
  operator bool();
  operator char();
  operator long();
  operator short();
  operator unsigned();
  operator unsigned char();
  operator unsigned long();
  operator unsigned short();
  operator unsigned long long();
  operator long long();
  operator double();
  operator float();

  operator std::string();

  operator int() const;
  operator bool() const;
  operator char() const;
  operator long() const;
  operator short() const;
  operator unsigned() const;
  operator unsigned char() const;
  operator unsigned long() const;
  operator unsigned short() const;
  operator unsigned long long() const;
  operator long long() const;
  operator double() const;
  operator float() const;

  operator std::string() const;

  variant();
  variant(const void *p);
  types_extrapolate_alldecc(variant)

  types_extrapolate_alldec(variant& operator=)
  variant& operator=(const void* p);
  types_extrapolate_alldec(variant& operator+=)
  types_extrapolate_alldec(variant& operator-=)
  types_extrapolate_alldec(variant& operator*=)
  types_extrapolate_alldec(variant& operator/=)
  types_extrapolate_alldec(variant& operator%=)

  types_extrapolate_alldec(variant& operator<<=)
  types_extrapolate_alldec(variant& operator>>=)
  types_extrapolate_alldec(variant& operator&=)
  types_extrapolate_alldec(variant& operator|=)
  types_extrapolate_alldec(variant& operator^=)

  #undef EVCONST
  #define EVCONST const
  types_extrapolate_alldec(variant operator+)
  types_extrapolate_alldec(double  operator-)
  types_extrapolate_alldec(double  operator*)
  types_extrapolate_alldec(double  operator/)
  types_extrapolate_alldec(double  operator%)

  types_extrapolate_alldec(long operator<<)
  types_extrapolate_alldec(long operator>>)
  types_extrapolate_alldec(long operator&)
  types_extrapolate_alldec(long operator|)
  types_extrapolate_alldec(long operator^)

  #undef types_extrapolate_alldec
  #define types_extrapolate_alldec(prefix)\
   types_extrapolate_real_p  (prefix,;)\
   types_extrapolate_string_p(prefix,;)\
   prefix (const variant &x) const;\
   prefix (const var &x) const;

  types_extrapolate_alldec(bool operator==)
  types_extrapolate_alldec(bool operator!=)
  types_extrapolate_alldec(bool operator>=)
  types_extrapolate_alldec(bool operator<=)
  types_extrapolate_alldec(bool operator>)
  types_extrapolate_alldec(bool operator<)
  #undef EVCONST
  #define EVCONST

  char&     operator[] (int);
  variant&  operator++();
  double    operator++(int);
  variant&  operator--();
  double    operator--(int);
  variant&  operator*();

  #undef EVCONST
  #define EVCONST const
  bool      operator!() EVCONST;
  long      operator~() EVCONST;
  double    operator-() EVCONST;
  double    operator+() EVCONST;
  #undef EVCONST
  #define EVCONST

  ~variant();
};



#undef types_extrapolate_alldec
#define types_extrapolate_alldec(prefix)\
 types_extrapolate_real_p  (prefix,;)\
 types_extrapolate_string_p(prefix,;)\
 prefix (variant x);

struct var : variant {
  using variant::operator+;
  using variant::operator-;
  using variant::operator*;
  using variant::operator/;
  using variant::operator%;
  using variant::operator<;
  using variant::operator>;
  using variant::operator<<;
  using variant::operator>>;
  using variant::operator|;
  using variant::operator^;
  using variant::operator!;
  using variant::operator~;

  using variant::operator+=;
  using variant::operator-=;
  using variant::operator*=;
  using variant::operator/=;
  using variant::operator%=;
  using variant::operator<=;
  using variant::operator>=;
  using variant::operator<<=;
  using variant::operator>>=;
  using variant::operator|=;
  using variant::operator^=;
  using variant::operator!=;

  lua_table<variant> array1d;
  lua_table<lua_table<variant>> array2d;

  var() {}
  var(const var&) = default;
  var(variant value, size_t length, size_t height = 1):
      variant(value), array1d(value, length) {
    for (size_t i = 1; i < height; ++i) array2d[i].fill(value, length);
  }
  template<typename T> var(const T &v): variant(v) {}

  variant& operator*  () { return *this; }
  variant& operator() () { return *this; }
  variant& operator() (int ind) { return (*this)[ind]; }

  variant& operator[] (int ind) {
    if (!ind) return *this;
    return array1d[ind];
  }
  variant& operator() (int ind_2d,int ind_1d) {
    if (ind_2d) return array2d[ind_2d][ind_1d];
    if (ind_1d) return array1d[ind_1d];
    return *this;
  }

  const variant& operator*  () const { return *this; }
  const variant& operator() () const { return *this; }
  const variant& operator() (int ind) const { return (*this)[ind]; }

  const variant& operator[] (int ind) const {
    if (!ind) return *this;
    return array1d[ind];
  }
  const variant& operator() (int ind_2d,int ind_1d) const {
    if (ind_2d) return array2d[ind_2d][ind_1d];
    if (ind_1d) return array1d[ind_1d];
    return *this;
  }

  //Calculate array lengths.
  int array_len() const { return array1d.max_index(); }
  int array_height() const { return array2d.max_index(); }
  int array_len(int row) const {
    if (row) return array2d[row].max_index();
    return array1d.max_index();
  }

  size_t dense_length() {
    return array1d.dense_length();
  }

  const std::vector<variant> &dense_array_1d() {
    *array1d = *this;
    return array1d.dense_part();
  }

  template<typename T>
  std::vector<T> to_vector() const {
    return {array1d.dense_part().begin(),
            array1d.dense_part().end()};
  }

  ~var() {}
};


#undef EVCONST
#define EVCONST

types_binary_assign_extrapolate_declare(+, const variant&)
types_binary_assign_extrapolate_declare(-, const variant&)
types_binary_assign_extrapolate_declare(*, const variant&)
types_binary_assign_extrapolate_declare(/, const variant&)
types_binary_assign_extrapolate_declare(%, const variant&)

types_binary_assign_extrapolate_declare(<<, const variant&)
types_binary_assign_extrapolate_declare(>>, const variant&)
types_binary_assign_extrapolate_declare(&,  const variant&)
types_binary_assign_extrapolate_declare(|,  const variant&)
types_binary_assign_extrapolate_declare(^,  const variant&)


types_binary_extrapolate_alldecc(double, operator+, const variant&)
types_binary_extrapolate_alldecc(double, operator-, const variant&)
types_binary_extrapolate_alldecc(double, operator*, const variant&)
types_binary_extrapolate_alldecc(double, operator/, const variant&)
types_binary_extrapolate_alldecc(double, operator%, const variant&)

types_binary_bitwise_extrapolate_alldecc(operator<<, const variant&)
types_binary_bitwise_extrapolate_alldecc(operator>>, const variant&)
types_binary_bitwise_extrapolate_alldecc(operator&,  const variant&)
types_binary_bitwise_extrapolate_alldecc(operator|,  const variant&)
types_binary_bitwise_extrapolate_alldecc(operator^,  const variant&)

types_binary_extrapolate_alldecce(bool, operator==, const variant&)
types_binary_extrapolate_alldecce(bool, operator!=, const variant&)
types_binary_extrapolate_alldecce(bool, operator>=, const variant&)
types_binary_extrapolate_alldecce(bool, operator<=, const variant&)
types_binary_extrapolate_alldecce(bool, operator>,  const variant&)
types_binary_extrapolate_alldecce(bool, operator<,  const variant&)

#undef EVCONST
#define EVCONST

#undef types_extrapolate_real_p
#undef types_extrapolate_string_p
#undef types_extrapolate_mix_p
#undef types_extrapolate_alldec
#undef types_extrapolate_alldecc

#ifdef INCLUDED_FROM_SHELLMAIN
#define string(...) toString(__VA_ARGS__)
#endif

#undef unsigll

enum {
    ty_undefined = -1,
    ty_real = 0,
    ty_string = 1,
    ty_pointer = 2
};

bool is_undefined(variant var);
bool is_real(variant val);
bool is_string(variant val);
bool is_ptr(variant var);

} //enigma_user

using enigma_user::variant;
using enigma_user::var;

#endif //ENIGMA_VAR4_H
