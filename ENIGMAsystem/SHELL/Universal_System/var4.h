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
#  define rvalue_ref &
#else
#  include <cstdint>
#  include <type_traits>
#  define rvalue_ref &&
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
  template<typename I> struct Kernel<I, decltype((U) std::declval<I>())> {
    static constexpr bool V = true;
  };

 public:
  static constexpr bool V = Kernel<T>::V;
};

#define ACCEPT_TYPE(K, T) template<> struct K<T> : EnabledType<T> {}

template<typename T> struct SIntTypeEnabler {};
ACCEPT_TYPE(SIntTypeEnabler, signed char);
ACCEPT_TYPE(SIntTypeEnabler, short int);
ACCEPT_TYPE(SIntTypeEnabler, int);
ACCEPT_TYPE(SIntTypeEnabler, long int);
ACCEPT_TYPE(SIntTypeEnabler, long long int);

template<typename T> struct UIntTypeEnabler {};
ACCEPT_TYPE(UIntTypeEnabler, unsigned char);
ACCEPT_TYPE(UIntTypeEnabler, unsigned short int);
ACCEPT_TYPE(UIntTypeEnabler, unsigned int);
ACCEPT_TYPE(UIntTypeEnabler, unsigned long int);
ACCEPT_TYPE(UIntTypeEnabler, unsigned long long int);

template<typename T> struct IntTypeEnabler {};
ACCEPT_TYPE(IntTypeEnabler, signed char);
ACCEPT_TYPE(IntTypeEnabler, short int);
ACCEPT_TYPE(IntTypeEnabler, int);
ACCEPT_TYPE(IntTypeEnabler, long int);
ACCEPT_TYPE(IntTypeEnabler, long long int);
ACCEPT_TYPE(IntTypeEnabler, unsigned char);
ACCEPT_TYPE(IntTypeEnabler, unsigned short int);
ACCEPT_TYPE(IntTypeEnabler, unsigned int);
ACCEPT_TYPE(IntTypeEnabler, unsigned long int);
ACCEPT_TYPE(IntTypeEnabler, unsigned long long int);


template<typename T> struct FloatTypeEnabler {};
ACCEPT_TYPE(FloatTypeEnabler, float);
ACCEPT_TYPE(FloatTypeEnabler, double);
ACCEPT_TYPE(FloatTypeEnabler, long double);

// Any explicitly numeric type. Note that this does NOT include bool and enum!
template<typename T> struct NumericTypeEnabler {};
ACCEPT_TYPE(NumericTypeEnabler, char);
ACCEPT_TYPE(NumericTypeEnabler, signed char);
ACCEPT_TYPE(NumericTypeEnabler, short int);
ACCEPT_TYPE(NumericTypeEnabler, int);
ACCEPT_TYPE(NumericTypeEnabler, long int);
ACCEPT_TYPE(NumericTypeEnabler, long long int);
ACCEPT_TYPE(NumericTypeEnabler, unsigned char);
ACCEPT_TYPE(NumericTypeEnabler, unsigned short int);
ACCEPT_TYPE(NumericTypeEnabler, unsigned int);
ACCEPT_TYPE(NumericTypeEnabler, unsigned long int);
ACCEPT_TYPE(NumericTypeEnabler, unsigned long long int);
ACCEPT_TYPE(NumericTypeEnabler, float);
ACCEPT_TYPE(NumericTypeEnabler, double);
ACCEPT_TYPE(NumericTypeEnabler, long double);
template<typename T> struct ArithmeticTypeEnabler {};
ACCEPT_TYPE(ArithmeticTypeEnabler, signed char);
ACCEPT_TYPE(ArithmeticTypeEnabler, short int);
ACCEPT_TYPE(ArithmeticTypeEnabler, int);
ACCEPT_TYPE(ArithmeticTypeEnabler, long int);
ACCEPT_TYPE(ArithmeticTypeEnabler, unsigned char);
ACCEPT_TYPE(ArithmeticTypeEnabler, unsigned short int);
ACCEPT_TYPE(ArithmeticTypeEnabler, unsigned int);
ACCEPT_TYPE(ArithmeticTypeEnabler, unsigned long int);
ACCEPT_TYPE(ArithmeticTypeEnabler, float);
ACCEPT_TYPE(ArithmeticTypeEnabler, double);
ACCEPT_TYPE(ArithmeticTypeEnabler, long double);

// More lax than NumericType, but does not permit any type that can also be cast
// to a string. Allows booleans and enum constants, but not vars or variants.
template<typename T> struct NonStringNumberTypeEnabler
    : MaybeEnabled<T, CanCast<T, double>::V && !CanCast<T, std::string>::V> {};

