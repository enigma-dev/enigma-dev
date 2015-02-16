/** Copyright (C) 2008-2012 Josh Ventura
*** Copyright (C) 2014 Robert B. Colton, TheExDeus
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
  ma_scalar abs(const variant& x)   { return ::fabs(ma_scalar(x)); }
  ma_scalar abs(const var& x)     { return ::fabs(ma_scalar(x)); }

  // Functions

  ma_scalar sqr(ma_scalar x)              { return x*x;      }
  ma_scalar logn(ma_scalar n,ma_scalar x)    { return ::log(x)/::log(n); }

  ma_scalar sind(ma_scalar x)              { return ::sin(x * M_PI / 180.0); }
  ma_scalar cosd(ma_scalar x)              { return ::cos(x * M_PI / 180.0); }
  ma_scalar tand(ma_scalar x)              { return ::tan(x * M_PI / 180.0); }
  ma_scalar asind(ma_scalar x)             { return ::asin(x)    * 180.0 / M_PI; }
  ma_scalar acosd(ma_scalar x)             { return ::acos(x)    * 180.0 / M_PI; }
  ma_scalar atand(ma_scalar x)             { return ::atan(x)    * 180.0 / M_PI; }
  ma_scalar atand2(ma_scalar y,ma_scalar x)   { return ::atan2(y,x) * 180.0 / M_PI; }
  ma_scalar arcsind(ma_scalar x)           { return ::asin(x)    * 180.0 / M_PI; }
  ma_scalar arccosd(ma_scalar x)           { return ::acos(x)    * 180.0 / M_PI; }
  ma_scalar arctand(ma_scalar x)           { return ::atan(x)    * 180.0 / M_PI; }
  ma_scalar arctand2(ma_scalar y,ma_scalar x) { return ::atan2(y,x) * 180.0 / M_PI; }

  int sign(ma_scalar x)                { return (x>0)-(x<0); }
  int cmp(ma_scalar x,ma_scalar y)        { return (x>y)-(x<y); }
  ma_scalar frac(ma_scalar x)             { return x-(int)x;    }

  ma_scalar degtorad(ma_scalar x)         { return x*(M_PI/180.0);}
  ma_scalar radtodeg(ma_scalar x)         { return x*(180.0/M_PI);}

  ma_scalar lengthdir_x(ma_scalar len,ma_scalar dir) { return len *  cosd(dir); }
  ma_scalar lengthdir_y(ma_scalar len,ma_scalar dir) { return len * -sind(dir); }

  ma_scalar angle_difference(ma_scalar dir1,ma_scalar dir2) {
	  return fmod((fmod((dir1 - dir2),360) + 540), 360) - 180;
  }
  ma_scalar point_direction(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2) { return ::fmod((::atan2(y1-y2,x2-x1)*(180/M_PI))+360,360); }
  ma_scalar point_distance(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2)  { return ::hypot(x2-x1,y2-y1); }
  ma_scalar point_distance_3d(ma_scalar x1,ma_scalar y1,ma_scalar z1,ma_scalar x2,
  ma_scalar y2,ma_scalar z2)  { return ::sqrt(sqr(x1-x2) + sqr(y1-y2) + sqr(z1-z2)); }
  
  ma_scalar triangle_area(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3){
    return fabs(((x2 - x1)*(y3 - y1) - (x3 - x1)*(y2 - y1))/2.0);
  }

  
  bool point_in_circle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar rad) {
    return (((px-x1)*(px-x1)+(py-y1)*(py-y1))<rad*rad);
  }
  
  bool point_in_rectangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2) {
    return px > x1 && px < x2 && py > y1 && py < y2; 
  }
  
  bool point_in_triangle(ma_scalar px, ma_scalar py, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3) {
    ma_scalar a = (x1 - px)*(y2 - py) - (x2 - px)*(y1 - py);
    ma_scalar b = (x2 - px)*(y3 - py) - (x3 - px)*(y2 - py);
    ma_scalar c = (x3 - px)*(y1 - py) - (x1 - px)*(y3 - py);
    return (sign(a) == sign(b) && sign(b) == sign(c));
  }
	
  //Based on GMLscripts.com
  ma_scalar lines_intersect(ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3, ma_scalar x4, ma_scalar y4, bool segment) {
    ma_scalar ud, ua = 0;
    ud = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (ud != 0) {
        ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ud;
        if (segment) {
            ma_scalar ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ud;
            if (ua < 0 || ua > 1 || ub < 0 || ub > 1) ua = 0;
        }
    }
    return ua;
  }
 
   int rectangle_in_rectangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar dx1, ma_scalar dy1, ma_scalar dx2, ma_scalar dy2) {
    if (dx2 > sx1 && dx1 < sx2 && dy2 > sy1 && dy1 < sy2) {
      ma_scalar iw = std::min(sx2, dx2) - std::max(sx1, dx1);
      ma_scalar ih = std::min(sy2, dy2) - std::max(sy1, dy1);
      return (iw*ih==std::abs((sx2-sx1)*(sy2-sy1))?1:2);
    }
    return 0;
  }
  
  int rectangle_in_circle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x, ma_scalar y, ma_scalar rad){
    ma_scalar rw2 = (sx2-sx1)/2.0;
    ma_scalar rh2 = (sy2-sy1)/2.0;
   
    ma_scalar cDx = fabs(x - (sx1+rw2));
    ma_scalar cDy = fabs(y - (sy1+rh2));
   
    rw2 = fabs(rw2);
    rh2 = fabs(rh2);
   
    //Check no intersection
    if (cDx > (rw2 + rad)) return 0;
    if (cDy > (rh2 + rad)) return 0;
   
    //Check if totally inside
    ma_scalar cpx = fmax(fabs(x-sx1),fabs(x-sx2));
    ma_scalar cpy = fmax(fabs(y-sy1),fabs(y-sy2));
    if (cpx*cpx + cpy*cpy <= rad*rad) return 1;
   
    //Check partial overlap
    if (cDx <= rw2 ) return 2;
    if (cDy <= rh2 ) return 2;
    //Check corner case
    ma_scalar csq = (cDx - rw2)*(cDx - rw2) + (cDy - rh2)*(cDy - rh2);
    return ((csq <= (rad*rad))?2:0);
  }
  
  //Based on discussion here in http://seb.ly/2009/05/super-fast-trianglerectangle-intersection-test/
  int rectangle_in_triangle(ma_scalar sx1, ma_scalar sy1, ma_scalar sx2, ma_scalar sy2, ma_scalar x1, ma_scalar y1, ma_scalar x2, ma_scalar y2, ma_scalar x3, ma_scalar y3){
    //Check if all points of the triangle are on one side of the rectangle
    if ((sx1>x1 && sx1>x2 && sx1>x3) || (sx2<x1 && sx2<x2 && sx2<x3) || (sy1>y1 && sy1>y2 && sy1>y3) || (sy2<y1 && sy2<y2 && sy2<y3)){
         return 0;
    }
 
    //Check partial collision with lines
    int b1 = ((x1 > sx1) ? 1 : 0) | (((y1 > sy1) ? 1 : 0) << 1) |
        (((x1 > sx2) ? 1 : 0) << 2) | (((y1 > sy2) ? 1 : 0) << 3);
    if (b1 == 3) return 2;
 
    int b2 = ((x2 > sx1) ? 1 : 0) | (((y2 > sy1) ? 1 : 0) << 1) |
        (((x2 > sx2) ? 1 : 0) << 2) | (((y2 > sy2) ? 1 : 0) << 3);
    if (b2 == 3) return 2;
 
    int b3 = ((x3 > sx1) ? 1 : 0) | (((y3 > sy1) ? 1 : 0) << 1) |
        (((x3 > sx2) ? 1 : 0) << 2) | (((y3 > sy2) ? 1 : 0) << 3);
    if (b3 == 3) return 2;
 
    //Check partial collision with points
    int i = b1 ^ b2;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y2-y1) / (x2-x1);
        c = y1 -(m * x1);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }
 
    i = b2 ^ b3;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y3-y2) / (x3-x2);
        c = y2 -(m * x2);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }
 
    i = b1 ^ b3;
    if (i != 0)
    {
        ma_scalar c, m, s;
        m = (y3-y1) / (x3-x1);
        c = y1 -(m * x1);
        if (i & 1) { s = m * sx1 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 2) { s = (sy1 - c) / m; if ( s > sx1 && s < sx2) return 2; }
        if (i & 4) { s = m * sx2 + c; if ( s > sy1 && s < sy2) return 2; }
        if (i & 8) { s = (sy2 - c) / m; if ( s > sx1 && s < sx2) return 2; }
    }
 
    //Check if totally overlapped (at this point can be done by checking if the rectangle center is inside the triangle)
    if (point_in_triangle(sx1+(sx2-sx1)/2.0, sy1+(sy2-sy1)/2.0,x1,y1,x2,y2,x3,y3)) return 1;
 
    return 0;
  }
  
  ma_scalar dot_product(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2) { return (x1 * x2 + y1 * y2); }
  ma_scalar dot_product_3d(ma_scalar x1,ma_scalar y1,ma_scalar z1,ma_scalar x2,ma_scalar y2, ma_scalar z2) { return (x1 * x2 + y1 * y2 + z1 * z2); }
  ma_scalar dot_product_normalised(ma_scalar x1,ma_scalar y1,ma_scalar x2,ma_scalar y2) { 
  	ma_scalar length = sqrt(x1*x1+y1*y1);

    x1 = x1/length;
    y1 = y1/length;
	
	length = sqrt(x2*x2+y2*y2);

    x2 = x2/length;
    y2 = y2/length;
    return (x1 * x2 + y1 * y2); 
  }
  ma_scalar dot_product_normalised_3d(ma_scalar x1,ma_scalar y1,ma_scalar z1,ma_scalar x2,ma_scalar y2, ma_scalar z2) { 
	ma_scalar length = sqrt(x1*x1+y1*y1+z1*z1);

    x1 = x1/length;
    y1 = y1/length;
    z1 = z1/length;
	
	length = sqrt(x2*x2+y2*y2+z2*z2);

    x2 = x2/length;
    y2 = y2/length;
    z2 = z2/length;
    return (x1 * x2 + y1 * y2 + z1 * z2); 
  }
  ma_scalar lerp(ma_scalar x, ma_scalar y, ma_scalar a) { return x + ((y-x)*a); }
  ma_scalar clamp(ma_scalar val, ma_scalar min, ma_scalar max) {
      if (val < min) { return min; }
      if (val > max) { return max; }
      return val;
  }

  ma_scalar min(ma_scalar x, ma_scalar y) { return x < y ? x : y; }
  ma_scalar max(ma_scalar x, ma_scalar y) { return x > y ? x : y; }

  ma_scalar max(const enigma::varargs &t)
  {
    register ma_scalar ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) > ret)
        ret = tst;
    return ret;
  }

  ma_scalar min(const enigma::varargs &t)
  {
    register ma_scalar ret = t.get(0), tst;
    for (int i = 1; i < t.argc; i++)
      if ((tst = t.get(i)) < ret)
        ret = tst;
    return ret;
  }

  ma_scalar median(enigma::varargs t)
  {
    t.sort();
    if (t.argc & 1)
      return t.get(t.argc/2);
    return (t.get(t.argc/2) + t.get(t.argc/2-1)) / 2.;
  }

  ma_scalar mean(const enigma::varargs &t)
  {
    register ma_scalar ret = 0;
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

  ma_scalar mtrandom(){
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

  ma_scalar random(ma_scalar n) // Do not fix:  Based off of Delphi PRNG.
  {
    ma_scalar rval = frac(
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

