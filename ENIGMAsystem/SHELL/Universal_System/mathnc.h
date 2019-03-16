/** Copyright (C) 2008-2012 Josh Ventura
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

#ifndef ENIGMA_MATHNC
#define ENIGMA_MATHNC

//Before anyone else includes math.h, let's make sure we
//Don't get bitten by those damn bessel functions.
#define j0 bessel_j0
  #define j1 bessel_j1
  #define jn bessel_jn
  #define y0 bessel_y0
  #define y1 bessel_y1
  #define yn bessel_yn
    #include <cmath> // Basic math functions
  #undef j1
  #undef j0
  #undef jn
  #undef y1
  #undef y0
#undef yn

#include "dynamic_args.h"
#include "generic_args.h"

#ifndef INCLUDED_FROM_SHELLMAIN
#error ln2math and stop including this damn header.
#endif

#include "math_consts.h"
#include "scalar.h"
#include "var4.h"

#include <string>

#ifdef JUST_DEFINE_IT_RUN
#  define WITH_ARITHMETIC_TYPES(...)
#  define WITH_RETURN_TYPE(type) type
#  define ARITHMETIC_OPERATION(guess_type, ...) guess_type
#else
#  define WITH_ARITHMETIC_TYPES(...) typename enigma::ArithmeticTypes<__VA_ARGS__>
#  define WITH_RETURN_TYPE(type) ::template returns<type>::T
#  define ARITHMETIC_OPERATION(guess_type, ...) decltype(__VA_ARGS__)
#endif
#define STL_ARITHMETIC(func)                                                  \
  using ::func;                                                               \
  static inline double func(const variant &v) { return ::func((double) v); }  \
  static inline double func(const var &v)     { return ::func((double) v); }

namespace enigma_user
{
  const ma_scalar pi = M_PI;

  // STL functions.
  STL_ARITHMETIC(ceil);
  STL_ARITHMETIC(floor);
  STL_ARITHMETIC(round);
  STL_ARITHMETIC(exp);
  STL_ARITHMETIC(sqrt);
  STL_ARITHMETIC(log);
  STL_ARITHMETIC(log2);
  STL_ARITHMETIC(log10);
  STL_ARITHMETIC(sin);
  STL_ARITHMETIC(cos);
  STL_ARITHMETIC(tan);

  // Overloading - These functions have special implementations
  // in C++ for each supported type.

  // abs(x)
  template<typename T> enigma::SIntType<T>  abs(T x) { return ::abs(x); }
  template<typename T> enigma::UIntType<T>  abs(T x) { return x; }
  template<typename T> enigma::FloatType<T> abs(T x) { return ::fabs(x); }
  static inline double abs(const variant& x) { return ::fabs(double(x)); }
  static inline double abs(const var& x)     { return ::fabs(double(x)); }

  // Aliases to STL functions.
  inline ma_scalar ln(double x)     { return ::log(x); }
  inline ma_scalar arcsin(double x) { return ::asin(x); }
  inline ma_scalar arccos(double x) { return ::acos(x); }
  inline ma_scalar arctan(double x) { return ::atan(x); }
  inline ma_scalar arctan2(double y, double x) { return ::atan2(y,x); }
  inline ma_scalar power(double x, double p)   { return ::pow(x,p); }

  template<typename T> WITH_ARITHMETIC_TYPES(T) WITH_RETURN_TYPE(int64_t)
  inline int64(T x) { return (int64_t) x; }
  inline int64_t int64(const char *x) { return (int64_t) atol(x); }
  inline int64_t int64(std::string x) { return (int64_t) atol(x.c_str()); }
  inline int64_t int64(const variant& x) {
    return x.type == ty_string ? int64((std::string) x) : int64((double) x);
  }
  inline int64_t int64(const var& x) { return int64(*x); }

  template<typename T>
  inline ARITHMETIC_OPERATION(T, T() * T()) sqr(T x) { return x * x; }
  inline double logn(double n, double x) { return ::log(x)/::log(n); }

  inline double sind(double x)    { return ::sin(x   * M_PI / 180.0); }
  inline double cosd(double x)    { return ::cos(x   * M_PI / 180.0); }
  inline double tand(double x)    { return ::tan(x   * M_PI / 180.0); }
  inline double asind(double x)   { return ::asin(x) * 180.0 / M_PI; }
  inline double acosd(double x)   { return ::acos(x) * 180.0 / M_PI; }
  inline double atand(double x)   { return ::atan(x) * 180.0 / M_PI; }
  inline double arcsind(double x) { return ::asin(x) * 180.0 / M_PI; }
  inline double arccosd(double x) { return ::acos(x) * 180.0 / M_PI; }
  inline double arctand(double x) { return ::atan(x) * 180.0 / M_PI; }
  inline double atand2(double y, double x)   { return ::atan2(y,x) * 180.0 / M_PI; }
  inline double arctand2(double y, double x) { return ::atan2(y,x) * 180.0 / M_PI; }

  // Non-variadic (two-item) min/max
  template<typename T, typename U> inline decltype(T() + U()) min(T x, U y) {
    return y < x ? y : x;
  }
  template<typename T, typename U> inline decltype(T() + U()) max(T x, U y) {
    return y > x ? y : x;
  }

  // Comparison / epsilon-equality
  /// C-like comparator; returns sign of x - y. Uses operator<().
  template<typename T, typename U>
  WITH_ARITHMETIC_TYPES(T, U) WITH_RETURN_TYPE(int) cmp(T x, U y) {
    return (y < x) - (x < y);
  }
  /// Return the sign of the input; one of { -1, 0, 1 }.
  template<typename T>
  WITH_ARITHMETIC_TYPES(T) WITH_RETURN_TYPE(int) sign(T x) {
    return cmp(x, 0);
  }
  /// Compare for equality using an epsilon.
  template<typename T, typename U>
  WITH_ARITHMETIC_TYPES(T, U) WITH_RETURN_TYPE(bool) equal(T x, U y) {
    return ::fabs(x - y) <= (std::numeric_limits<T>::epsilon()
                                * max(T(1), ::fmax(abs(x), abs(y))));
  }

  // ENIGMA's own math helpers
  inline ma_scalar frac(ma_scalar x)     { return x - (ma_scalar) floor(x); }
  inline ma_scalar degtorad(ma_scalar x) { return x * (pi / (ma_scalar) 180); }
  inline ma_scalar radtodeg(ma_scalar x) { return x * ((ma_scalar) 180 / pi); }

  inline double lengthdir_x(double len, double dir) { return len *  cosd(dir); }
  inline double lengthdir_y(double len, double dir) { return len * -sind(dir); }
  inline double angle_difference(double dir1, double dir2) {
    return fmod(fmod(dir1 - dir2, 360.0) + 540.0, 360.0) - 180.0;
  }

  template<typename X1, typename Y1, typename X2, typename Y2>
  WITH_ARITHMETIC_TYPES(X1, Y1, X2, Y2) WITH_RETURN_TYPE(double)
  point_direction(X1 x1, Y1 y1, X2 x2, Y2 y2) {
    return fmod((::atan2(y1 - y2, x2 - x1) * (180 / M_PI)) + 360, 360);
  }
  template<typename X1, typename Y1, typename X2, typename Y2>
  WITH_ARITHMETIC_TYPES(X1, Y1, X2, Y2) WITH_RETURN_TYPE(double)
  point_distance(X1 x1, Y1 y1, X2 x2, Y2 y2) {
    return ::hypot(x2 - x1, y2 - y1);
  }
  template<typename X1, typename Y1, typename Z1, typename X2, typename Y2, typename Z2>
  WITH_ARITHMETIC_TYPES(X1, Y1, Z1, X2, Y2, Z2) WITH_RETURN_TYPE(double)
  point_distance_3d(X1 x1, Y1 y1, Z1 z1, X2 x2, Y2 y2, Z2 z2) {
    return ::sqrt(sqr(x1 - x2) + sqr(y1 - y2) + sqr(z1 - z2));
  }

  // NOTE: These are technically collision functions in Studio
  ma_scalar triangle_area(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3);
  bool point_in_circle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar rad);
  bool point_in_rectangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2);
  bool point_in_triangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3);

  ma_scalar lines_intersect(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3, ma_scalar x4, ma_scalar y4, bool segment);
  int rectangle_in_circle(ma_scalar x1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x, ma_scalar y, ma_scalar rad);
  int rectangle_in_rectangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2);
  int rectangle_in_triangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2, ma_scalar dx3, ma_scalar dy3);

  int ray_sphere_intersect(ma_scalar xc, ma_scalar yc, ma_scalar zc, ma_scalar xs, ma_scalar ys, ma_scalar zs, ma_scalar xd, ma_scalar yd, ma_scalar zd, ma_scalar r);

  ma_scalar dot_product(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2);
  ma_scalar dot_product_3d(ma_scalar x1, ma_scalar y1, ma_scalar z1, ma_scalar x2,
  ma_scalar y2, ma_scalar z2);
  ma_scalar dot_product_normalised(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2);
  ma_scalar dot_product_normalised_3d(ma_scalar x1, ma_scalar y1, ma_scalar z1, ma_scalar x2,
  ma_scalar y2, ma_scalar z2);
  ma_scalar lerp(ma_scalar x, ma_scalar y, ma_scalar a);
  ma_scalar clamp(ma_scalar val, ma_scalar min, ma_scalar max);

  ma_scalar max(const enigma::varargs &t);
  ma_scalar min(const enigma::varargs &t);
  ma_scalar min(ma_scalar x, ma_scalar y);
  ma_scalar max(ma_scalar x, ma_scalar y);
  ma_scalar median(enigma::varargs t);
  ma_scalar mean(const enigma::varargs &t);
  variant choose(const enigma::varargs& args);
}

#undef WITH_ARITHMETIC_TYPES
#undef WITH_RETURN_TYPE
#undef ARITHMETIC_OPERATION

#endif // ENIGMA_MATHNC
