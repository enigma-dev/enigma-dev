/** Copyright (C) 2019 Josh Ventura
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

#ifndef ENIGMA_VAR4_H
#define ENIGMA_VAR4_H

#include "lua_table.h"

#include <cmath>
#include <limits>
#include <string>

#ifdef JUST_DEFINE_IT_RUN
#  include <stdint.h>
#  define constexpr const
#  define decltype(x) double
#else
#  include <cstdint>
#  include <type_traits>
#endif

namespace enigma {

// Some syntactic sugar for declaring things 
template<typename E> struct EnabledType {
  typedef E T;
  static constexpr bool V = true;
  typedef bool EN;
  template<typename R> struct returns {
    typedef R T;
  };
};

// Like enable_if, but invented here
template<typename E, bool Enable> struct MaybeEnabled {};
template<typename E> struct MaybeEnabled<E, true>: EnabledType<E> {};

// Tests whether one type can be cast explicitly to another.
// Note that this excludes explicit construction; this is for a direct cast.
template<typename T, typename U> class CanCast {
  template<typename I, typename O = U> struct Kernel {
    static constexpr bool V = false;
  };
  template<typename I> struct Kernel<I, decltype((U) *(I*)nullptr)> {
    static constexpr bool V = true;
  };

 public:
  static constexpr bool V = Kernel<T>::V;
};

template<typename T> struct SIntTypeEnabler {};
template<> struct SIntTypeEnabler<int8_t>  : EnabledType<int8_t>  {};
template<> struct SIntTypeEnabler<int16_t> : EnabledType<int16_t> {};
template<> struct SIntTypeEnabler<int32_t> : EnabledType<int32_t> {};
template<> struct SIntTypeEnabler<int64_t> : EnabledType<int64_t> {};

template<typename T> struct UIntTypeEnabler {};
template<> struct UIntTypeEnabler<uint8_t>   : EnabledType<uint8_t>  {};
template<> struct UIntTypeEnabler<uint16_t>  : EnabledType<uint16_t> {};
template<> struct UIntTypeEnabler<uint32_t>  : EnabledType<uint32_t> {};
template<> struct UIntTypeEnabler<uint64_t>  : EnabledType<uint64_t> {};
template<> struct UIntTypeEnabler<long long> : EnabledType<long long> {};

template<typename T> struct IntTypeEnabler {};
template<> struct IntTypeEnabler<int8_t>    : EnabledType<int8_t>    {};
template<> struct IntTypeEnabler<int16_t>   : EnabledType<int16_t>   {};
template<> struct IntTypeEnabler<int32_t>   : EnabledType<int32_t>   {};
template<> struct IntTypeEnabler<int64_t>   : EnabledType<int64_t>   {};
template<> struct IntTypeEnabler<uint8_t>   : EnabledType<uint8_t>   {};
template<> struct IntTypeEnabler<uint16_t>  : EnabledType<uint16_t>  {};
template<> struct IntTypeEnabler<uint32_t>  : EnabledType<uint32_t>  {};
template<> struct IntTypeEnabler<uint64_t>  : EnabledType<uint64_t>  {};
template<> struct IntTypeEnabler<long long> : EnabledType<long long> {};

template<typename T> struct FloatTypeEnabler {};
template<> struct FloatTypeEnabler<float>       : EnabledType<float>  {};
template<> struct FloatTypeEnabler<double>      : EnabledType<double> {};
template<> struct FloatTypeEnabler<long double> : EnabledType<long double> {};

// Any explicitly numeric type. Note that this does NOT include bool and enum!
template<typename T> struct NumericTypeEnabler {};
template<> struct NumericTypeEnabler<char>        : EnabledType<int8_t>      {};
template<> struct NumericTypeEnabler<int8_t>      : EnabledType<int8_t>      {};
template<> struct NumericTypeEnabler<int16_t>     : EnabledType<int16_t>     {};
template<> struct NumericTypeEnabler<int32_t>     : EnabledType<int32_t>     {};
template<> struct NumericTypeEnabler<int64_t>     : EnabledType<int64_t>     {};
template<> struct NumericTypeEnabler<uint8_t>     : EnabledType<uint8_t>     {};
template<> struct NumericTypeEnabler<uint16_t>    : EnabledType<uint16_t>    {};
template<> struct NumericTypeEnabler<uint32_t>    : EnabledType<uint32_t>    {};
template<> struct NumericTypeEnabler<uint64_t>    : EnabledType<uint64_t>    {};
template<> struct NumericTypeEnabler<long long>   : EnabledType<long long>   {};
template<> struct NumericTypeEnabler<float>       : EnabledType<float>       {};
template<> struct NumericTypeEnabler<double>      : EnabledType<double>      {};
template<> struct NumericTypeEnabler<long double> : EnabledType<long double> {};

// More lax than NumericType, but does not permit any type that can also be cast
// to a string. Allows booleans and enum constants, but not vars or variants.
template<typename T> struct NonStringNumberTypeEnabler
    : MaybeEnabled<T, CanCast<T, double>::V && !CanCast<T, std::string>::V> {};

template<typename T> struct NonVariantTypeEnabler
    : MaybeEnabled<T, CanCast<T, double>::V != CanCast<T, std::string>::V> {};

template<typename T, typename U, typename V = decltype(-*(T*)0 | -*(U*)0)>
struct EnumAndNumericBinaryFuncEnabler
    : MaybeEnabled<V, std::is_enum<T>::value || std::is_enum<U>::value> {};

template<typename T> struct ArithmeticTypeEnabler {};
template<> struct ArithmeticTypeEnabler<int8_t>    : EnabledType<int8_t> {};
template<> struct ArithmeticTypeEnabler<int16_t>   : EnabledType<int16_t> {};
template<> struct ArithmeticTypeEnabler<int32_t>   : EnabledType<int32_t> {};
template<> struct ArithmeticTypeEnabler<int64_t>   : EnabledType<int64_t> {};
template<> struct ArithmeticTypeEnabler<uint8_t>   : EnabledType<uint8_t> {};
template<> struct ArithmeticTypeEnabler<uint16_t>  : EnabledType<uint16_t> {};
template<> struct ArithmeticTypeEnabler<uint32_t>  : EnabledType<uint32_t> {};
template<> struct ArithmeticTypeEnabler<uint64_t>  : EnabledType<uint64_t> {};
template<> struct ArithmeticTypeEnabler<long long> : EnabledType<long long> {};
template<> struct ArithmeticTypeEnabler<float>     : EnabledType<float> {};
template<> struct ArithmeticTypeEnabler<double>    : EnabledType<double> {};
template<> struct ArithmeticTypeEnabler<long double> : EnabledType<long double> {};

template<typename X, typename Y = X, typename Z = Y,
         typename W = Z, typename P = W, typename R = P,
         typename EN = decltype(X() - Y() - Z() - W() - P() - R())>
struct ArithmeticTypes: EnabledType<EN> {};

template<typename T> struct StringTypeEnabler {};
template<> struct StringTypeEnabler<std::string> : EnabledType<std::string> {};
template<> struct StringTypeEnabler<const char*> : EnabledType<const char*> {};
// template<size_t K> struct StringTypeEnabler<const char[K]> : EnabledType<const char[K]> {};
// TODO: support string_view

#ifndef JUST_DEFINE_IT_RUN

#define RLY_INLINE __attribute__((always_inline))

template<typename T> using SIntType = typename SIntTypeEnabler<T>::T;
template<typename T> using UIntType = typename UIntTypeEnabler<T>::T;
template<typename T> using IntType = typename IntTypeEnabler<T>::T;
template<typename T> using FloatType = typename FloatTypeEnabler<T>::T;
template<typename T> using NumericType = typename NumericTypeEnabler<T>::T;
template<typename T> using ArithmeticType = typename ArithmeticTypeEnabler<T>::T;
template<typename T> using StringType = typename StringTypeEnabler<T>::T;

template<typename T, typename U> using NumericFunc =
    typename NumericTypeEnabler<T>::template returns<U>::T;
template<typename T, typename U> using NSNumberFunc =
    typename NonStringNumberTypeEnabler<T>::template returns<U>::T;
template<typename T, typename U> using StringFunc =
    typename StringTypeEnabler<T>::template returns<U>::T;
template<typename T> using NonStringNumber =
    typename NonStringNumberTypeEnabler<T>::T;

#else

#define RLY_INLINE
#endif  // JUST_DEFINE_IT_RUN

union rvt {
  double d;
  const void * p;
  rvt(double x): d(x) {}
  rvt(const void * x): p(x) {}
};

}  // namespace enigma

namespace enigma_user {

enum {
  ty_undefined = -1,
  ty_real = 0,
  ty_string = 1,
  ty_pointer = 2
};

}  // namespace enigma_user

struct var;
struct variant;

namespace enigma_user {

std::string toString(const void*);
std::string toString(double);

#ifdef INCLUDED_FROM_SHELLMAIN
#define string(...) toString(__VA_ARGS__)
#endif

}


//▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚
//██▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜██████████████████████████████
//██▌ variant: a smart pair of string and double ▐██████████████████████████████
//██▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟██████████████████████████████
//▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞

struct variant {
  // Default type (-1 or real), changes based on "assume_uninitialized_is_zero."
  // (This variable is defined in a compiler-generated class.)
  static const int default_type;

  enigma::rvt rval;
  std::string sval;
  int type;

  static constexpr double epsilon = 1e-12;
  static constexpr int ty_real =  enigma_user::ty_real;
  static constexpr int ty_string =  enigma_user::ty_string;

  // Real-valued casts.
  template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
  operator T() const {
    return (T) rval.d;
  }
  // String cast.
  operator const std::string&() const {
    return sval;
  }

  std::string to_string() const {
    if (type == ty_string)  return sval;
    if (type == enigma_user::ty_pointer) return enigma_user::toString(rval.p);
    if (type == ty_real) return enigma_user::toString(rval.d);
    return "<undefined>";
  }

  // Our bool cast is special... thanks, Delphi
  operator bool() const {
    // TODO: Enable turning this off...
    return lrint(rval.d) > 0;
  }

  // How to construct a variant: the basics
  variant():
      rval(0.0), sval(), type(default_type) {}
  variant(const void *p):
      rval(p), type(enigma_user::ty_pointer) {}
  variant(const variant &x):
      rval(x.rval.d), sval(x.sval), type(x.type) {}
  variant(variant &&x):
      rval(x.rval.d), sval(std::move(x.sval)), type(x.type) {}

  // Construct a variant from numeric types
  template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
  variant(T number): rval((double) number), type(ty_real) {}

  variant(const std::string &str): rval(0.), sval(str), type(ty_string) {}
  variant(std::string &&str): rval(0.), sval(str), type(ty_string) {}

  // Assignment operators
  // ===========================================================================

  variant& operator=(const variant &v) {
    rval = v.rval;
    if ((type = v.type) == ty_string) sval = v.sval;
    return *this;
  }

  // Assignment to a numeric type
  template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
  variant& operator=(T number) {
    rval.d = (double) number;
    type = ty_real;
    return *this;
  }

  // Assignment to a string type
  variant& operator=(const std::string &str) {
    sval = str;
    type = ty_string;
    return *this;
  }
  variant& operator=(std::string &&str) {
    sval = std::move(str);
    type = ty_string;
    return *this;
  }

  // The plus operator is actually really special.
  template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
  variant& operator+=(T number) {
    rval.d += number;
    return *this;
  }
  template<typename T, typename enigma::StringTypeEnabler<T>::EN = 0>
  variant& operator+=(const T &str) {
    sval += str;
    return *this;
  }
  variant& operator+=(const variant &other) RLY_INLINE {
    if (type == ty_string) sval += other.sval;
    else rval.d += other.rval.d;
    return *this;
  }

  variant operator+(const variant &other) const RLY_INLINE {
    if (type == ty_string) return sval + other.sval;
    return rval.d + other.rval.d;
  }
  template<typename T, typename enigma::StringTypeEnabler<T>::EN = 0>
  RLY_INLINE std::string operator+(const T &str) const {
    return to_string() + str;
  }

  // Arithmetic operators.
  // ===========================================================================

  // Basic arithmetic operators.
  template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
  decltype(rval.d + T()) operator+(T x) const {
    return rval.d + x;
  }
  template<typename T> decltype(rval.d - T()) operator-(T x) const {
    return rval.d - x;
  }
  template<typename T> decltype(rval.d * T()) operator*(T x) const {
    return rval.d * x;
  }
  template<typename T> decltype(rval.d / T()) operator/(T x) const {
    return rval.d / x;
  }
  template<typename T> decltype(fmod(rval.d, T())) operator%(T x) const {
    return fmod(rval.d, x);
  }

  // Same as the above, but for another variant.
  double operator-(const variant &x) const { return rval.d - x.rval.d; }
  double operator*(const variant &x) const { return rval.d * x.rval.d; }
  double operator/(const variant &x) const { return rval.d / x.rval.d; }
  double operator%(const variant &x) const { return fmod(rval.d, x.rval.d); }

  // This is just fucking stupid.
  template<class T> variant &operator-=(const T &v)  { return *this = *this - v;  }
  template<class T> variant &operator*=(const T &v)  { return *this = *this * v;  }
  template<class T> variant &operator/=(const T &v)  { return *this = *this / v;  }
  template<class T> variant &operator%=(const T &v)  { return *this = *this % v;  }

  template<class T> variant &operator&=( const T &v) { return *this = *this & v;  }
  template<class T> variant &operator|=( const T &v) { return *this = *this | v;  }
  template<class T> variant &operator^=( const T &v) { return *this = *this ^ v;  }
  template<class T> variant &operator<<=(const T &v) { return *this = *this << v; }
  template<class T> variant &operator>>=(const T &v) { return *this = *this >> v; }

  // Comparison helpers.
  template<typename T> bool epsilon_eq(T x) const {
    return rval.d <= x + epsilon && rval.d >= x - epsilon;
  }
  template<typename T> bool epsilon_neq(T x) const {
    return rval.d < x - epsilon || rval.d > x + epsilon;
  }
  template<typename T> bool epsilon_leq(T x) const {
    return rval.d <= x + epsilon;
  }
  template<typename T> bool epsilon_geq(T x) const {
    return rval.d >= x - epsilon;
  }
  template<typename T> bool epsilon_lt(T x) const {
    return rval.d < x - epsilon;
  }
  template<typename T> bool epsilon_gt(T x) const {
    return rval.d > x + epsilon;
  }

  // Comparison operators:  <  >  <=  >=  !=  ==
  // ===========================================================================

  // Block one: Type-aware comparison with other variant.
  bool operator==(const variant &x) const RLY_INLINE {
    if (type != x.type) return false;
    if (type == ty_string) return sval == x.sval;
    return epsilon_eq(x.rval.d);
  }
  bool operator!=(const variant &x) const RLY_INLINE {
    if (type != x.type) return true;
    if (type == ty_string) return sval != x.sval;
    return epsilon_neq(x.rval.d);
  }
  bool operator<=(const variant &x) const RLY_INLINE {
    if (type == ty_string) {
      if (x.type != ty_string) return false;  // As a string, we are larger.
      return sval <= x.sval;
    }
    return epsilon_leq(x.rval.d);
  }
  bool operator>=(const variant &x) const RLY_INLINE {
    if (type == ty_string) {
      if (x.type != ty_string) return true;  // As a string, we are larger.
      return sval >= x.sval;
    }
    return epsilon_geq(x.rval.d);
  }
  bool operator<(const variant &x) const RLY_INLINE {
    if (type == ty_string) {
      if (x.type != ty_string) return false;  // As a string, we are larger.
      return sval < x.sval;
    }
    return epsilon_lt(x.rval.d);
  }
  bool operator>(const variant &x) const RLY_INLINE {
    if (type == ty_string) {
      if (x.type != ty_string) return true;  // As a string, we are larger.
      return sval > x.sval;
    }
    return epsilon_gt(x.rval.d);
  }

  // Block two: strictly numeric comparisons.
  // Note: Strings are always larger than reals.
  template<typename T> enigma::NSNumberFunc<T, bool> operator==(T x) const {
    return type == ty_real && epsilon_eq(x);
  }
  template<typename T> enigma::NSNumberFunc<T, bool> operator!=(T x) const {
    return type != ty_real || epsilon_neq(x);
  }
  template<typename T> enigma::NSNumberFunc<T, bool> operator<=(T x) const {
    return type == ty_real && epsilon_leq(x);
  }
  template<typename T> enigma::NSNumberFunc<T, bool> operator>=(T x) const {
    return type != ty_real || epsilon_geq(x);
  }
  template<typename T> enigma::NSNumberFunc<T, bool> operator<(T x) const {
    return type == ty_real && epsilon_lt(x);
  }
  template<typename T> enigma::NSNumberFunc<T, bool> operator>(T x) const {
    return type != ty_real || epsilon_gt(x);
  }

  // Block three: string comparisons.
  // As before, Strings are always larger than reals.
  template<typename T> enigma::StringFunc<T, bool> operator==(T x) const {
    return type == ty_string && sval == x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator!=(T x) const {
    return type != ty_string || sval != x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator<=(T x) const {
    return type != ty_string || sval <= x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator>=(T x) const {
    return type == ty_string && sval >= x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator<(T x) const {
    return type != ty_string || sval < x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator>(T x) const {
    return type == ty_string && sval > x;
  }

  // Bitwise operators:  <<  >>  &  |  ^
  // ===========================================================================

  template<typename T>
  decltype(0LL << (int)*(T*)nullptr) operator<<(T x) const {
    return (long long) rval.d << (int) x;
  }
  template<typename T>
  decltype(0LL >> (int)*(T*)nullptr) operator>>(T x) const {
    return (long long) rval.d >> (int) x;
  }
  template<typename T>
  decltype(0LL & (long long)*(T*)nullptr) operator&(T x) const {
    return (long long) rval.d & (long long) x;
  }
  template<typename T>
  decltype(0LL | (long long)*(T*)nullptr) operator|(T x) const {
    return (long long) rval.d | (long long) x;
  }
  template<typename T>
  decltype(0LL | (long long)*(T*)nullptr) operator^(T x) const {
    return (long long) rval.d ^ (long long) x;
  }

  // Miscellanea
  // ===========================================================================

  // Array accessors.
  char &operator[](int ind)       { return sval[ind]; }
  char  operator[](int ind) const { return sval[ind]; }

  // Increment operators.
  variant&  operator++()    { return ++rval.d, *this; }
  double    operator++(int) { return rval.d++; }
  variant&  operator--()    { return --rval.d, *this; }
  double    operator--(int) { return rval.d--; }
  variant&  operator*()     { return *this; }

  // Other unary operators.
  bool   operator!() const { return !bool(*this); }
  long   operator~() const { return ~long(*this); }
  double operator-() const { return -rval.d; }
  double operator+() const { return  rval.d; }

  ~variant() {}
};


//▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚
//██▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜█████████████████████████████████████████
//██▌ var: a sparse matrix of variant ▐█████████████████████████████████████████
//██▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟█████████████████████████████████████████
//▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞

struct var : variant {
  lua_table<variant> array1d;
  lua_table<lua_table<variant>> array2d;

  var() {}
  var(const var&) = default;
  var(variant value, size_t length, size_t height = 1):
      variant(value), array1d(value, length) {
    for (size_t i = 1; i < height; ++i) array2d[i].fill(value, length);
  }
  template<typename T> var(const T &v): variant(v) {}

  // Non-variant operators (matrix-related)
  // ===========================================================================

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

  // Other array functions
  // ===========================================================================

  // Calculate array lengths.
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

  // Annoying overhead and var extensions of variant operators
  // ===========================================================================

  // Inherit 75 operators from variant like it's 1982
  using variant::operator=;
  using variant::operator+;   using variant::operator+=;
  using variant::operator-;   using variant::operator-=;
  using variant::operator*;   using variant::operator*=;
  using variant::operator/;   using variant::operator/=;
  using variant::operator%;   using variant::operator%=;
  using variant::operator<;   using variant::operator<=;
  using variant::operator>;   using variant::operator>=;
  using variant::operator<<;  using variant::operator<<=;
  using variant::operator>>;  using variant::operator>>=;
  using variant::operator|;   using variant::operator|=;
  using variant::operator^;   using variant::operator^=;
  using variant::operator!;   using variant::operator!=;
  using variant::operator~;

  var &operator=(const var &v) {
    *(variant*) this = v;
    return *this;
  }

  variant operator+(const variant &v) {
    return *(variant*) this + v;
  }
  variant operator+(const var &v) {
    return *(variant*) this + v;
  }

  // auto operator-(const var &v)  { return *(variant *) this -  v; }
  // auto operator*(const var &v)  { return *(variant *) this *  v; }
  // auto operator/(const var &v)  { return *(variant *) this /  v; }
  // auto operator%(const var &v)  { return *(variant *) this %  v; }
  // auto operator<(const var &v)  { return *(variant *) this <  v; }
  // auto operator>(const var &v)  { return *(variant *) this >  v; }
  // auto operator<<(const var &v) { return *(variant *) this << v; }
  // auto operator>>(const var &v) { return *(variant *) this >> v; }
  // auto operator|(const var &v)  { return *(variant *) this |  v; }
  // auto operator^(const var &v)  { return *(variant *) this ^  v; }

  ~var() {}
};


//▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚
//██▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜████████████████████████████████████
//██▌ Reverse-direction operator overloads ▐████████████████████████████████████
//██▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟████████████████████████████████████
//▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞

template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline decltype(T() + double()) operator+(T a, const variant &b) {
  return a + b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline decltype(T() - double()) operator-(T a, const variant &b) {
  return a - b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline decltype(T() * double()) operator*(T a, const variant &b) {
  return a * b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline decltype(T() / double()) operator/(T a, const variant &b) {
  return a / b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
double operator%(T a, const variant &b) {
  return fmod(a, b.rval.d);
}

template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline long long operator<<(T a, const variant &b) {
  return (long long) a << (long long) b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline long long operator>>(T a, const variant &b) {
  return (long long) a >> (long long) b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline long long operator&(T a, const variant &b) {
  return (long long) a & (long long) b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline long long operator|(T a, const variant &b) {
  return (long long) a | (long long) b.rval.d;
}
template<typename T, typename enigma::NonStringNumberTypeEnabler<T>::EN = 0>
static inline long long operator^(T a, const variant &b) {
  return (long long) a ^ (long long) b.rval.d;
}

// We need to disallow these operators for variants or Clang will decide
// that the global version is equally preferable with the FULLY-SPECIFIED
// match in the actual variant class.
#define PRIMITIVE_OP \
template<class T, typename enigma::NonVariantTypeEnabler<T>::EN = 0> \
static inline

PRIMITIVE_OP bool operator==(const T &a, const variant &b) { return b == a; }
PRIMITIVE_OP bool operator!=(const T &a, const variant &b) { return b != a; }
PRIMITIVE_OP bool operator<=(const T &a, const variant &b) { return b >= a; }
PRIMITIVE_OP bool operator>=(const T &a, const variant &b) { return b <= a; }
PRIMITIVE_OP bool operator< (const T &a, const variant &b) { return b > a; }
PRIMITIVE_OP bool operator> (const T &a, const variant &b) { return b < a; }

PRIMITIVE_OP T &operator+= (T &a, const variant &b) { return a +=  (T) b; }
PRIMITIVE_OP T &operator-= (T &a, const variant &b) { return a -=  (T) b; }
PRIMITIVE_OP T &operator*= (T &a, const variant &b) { return a *=  (T) b; }
PRIMITIVE_OP T &operator/= (T &a, const variant &b) { return a /=  (T) b; }
PRIMITIVE_OP T &operator%= (T &a, const variant &b) { return a %=  (T) b; }
PRIMITIVE_OP T &operator&= (T &a, const variant &b) { return a &=  (T) b; }
PRIMITIVE_OP T &operator|= (T &a, const variant &b) { return a |=  (T) b; }
PRIMITIVE_OP T &operator^= (T &a, const variant &b) { return a ^=  (T) b; }
PRIMITIVE_OP T &operator<<=(T &a, const variant &b) { return a <<= (T) b; }
PRIMITIVE_OP T &operator>>=(T &a, const variant &b) { return a >>= (T) b; }

// String + variant operator we missed above
template<typename T, typename enigma::StringTypeEnabler<T>::EN = 0> static
inline std::string operator+(const T &str, const variant &v) {
  return str + v.to_string();
}

namespace enigma_user {

using ::var;
using ::variant;

static inline bool is_undefined(const variant &val) {
  return val.type == enigma_user::ty_undefined;
}
static inline bool is_real     (const variant &val) {
  return val.type == enigma_user::ty_real;
}
static inline bool is_string   (const variant &val) {
  return val.type == enigma_user::ty_string;
}
static inline bool is_ptr      (const variant &val) {
  return val.type == enigma_user::ty_pointer;
}

}  // namespace enigma_user


// Some compilers favor the above variant casts for enums,
// rather than just comparing them as integers.
#define ENUM_OPERATION template<typename T, typename U, typename Common =      \
    typename enigma::EnumAndNumericBinaryFuncEnabler<T, U>::T>                 \
    static inline constexpr

ENUM_OPERATION bool operator==(T a, U b) { return (Common) a == (Common) b; }
ENUM_OPERATION bool operator!=(T a, U b) { return (Common) a != (Common) b; }
ENUM_OPERATION bool operator<=(T a, U b) { return (Common) a <= (Common) b; }
ENUM_OPERATION bool operator>=(T a, U b) { return (Common) a >= (Common) b; }
ENUM_OPERATION bool operator<(T a, U b)  { return (Common) a <  (Common) b; }
ENUM_OPERATION bool operator>(T a, U b)  { return (Common) a >  (Common) b; }

ENUM_OPERATION Common operator&(T a, U b) { return (Common) a & (Common) b; }
ENUM_OPERATION Common operator|(T a, U b) { return (Common) a | (Common) b; }
ENUM_OPERATION Common operator^(T a, U b) { return (Common) a ^ (Common) b; }


// End of reverse-order operations.

namespace std {

template<> class numeric_limits<var>: numeric_limits<double> {};
template<> class numeric_limits<variant>: numeric_limits<double> {};
template<> class numeric_limits<const var&>: numeric_limits<double> {};
template<> class numeric_limits<const variant&>: numeric_limits<double> {};

}  // namespace std


namespace enigma {

// Define var and variant as arithmetic types.
// Note: This isn't some profound or magical truth; it's just the distinction
//       between ArithmeticType and NumericType, as we've defined them.
template<> struct ArithmeticTypeEnabler<const var&>
    : EnabledType<const var&>     {};
template<> struct ArithmeticTypeEnabler<const variant&>
    : EnabledType<const variant&> {};

}  // namespace enigma


#endif //ENIGMA_VAR4_H