template<typename T> struct NonVariantTypeEnabler
    : MaybeEnabled<T, CanCast<T, double>::V != CanCast<T, std::string>::V> {};

template<typename T, typename U> struct NeqEnabler : EnabledType<T> {};
template<typename T> struct NeqEnabler<T, T> {};

template<typename X, typename Y = X, typename Z = Y,
         typename W = Z, typename P = W, typename R = P,
         typename EN = decltype(+X() - +Y() - +Z() - +W() - +P() - +R())>
struct ArithmeticTypes: EnabledType<EN> {};

template<typename T> struct StringTypeEnabler {};
ACCEPT_TYPE(StringTypeEnabler, std::string);
ACCEPT_TYPE(StringTypeEnabler, std::string&);
ACCEPT_TYPE(StringTypeEnabler, const std::string&);
ACCEPT_TYPE(StringTypeEnabler, std::string&&);
ACCEPT_TYPE(StringTypeEnabler, const char*);
// TODO: support string_view

#undef ACCEPT_TYPE

#ifndef JUST_DEFINE_IT_RUN

#define RLY_INLINE __attribute__((always_inline))

template<typename T, typename U, typename V = decltype(+*(T*)0 | +*(U*)0)>
struct EnumAndNumericBinaryFuncEnabler
    : MaybeEnabled<V, std::is_enum<T>::value || std::is_enum<U>::value> {};

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

#define REQUIRE_NON_STRING_NUMBER(T) \
    typename enigma::NonStringNumberTypeEnabler<T>::EN = 0
#define REQUIRE_STRING_TYPE(T)\
    typename enigma::StringTypeEnabler<T>::EN = 0
#define REQUIRE_NON_VARIANT_TYPE(T) \
    typename enigma::NonVariantTypeEnabler<T>::EN = 0
#define REQUIRE_VARIANT_TYPE(T) \
    typename enigma::VariantTypeEnabler<T>::EN = 0

#else

template<typename T> struct SIntType {};
template<typename T> struct UIntType {};
template<typename T> struct IntType  {};
template<typename T> struct FloatType   {};
template<typename T> struct NumericType {};
template<typename T> struct ArithmeticType {};
template<typename T> struct StringType  {};

#define REQUIRE_NON_STRING_NUMBER(T) bool non_string_number = true
#define REQUIRE_STRING_TYPE(T)       bool is_string_type = true
#define REQUIRE_NON_VARIANT_TYPE(T)  bool is_not_variant_type = true
#define REQUIRE_VARIANT_TYPE(T)      bool is_variant_type = true

template<typename T, typename U, typename V = decltype(+*(T*)0 | +*(U*)0)>
struct EnumAndNumericBinaryFuncEnabler {};

template<typename T, typename U> struct NumericFunc {};
template<typename T, typename U> struct NSNumberFunc {};
template<typename T, typename U> struct StringFunc {};

#define RLY_INLINE
#endif  // JUST_DEFINE_IT_RUN

union rvt {
  double d;
  const void * p;
  rvt(double x): d(x) {}
  rvt(const void *x): p(x) {}
};

struct variant_real_union {
  enigma::rvt rval;
  variant_real_union(double x): rval(x) {}
  variant_real_union(const void *x): rval(x) {}
};
struct variant_string_wrapper : std::string {
  std::string &sval() { return *this; }
  const std::string &sval() const { return *this; }
  variant_string_wrapper() {}
  variant_string_wrapper(std::string const      &x): std::string(x) {}
  variant_string_wrapper(std::string rvalue_ref  x): std::string(x) {}
  std::string rvalue_ref release_sval() {
    return std::move(*(std::string*) this);
  }
};

}  // namespace enigma

struct var;
struct variant;

namespace enigma {

template<typename T> struct VariantTypeEnabler
    : MaybeEnabled<T, std::is_base_of<variant, T>::value> {};
template<> struct VariantTypeEnabler<variant> : EnabledType<variant> {};
template<> struct VariantTypeEnabler<var> : EnabledType<var> {};

}

namespace enigma_user {

enum {
  ty_undefined = -1,
  ty_real = 0,
  ty_string = 1,
  ty_pointer = 2
};

std::string toString(const void*);
std::string toString(double);

#if defined(INCLUDED_FROM_SHELLMAIN) && !defined(JUST_DEFINE_IT_RUN)
#define string(...) toString(__VA_ARGS__)
#endif

using std::string;

}

//▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚
//██▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜██████████████████████████████
//██▌ variant: a smart pair of string and double ▐██████████████████████████████
//██▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟██████████████████████████████
//▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞

struct variant : enigma::variant_real_union, enigma::variant_string_wrapper {
  int type;  ///< Union tag.

  // Default type (-1 or real), changes based on "assume_uninitialized_is_zero."
  // (This variable is defined in a compiler-generated class.)
  static const int default_type;

  static constexpr double epsilon = 1e-12;
  static constexpr int ty_real   = enigma_user::ty_real;
  static constexpr int ty_string = enigma_user::ty_string;

  // Real-valued casts.
  template<typename T, REQUIRE_NON_STRING_NUMBER(T)>
  operator T() const {
    return (T) rval.d;
  }

  size_t string_length()   const { return sval().length(); }
  char char_at(size_t ind) const { return sval()[ind]; }

  std::string to_string() const {
    if (type == ty_string)  return sval();
    if (type == enigma_user::ty_pointer) return enigma_user::toString(rval.p);
    if (type == ty_real) return enigma_user::toString(rval.d);
    return "<undefined>";
  }

  // Our bool cast is special... thanks, Delphi
  operator bool() const {
    // TODO: Enable turning this off...
    return lrint(rval.d) > 0;
  }

  // Char casting must be explicit, or else string construction is ambiguous.
  // Also, this is a good place for special logic.
  explicit operator char() const {
    if (type == ty_string) return sval()[0];
    return (char) rval.d;
  }

  // How to construct a variant: the basics
  variant():
      enigma::variant_real_union(0.0), type(default_type) {}
  variant(const void *p):
      enigma::variant_real_union(p), type(enigma_user::ty_pointer) {}
  variant(const variant &x):
      enigma::variant_real_union(x.rval.d),
      enigma::variant_string_wrapper(x.sval()),
      type(x.type) {}
  variant(variant rvalue_ref x):
      enigma::variant_real_union(x.rval.d),
      enigma::variant_string_wrapper(x.release_sval()),
      type(x.type) {}

  // Construct a variant from numeric types
  template<typename T, REQUIRE_NON_STRING_NUMBER(T)>
  variant(T number): enigma::variant_real_union((double) number), type(ty_real) {}

  variant(const char *str):
      enigma::variant_real_union(0.),
      enigma::variant_string_wrapper(str),
      type(ty_string) {}
  variant(const std::string &str):
      enigma::variant_real_union(0.),
      enigma::variant_string_wrapper(str),
      type(ty_string) {}
  variant(std::string rvalue_ref str):
      enigma::variant_real_union(0.),
      enigma::variant_string_wrapper(str),
      type(ty_string) {}

  // Assignment operators
  // ===========================================================================

  variant& operator=(const variant &v) {
    rval = v.rval;
    if ((type = v.type) == ty_string) sval() = v.sval();
    return *this;
  }

  // Assignment to a numeric type
  template<typename T, REQUIRE_NON_STRING_NUMBER(T)>
  variant& operator=(T number) {
    rval.d = (double) number;
    type = ty_real;
    return *this;
  }

  // Assignment to a string type
  variant& operator=(const std::string &str) {
    sval() = str;
    type = ty_string;
    return *this;
  }
  variant& operator=(std::string rvalue_ref str) {
    sval() = std::move(str);
    type = ty_string;
    return *this;
  }

  variant& operator=(const char *str) {
    sval() = str;
    type = ty_string;
    return *this;
  }
  // TODO: string_view construction/assignment


  // The plus operator is actually really special.
  template<typename T, REQUIRE_NON_STRING_NUMBER(T)>
  variant& operator+=(T number) {
    rval.d += number;
    return *this;
  }
  template<typename T, REQUIRE_STRING_TYPE(T)>
  variant& operator+=(T str) {
    sval() += str;
    return *this;
  }
  template<class T, REQUIRE_VARIANT_TYPE(T)>
  RLY_INLINE variant& operator+=(const T &other) {
    if (type == ty_string) sval() += other.sval();
    else rval.d += other.rval.d;
    return *this;
  }

  template<typename T, REQUIRE_VARIANT_TYPE(T)>
  RLY_INLINE variant operator+(const T &other) const {
    if (type == ty_string) return sval() + other.sval();
    return rval.d + other.rval.d;
  }
  template<typename T, REQUIRE_STRING_TYPE(T)>
  RLY_INLINE std::string operator+(T str) const {
    return to_string() + str;
  }

