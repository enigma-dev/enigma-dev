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

#include <stdlib.h>
#include <cmath>
#include "var4.h"
#include "dynamic_args.h"

// Note: This hack is justifiable in that it was put here to prevent
// around contributors' bad habits, not because of developers' bad habits.
#define INCLUDED_FROM_SHELLMAIN Not really.
#include "mathnc.h"

//overloading

namespace enigma_user
{
  // Overloading
  // TODO: Once the user space switch to namespace enigma_user has been made,
  // remove these functions.
  double abs(const variant& x)   { return ::fabs(double(x)); }
  double abs(const var& x)     { return ::fabs(double(x)); }

  // Functions

  double sqr(double x)              { return x*x;      }
  double logn(double n,double x)    { return ::log(x)/::log(n); }

  double sind(double x)              { return ::sin(x * M_PI / 180.0); }
  double cosd(double x)              { return ::cos(x * M_PI / 180.0); }
  double tand(double x)              { return ::tan(x * M_PI / 180.0); }
  double asind(double x)             { return ::asin(x)    * 180.0 / M_PI; }
  double acosd(double x)             { return ::acos(x)    * 180.0 / M_PI; }
  double atand(double x)             { return ::atan(x)    * 180.0 / M_PI; }
  double atand2(double y,double x)   { return ::atan2(y,x) * 180.0 / M_PI; }
  double arcsind(double x)           { return ::asin(x)    * 180.0 / M_PI; }
  double arccosd(double x)           { return ::acos(x)    * 180.0 / M_PI; }
  double arctand(double x)           { return ::atan(x)    * 180.0 / M_PI; }
  double arctand2(double y,double x) { return ::atan2(y,x) * 180.0 / M_PI; }

  int sign(double x)                { return (x>0)-(x<0); }
  int cmp(double x,double y)        { return (x>y)-(x<y); }
  double frac(double x)             { return x-(int)x;    }

  double degtorad(double x)         { return x*(M_PI/180.0);}
  double radtodeg(double x)         { return x*(180.0/M_PI);}

  double lengthdir_x(double len,double dir) { return len *  cosd(dir); }
  double lengthdir_y(double len,double dir) { return len * -sind(dir); }

  double direction_difference(double dir1,double dir2) {
	  return fmod((fmod((dir1 - dir2),360) + 540), 360) - 180;
  }
  double point_direction(double x1,double y1,double x2,double y2) { return ::fmod((::atan2(y1-y2,x2-x1)*(180/M_PI))+360,360); }
  double point_distance(double x1,double y1,double x2,double y2)  { return ::hypot(x2-x1,y2-y1); }
  double point_distance_3d(double x1,double y1,double z1,double x2,
  double y2,double z2)  { return ::sqrt(sqr(x1-x2) + sqr(y1-y2) + sqr(z1-z2)); }
  double dot_product(double x1,double y1,double x2,double y2) { return (x1 * x2 + y1 * y2); }
  double dot_product_3d(double x1,double y1,double z1,double x2,double y2, double z2) { return (x1 * x2 + y1 * y2 + z1 * z2); }
  double dot_product_normalised(double x1,double y1,double x2,double y2) { 
  	double length = sqrt(x1*x1+y1*y1);

    x1 = x1/length;
    y1 = y1/length;
	
	length = sqrt(x2*x2+y2*y2);

    x2 = x2/length;
    y2 = y2/length;
    return (x1 * x2 + y1 * y2); 
  }
  double dot_product_normalised_3d(double x1,double y1,double z1,double x2,double y2, double z2) { 
	double length = sqrt(x1*x1+y1*y1+z1*z1);

    x1 = x1/length;
    y1 = y1/length;
    z1 = z1/length;
	
	length = sqrt(x2*x2+y2*y2+z2*z2);

    x2 = x2/length;
    y2 = y2/length;
    z2 = z2/length;
    return (x1 * x2 + y1 * y2 + z1 * z2); 
  }
  double lerp(double x, double y, double a) { return x + ((y-x)*a); }
  double clamp(double val, double min, double max) {
      if (val < min) { return min; }
      if (val > max) { return max; }
      return val;
  }

