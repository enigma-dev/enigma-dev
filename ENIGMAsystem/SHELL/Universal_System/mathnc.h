/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
    #include <cmath>
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

extern double random(double n);
int random_set_seed(int seed);
int random_get_seed();
int randomize();
int random_integer(int x);
static inline int irandom(int x) {return int(random(x + 1));}  //Mark made this inclusive of x...
static inline double random_range(double x1, double x2) {return x1 + random(x2 - x1);}
static inline double irandom_range(int x1, int x2) {return x1 + irandom(x2 - x1);}
unsigned int random32();
double mtrandom();
int mtrandom_seed(int x);

//overloading
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
//double tand2(double x);
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
// TODO: Make the compiler do this shit automatically so we can lose the
//       stdargs declaration and the hackish define that fucks with everything

#endif // ENIGMA_MATHNC