  // Arithmetic operators.
  // ===========================================================================

# define VAROP    template<typename T, REQUIRE_VARIANT_TYPE(T)>
# define NONVAROP template<typename T, REQUIRE_NON_VARIANT_TYPE(T)>
# define ANYOP    template<typename T>
  // Basic arithmetic operators.
  NONVAROP decltype(rval.d + T()) operator+(T x) const { return rval.d + x; }
  NONVAROP decltype(rval.d - T()) operator-(T x) const { return rval.d - x; }
  NONVAROP decltype(rval.d * T()) operator*(T x) const { return rval.d * x; }
  NONVAROP decltype(rval.d / T()) operator/(T x) const { return rval.d / x; }
  NONVAROP decltype(fmod(rval.d, T())) operator%(T x) const {
    return fmod(rval.d, x);
  }

  // Same as the above, but for another variant.
  VAROP double operator-(const T &x) const { return rval.d - x.rval.d; }
  VAROP double operator*(const T &x) const { return rval.d * x.rval.d; }
  VAROP double operator/(const T &x) const { return rval.d / x.rval.d; }
  VAROP double operator%(const T &x) const { return fmod(rval.d, x.rval.d); }

  // This is just fucking stupid.
  ANYOP variant &operator-=(const T &v)  { return *this = *this - v;  }
  ANYOP variant &operator*=(const T &v)  { return *this = *this * v;  }
  ANYOP variant &operator/=(const T &v)  { return *this = *this / v;  }
  ANYOP variant &operator%=(const T &v)  { return *this = *this % v;  }

  ANYOP variant &operator&=( const T &v) { return *this = *this & v;  }
  ANYOP variant &operator|=( const T &v) { return *this = *this | v;  }
  ANYOP variant &operator^=( const T &v) { return *this = *this ^ v;  }
  ANYOP variant &operator<<=(const T &v) { return *this = *this << v; }
  ANYOP variant &operator>>=(const T &v) { return *this = *this >> v; }

# undef NONVAROP
# undef VAROP
# undef ANYOP

  // Comparison helpers.
  // ===========================================================================

  template<typename T> bool epsilon_eq(T x) const {
    return rval.d - epsilon <= x && rval.d + epsilon >= x;
  }
  template<typename T> bool epsilon_neq(T x) const {
    return rval.d + epsilon < x || rval.d - epsilon > x;
  }
  template<typename T> bool epsilon_leq(T x) const {
    return rval.d - epsilon <= x;
  }
  template<typename T> bool epsilon_geq(T x) const {
    return rval.d + epsilon >= x;
  }
  template<typename T> bool epsilon_lt(T x) const {
    return rval.d + epsilon < x;
  }
  template<typename T> bool epsilon_gt(T x) const {
    return rval.d - epsilon > x;
  }

  // Comparison operators:  <  >  <=  >=  !=  ==
  // ===========================================================================