  double min(double x, double y) { return x < y ? x : y; }
  double max(double x, double y) { return x > y ? x : y; }

  double max(const enigma::varargs &t)
  {
    register double ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) > ret)
        ret = tst;
    return ret;
  }

  double min(const enigma::varargs &t)
  {
    register double ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) < ret)
        ret = tst;
    return ret;
  }

  double median(enigma::varargs t)
  {
    t.sort();
    if (t.argc & 1)
      return t.get(t.argc/2);
    return (t.get(t.argc/2) + t.get(t.argc/2-1)) / 2.;
  }

  double mean(const enigma::varargs &t)
  {
    register double ret = 0;
    for (int i = 0; i < t.argc; i++)
        ret += t.get(i);
    return ret/t.argc;
  }

  variant choose(const enigma::varargs& args) {
     return args.get(rand() % args.argc);
  }
}

// For added randomness
// ...................................................

#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

namespace enigma {
	unsigned int Random_Seed;
	unsigned long mt[625];
}


/*  The code in this module was based on a download from:
	 http://www.math.keio.ac.jp/~matumoto/MT2002/emt19937ar.html

   It was modified in 2002 by Raymond Hettinger. Then I modified 2009

   The following are the verbatim comments from the original code

   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote
	products derived from this software without specific prior written
	permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.keio.ac.jp/matumoto/emt.html
   email: matumoto@math.keio.ac.jp
*/

namespace enigma_user
{

  unsigned int mtrandom32()
  {
	  unsigned int y;
	  static const unsigned int mag01[2]={0,0x9908b0df};
	  if (enigma::mt[624] >= 624)
	  { /* generate N words at one time */
		  int kk;
		  for(kk=0;kk<227;kk++)
		  {
			  y = (enigma::mt[kk]&UPPER_MASK)|(enigma::mt[kk+1]&LOWER_MASK);
			  enigma::mt[kk] = enigma::mt[kk+397] ^ (y >> 1) ^ mag01[y&1];
		  }
		  for(;kk<623;kk++)
		  {
			  y = (enigma::mt[kk]&UPPER_MASK)|(enigma::mt[kk+1]&LOWER_MASK);
			  enigma::mt[kk] = enigma::mt[kk-227] ^ (y >> 1) ^ mag01[y&1];
		  }

		  y = (enigma::mt[623]&UPPER_MASK) | (enigma::mt[0]&LOWER_MASK);
		  enigma::mt[623] = enigma::mt[396] ^ (y >> 1) ^ mag01[y & 1];
		  enigma::mt[624] = 0;
	  }
	  y = enigma::mt[enigma::mt[624]++];
	  y ^= y >> 11;
	  y ^= (y << 7) & 0x9d2c5680UL;
	  y ^= (y << 15) & 0xefc60000UL;
	  return y^(y >> 18);
  }

  double mtrandom(){
	  return ((mtrandom32()>>5)*67108864.+(mtrandom32()>>6))/9007199254740992.;
  }

  int mtrandom_seed(int x){
	  enigma::mt[0]=x&0xffffffff;
	  for(int mti=1;mti<624;mti++)
		  enigma::mt[mti]=1812433253*(enigma::mt[mti-1]^(enigma::mt[mti-1]>>30))+mti;
	  enigma::mt[624] = 624;
	  return 0;
  }

  // END MERSENNE

  // ENIGMA Random Functions

  double random(double n) // Do not fix:  Based off of Delphi PRNG.
  {
    double rval = frac(
      3.1379939289763571e-2  * (enigma::Random_Seed % 32)
      + 2.3283064365387e-10  * (enigma::Random_Seed / 32 + 1)
      + 4.158057505264878e-3 * (enigma::Random_Seed / 32)
    )*n;
    enigma::Random_Seed = mtrandom32();
    return rval;
  }

  int mtrandom_integer(int x) {
    return x > 0? mtrandom32() * (x/0xFFFFFFFF) : 0;
  }

  int random_set_seed(int seed) { return enigma::Random_Seed = seed; }
  int random_get_seed() { return enigma::Random_Seed; }
  int randomize() { return enigma::Random_Seed = mtrandom32(); }
}

