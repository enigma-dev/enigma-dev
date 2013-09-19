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

namespace enigma_user
{
  const double pi = M_PI;

  // Overloading
  // TODO: Once the user space switch to namespace enigma_user has been made,
  // remove these functions.
  double abs(const variant& x);
  double abs(const var& x);

  // Functions

  // TODO: Once the user space switch to namespace enigma_user has been made,
  // comment in all these functions.
  inline double abs(double x)   { return ::fabs(x); }
  //inline double ceil(double x)  { return ::ceil(x); }
  //inline double floor(double x) { return ::floor(x); }
  //inline double round(double x) { return ::round(x); }
  //inline double exp(double x)   { return ::exp(x); }
  //inline double sqrt(double x)  { return ::sqrt(x); }
  inline double ln(double x)    { return ::log(x); }
  //inline double log(double x)   { return ::log(x); }
  //inline double log2(double x)  { return ::log2(x); }
  //inline double log10(double x) { return ::log10(x); }
  //inline double sin(double x)   { return ::sin(x); }
  //inline double cos(double x)   { return ::cos(x); }
  //inline double tan(double x)   { return ::tan(x); }
  inline double power(double x,double p)   { return ::pow(x,p); }
  inline double arcsin(double x)           { return ::asin(x); }
  inline double arccos(double x)           { return ::acos(x); }
  inline double arctan(double x)           { return ::atan(x); }
  inline double arctan2(double y,double x) { return ::atan2(y,x); }

  // TODO: Once the user space switch to namespace enigma_user has been made,
  // comment in these functions.
  /*
  inline double bessel_j0(double x)        {return ::j0(x);};
  inline double bessel_j1(double x)        {return ::j1(x);};
  inline double bessel_jn(int x, double y) {return ::jn(x, y);};
  inline double bessel_y0(double x)        {return ::y0(x);};
  inline double bessel_y1(double x)        {return ::y1(x);};
  inline double bessel_yn(int x, double y) {return ::yn(x, y);};
  */

  double sqr(double x);
  double logn(double n,double x);

  double sind(double x);
  double cosd(double x);
  double tand(double x);
  double asind(double x);
  double acosd(double x);
  double atand(double x);
  double atand2(double y,double x);
  double arcsind(double x);
  double arccosd(double x);
  double arctand(double x);
  double arctand2(double y,double x);

  int sign(double x);
  int cmp(double x,double y);
  double frac(double x);

  double degtorad(double x);
  double radtodeg(double x);

  double lengthdir_x(double len,double dir);
  double lengthdir_y(double len,double dir);
  double direction_difference(double dir1,double dir2);
  double point_direction(double x1,double y1,double x2,double y2);
  double point_distance(double x1, double y1, double x2, double y2);
  double point_distance_3d(double x1, double y1, double z1, double x2,
  double y2, double z2);
  double dot_product(double x1, double y1, double x2, double y2);
  double dot_product_3d(double x1, double y1, double z1, double x2,
  double y2, double z2);
  double dot_product_normalised(double x1, double y1, double x2, double y2);
  double dot_product_normalised_3d(double x1, double y1, double z1, double x2,
  double y2, double z2);
  double lerp(double x, double y, double a);
  double clamp(double val, double min, double max);

  double max(const enigma::varargs &t);
  double min(const enigma::varargs &t);
  double min(double x, double y);
  double max(double x, double y);
  double median(enigma::varargs t);
  double mean(const enigma::varargs &t);
  variant choose(const enigma::varargs& args);

  // Delphi-esque Random

  extern double random(double n);
  inline double random(double low, double high) {
    return low + random(high - low);
  }
  inline double random_range(double low, double high) { return random(low, high); }

  int random_set_seed(int seed);
  int random_get_seed();

  static inline int random_integer(int x) { // Mark made this inclusive of x...
    return int(random(x + 1));
  }
  static inline double random_integer(int low, int high) {
    return low + random_integer(high - low);
  }

  // These functions are for compatability with GM8, wherein they were implemented to circumvent
  // GML's lack of function overload capability. Since irandom is a nice shorthand, it supports
  // random_integer's overload. Since irandom_range is a stupid idea, it uses only GM's behavior.
  #define irandom random_integer
  inline double irandom_range(int low, int high) { return random_integer(low, high); }

  // Mersenne random
  int randomize();
  int mtrandom_integer(int x);
  double mtrandom();
  int mtrandom_seed(int x);
  unsigned int mtrandom32();
  static inline double mtrandom(double x) {
    return mtrandom() * x;
  }
  static inline double mtrandom(double x, double y) {
    return x + mtrandom() * (y-x);
  }

  // C Random
  static inline int random32() { return rand(); }
  static inline long random64() { return ::rand(); }
}

#endif // ENIGMA_MATHNC