  // Block one: Type-aware comparison with other variant.
# define VAROP template<typename T, REQUIRE_VARIANT_TYPE(T)> bool RLY_INLINE
  VAROP operator==(const T &x) const {
    if (type != x.type) return false;
    if (type == ty_string) return sval() == x.sval();
    return epsilon_eq(x.rval.d);
  }
  VAROP operator!=(const T &x) const {
    if (type != x.type) return true;
    if (type == ty_string) return sval() != x.sval();
    return epsilon_neq(x.rval.d);
  }
  VAROP operator<=(const T &x) const {
    if (type == ty_string) {
      if (x.type != ty_string) return false;  // As a string, we are larger.
      return sval() <= x.sval();
    }
    return epsilon_leq(x.rval.d);
  }
  VAROP operator>=(const T &x) const {
    if (type == ty_string) {
      if (x.type != ty_string) return true;  // As a string, we are larger.
      return sval() >= x.sval();
    }
    return epsilon_geq(x.rval.d);
  }
  VAROP operator<(const T &x) const {
    if (type == ty_string) {
      if (x.type != ty_string) return false;  // As a string, we are larger.
      return sval() < x.sval();
    }
    return epsilon_lt(x.rval.d);
  }
  VAROP operator>(const T &x) const {
    if (type == ty_string) {
      if (x.type != ty_string) return true;  // As a string, we are larger.
      return sval() > x.sval();
    }
    return epsilon_gt(x.rval.d);
  }
#undef VAROP

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
    return type == ty_string && sval() == x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator!=(T x) const {
    return type != ty_string || sval() != x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator<=(T x) const {
    return type != ty_string || sval() <= x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator>=(T x) const {
    return type == ty_string && sval() >= x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator<(T x) const {
    return type != ty_string || sval() < x;
  }
  template<typename T> enigma::StringFunc<T, bool> operator>(T x) const {
    return type == ty_string && sval() > x;
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
  char &operator[](int ind)       { return sval()[ind]; }
  char  operator[](int ind) const { return sval()[ind]; }

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
  using variant::operator=;   using variant::operator==;
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

  #ifndef JUST_DEFINE_IT_RUN // These confuse JDI for some reason
  var &operator=(const var &v) {
    *(variant*) this = v;
    return *this;
  }

  template<typename T, REQUIRE_VARIANT_TYPE(T)>
  variant operator+(const T &v) {
    return *(variant*) this + v;
  }
  #endif

  ~var() {}
};


//▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚
//██▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜████████████████████████████████████
//██▌ Reverse-direction operator overloads ▐████████████████████████████████████
//██▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟████████████████████████████████████
//▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞▚▞

#define VARBINOP template<typename T, typename U,                              \
    REQUIRE_NON_STRING_NUMBER(T), REQUIRE_VARIANT_TYPE(U)>                     \
    static inline

VARBINOP decltype(+T() + double()) operator+(T a, const U &b) {
  return a + b.rval.d;
}
VARBINOP decltype(+T() - double()) operator-(T a, const U &b) {
  return a - b.rval.d;
}
VARBINOP decltype(+T() * double()) operator*(T a, const U &b) {
  return a * b.rval.d;
}
VARBINOP decltype(+T() / double()) operator/(T a, const U &b) {
  return a / b.rval.d;
}
VARBINOP double operator%(T a, const U &b) {
  return fmod(a, b.rval.d);
}

VARBINOP long long operator<<(T a, const U &b) {
  return (long long) a << (long long) b.rval.d;
}
VARBINOP long long operator>>(T a, const U &b) {
  return (long long) a >> (long long) b.rval.d;
}
VARBINOP long long operator&(T a, const U &b) {
  return (long long) a & (long long) b.rval.d;
}
VARBINOP long long operator|(T a, const U &b) {
  return (long long) a | (long long) b.rval.d;
}
VARBINOP long long operator^(T a, const U &b) {
  return (long long) a ^ (long long) b.rval.d;
}

#undef VARBINOP

// We need to disallow these operators for variants or Clang will decide
// that the global version is equally preferable with the FULLY-SPECIFIED
// match in the actual variant class.
#ifndef JUST_DEFINE_IT_RUN
#define PRIMITIVE_OP                                                           \
    template<class T, typename U,                                              \
        REQUIRE_NON_VARIANT_TYPE(T), REQUIRE_VARIANT_TYPE(U)>                  \
        static inline
#else
#define PRIMITIVE_OP template<typename T, typename U>
#endif

PRIMITIVE_OP bool operator==(const T &a, const U &b) { return b == a; }
PRIMITIVE_OP bool operator!=(const T &a, const U &b) { return b != a; }
PRIMITIVE_OP bool operator<=(const T &a, const U &b) { return b >= a; }
PRIMITIVE_OP bool operator>=(const T &a, const U &b) { return b <= a; }
PRIMITIVE_OP bool operator< (const T &a, const U &b) { return b > a; }
PRIMITIVE_OP bool operator> (const T &a, const U &b) { return b < a; }

PRIMITIVE_OP T &operator+= (T &a, const U &b) { return a +=  (T) b; }
PRIMITIVE_OP T &operator-= (T &a, const U &b) { return a -=  (T) b; }
PRIMITIVE_OP T &operator*= (T &a, const U &b) { return a *=  (T) b; }
PRIMITIVE_OP T &operator/= (T &a, const U &b) { return a /=  (T) b; }
PRIMITIVE_OP T &operator%= (T &a, const U &b) { return a %=  (T) b; }
PRIMITIVE_OP T &operator&= (T &a, const U &b) { return a &=  (T) b; }
PRIMITIVE_OP T &operator|= (T &a, const U &b) { return a |=  (T) b; }
PRIMITIVE_OP T &operator^= (T &a, const U &b) { return a ^=  (T) b; }
PRIMITIVE_OP T &operator<<=(T &a, const U &b) { return a <<= (T) b; }
PRIMITIVE_OP T &operator>>=(T &a, const U &b) { return a >>= (T) b; }

#undef PRIMITIVE_OP

// String + variant operator we missed above
template<typename T, typename U, REQUIRE_STRING_TYPE(T), REQUIRE_VARIANT_TYPE(U)>
static inline std::string operator+(T str, const U &v) {
  return str + v.to_string();
}

namespace enigma_user {

using ::var;
using ::variant;
typedef std::string std_string;

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
