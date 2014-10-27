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

#ifndef INCLUDED_FROM_SHELLMAIN
#error ln2math and stop including this damn header.
#endif

#include <cstdlib> // random()
#include "scalar.h"

#define M_PI		3.14159265358979323846

namespace enigma_user
{
  const ma_scalar pi = M_PI;

  // Overloading
  // TODO: Once the user space switch to namespace enigma_user has been made,
  // remove these functions.
  ma_scalar abs(const variant& x);
  ma_scalar abs(const var& x);

  // Functions

  // TODO: Once the user space switch to namespace enigma_user has been made,
  // comment in all these functions.
  inline ma_scalar abs(double x)   { return ::fabs(x); }
  //inline ma_scalar ceil(ma_scalar x)  { return ::ceil(x); }
  //inline ma_scalar floor(ma_scalar x) { return ::floor(x); }
  //inline ma_scalar round(ma_scalar x) { return ::round(x); }
  //inline ma_scalar exp(ma_scalar x)   { return ::exp(x); }
  //inline ma_scalar sqrt(ma_scalar x)  { return ::sqrt(x); }
  inline ma_scalar ln(double x)    { return ::log(x); }
  //inline ma_scalar log(ma_scalar x)   { return ::log(x); }
  //inline ma_scalar log2(ma_scalar x)  { return ::log2(x); }
  //inline ma_scalar log10(ma_scalar x) { return ::log10(x); }
  //inline ma_scalar sin(ma_scalar x)   { return ::sin(x); }
  //inline ma_scalar cos(ma_scalar x)   { return ::cos(x); }
  //inline ma_scalar tan(ma_scalar x)   { return ::tan(x); }
  inline ma_scalar power(ma_scalar x,ma_scalar p)   { return ::pow(x,p); }
  inline ma_scalar arcsin(ma_scalar x)           { return ::asin(x); }
  inline ma_scalar arccos(ma_scalar x)           { return ::acos(x); }
  inline ma_scalar arctan(ma_scalar x)           { return ::atan(x); }
  inline ma_scalar arctan2(ma_scalar y,ma_scalar x) { return ::atan2(y,x); }

  // TODO: Once the user space switch to namespace enigma_user has been made,
  // comment in these functions.
  /*
  inline ma_scalar bessel_j0(ma_scalar x)        {return ::j0(x);};
  inline ma_scalar bessel_j1(ma_scalar x)        {return ::j1(x);};
  inline ma_scalar bessel_jn(int x, ma_scalar y) {return ::jn(x, y);};
  inline ma_scalar bessel_y0(ma_scalar x)        {return ::y0(x);};
  inline ma_scalar bessel_y1(ma_scalar x)        {return ::y1(x);};
  inline ma_scalar bessel_yn(int x, ma_scalar y) {return ::yn(x, y);};
  */

  ma_scalar sqr(ma_scalar x);
  ma_scalar logn(ma_scalar n,ma_scalar x);

  ma_scalar sind(ma_scalar x);
  ma_scalar cosd(ma_scalar x);
  ma_scalar tand(ma_scalar x);
  ma_scalar asind(ma_scalar x);
  ma_scalar acosd(ma_scalar x);
  ma_scalar atand(ma_scalar x);
  ma_scalar atand2(ma_scalar y,ma_scalar x);
  ma_scalar arcsind(ma_scalar x);
  ma_scalar arccosd(ma_scalar x);
  ma_scalar arctand(ma_scalar x);
  ma_scalar arctand2(ma_scalar y,ma_scalar x);

  int sign(ma_scalar x);
  int cmp(ma_scalar x,ma_scalar y);
  ma_scalar frac(ma_scalar x);

  ma_scalar degtorad(ma_scalar x);
  ma_scalar radtodeg(ma_scalar x);

  ma_scalar lengthdir_x(ma_scalar len,ma_scalar dir);
  ma_scalar lengthdir_y(ma_scalar len,ma_scalar dir);
  ma_scalar angle_difference(ma_scalar dir1,ma_scalar dir2);
  ma_scalar point_direction(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2);
  ma_scalar point_distance(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2);
  ma_scalar point_distance_3d(ma_scalar x1, ma_scalar y1, ma_scalar z1, ma_scalar x2,
  ma_scalar y2, ma_scalar z2);

  //NOTE: These are technically collision functions in Studio
  ma_scalar triangle_area(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3);
  bool point_in_circle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar rad);
  bool point_in_rectangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2);
  bool point_in_triangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3);

  ma_scalar lines_intersect(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3, ma_scalar x4, ma_scalar y4, bool segment);
  int rectangle_in_circle(ma_scalar x1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x, ma_scalar y, ma_scalar rad);
  int rectangle_in_rectangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2);
  int rectangle_in_triangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2, ma_scalar dx3, ma_scalar dy3);

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

  // Delphi-esque Random

  extern ma_scalar random(ma_scalar n);
  inline ma_scalar random(ma_scalar low, ma_scalar high) {
    return low + random(high - low);
  }
  inline ma_scalar random_range(ma_scalar low, ma_scalar high) { return random(low, high); }

  int random_set_seed(int seed);
  int random_get_seed();

  static inline int random_integer(int x) { // Mark made this inclusive of x...
    return int(random(x + 1));
  }
  static inline ma_scalar random_integer(int low, int high) {
    return low + random_integer(high - low);
  }

  // These functions are for compatability with GM8, wherein they were implemented to circumvent
  // GML's lack of function overload capability. Since irandom is a nice shorthand, it supports
  // random_integer's overload. Since irandom_range is a stupid idea, it uses only GM's behavior.
  #define irandom random_integer
  inline ma_scalar irandom_range(int low, int high) { return random_integer(low, high); }

  // Mersenne random
  int randomize();
  int mtrandom_integer(int x);
  ma_scalar mtrandom();
  int mtrandom_seed(int x);
  unsigned int mtrandom32();
  static inline ma_scalar mtrandom(ma_scalar x) {
    return mtrandom() * x;
  }
  static inline ma_scalar mtrandom(ma_scalar x, ma_scalar y) {
    return x + mtrandom() * (y-x);
  }

  // C Random
  static inline int random32() { return rand(); }
  static inline long random64() { return ::rand(); }
}

#endif // ENIGMA_MATHNC
