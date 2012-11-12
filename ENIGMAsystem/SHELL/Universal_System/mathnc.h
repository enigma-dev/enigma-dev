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
    #include <cstdlib> // random()
  #undef j1
  #undef j0
  #undef jn
  #undef y1
  #undef y0
#undef yn

#ifndef INCLUDED_FROM_SHELLMAIN
#error ln2math and stop including this damn header.
#endif

const double pi = M_PI;

// Delphi-esque Random
extern double random(double n);
inline double random(double low, double high) {
  return low + random(high - low);
}
#define random_range random
int random_set_seed(int seed);
int random_get_seed();

static inline int random_integer(int x) { // Mark made this inclusive of x...
  return int(random(x + 1));
}
static inline double random_integer(int low, int high) {
  return low + random_integer(high - low);
}

#define irandom random_integer
#define irandom_range random_integer

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
static inline long random64() { return ::random(); }

// Overloading
double abs(const variant& x);
double ceil(const variant& x);
double floor(const variant& x);
double exp(const variant& x);
double sqrt(const variant& x);
double log10(const variant& x);
double sin(const variant& x);
double cos(const variant& x);
double tan(const variant& x);

double abs(const var& x);
double ceil(const var& x);
double floor(const var& x);
double exp(const var& x);
double sqrt(const var& x);
double log10(const var& x);
double sin(const var& x);
double cos(const var& x);
double tan(const var& x);

double sqr(double x);
double power(double x,double power);
double ln(double x);
double logn(double n,double x);

double arcsin(double x);
double arccos(double x);
double arctan(double x);
double arctan2(double y,double x);

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

double lerp(double x, double y, double a);

double lengthdir_x(double len,double dir);
double lengthdir_y(double len,double dir);
double direction_difference(double dir1,double dir2);
double point_direction(double x1,double y1,double x2,double y2);
double point_distance(double x1, double y1, double x2, double y2);

double max(const enigma::varargs &t);
double min(const enigma::varargs &t);
double min(double x, double y);
double max(double x, double y);
double median(enigma::varargs t);
double mean(const enigma::varargs &t);
variant choose(const enigma::varargs& args);

#endif // ENIGMA_MATHNC
