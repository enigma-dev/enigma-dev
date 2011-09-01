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

#include <stdlib.h>
#include <cmath>
#include "var4.h"

double bessel_j0(double x) { return j0(x); }
double bessel_j1(double x) { return j1(x); }
double bessel_jn(int x, double y) { return jn(x,y); }
double bessel_y0(double x) { return y0(x); }
double bessel_y1(double x) { return y1(x); }
double bessel_yn(int x, double y) { return yn(x,y); }

//overloading
double abs(const variant& x) { return fabs(double(x)); }
double ceil(const variant& x)              { return ceil((double)x); }
double floor(const variant& x)             { return floor((double)x); }
double exp(const variant& x)               { return exp((double)x); }
double sqrt(const variant& x)              { return sqrt((double)x); }
double log10(const variant& x)             { return log10((double)x); }
double sin(const variant& x)               { return sin((double)x); }
double cos(const variant& x)               { return cos((double)x); }
double tan(const variant& x)               { return tan((double)x); }

double abs(const var& x)     { return fabs(double(x)); }
double ceil(const var& x)              { return ceil((double)x); }
double floor(const var& x)             { return floor((double)x); }
double exp(const var& x)               { return exp((double)x); }
double sqrt(const var& x)              { return sqrt((double)x); }
double log10(const var& x)             { return log10((double)x); }
double sin(const var& x)               { return sin((double)x); }
double cos(const var& x)               { return cos((double)x); }
double tan(const var& x)               { return tan((double)x); }


double round(double x)            { return lrint(x); }
double sqr(double x)              { return x*x;      }
double power(double x,double p)   { return pow(x,p); }
double ln(double x)               { return log(x);   }
double logn(double n,double x)    { return log(x)/log(n); }
double log2(double x)             { return log(x)/M_LN2; }//This may already exist
double arcsin(double x)           { return asin(x);     }
double arccos(double x)           { return acos(x);     }
double arctan(double x)           { return atan(x);     }
double arctan2(double y,double x) { return atan2(y,x);  }

double sind(double x)              { return sin(x * M_PI / 180.0); }
double cosd(double x)              { return cos(x * M_PI / 180.0); }
double tand(double x)              { return tan(x * M_PI / 180.0); }
//double tand2(double y,double x);
double asind(double x)             { return asin(x)    * 180.0 / M_PI; }
double acosd(double x)             { return acos(x)    * 180.0 / M_PI; }
double atand(double x)             { return atan(x)    * 180.0 / M_PI; }
double atand2(double y,double x)   { return atan2(y,x) * 180.0 / M_PI; }
double arcsind(double x)           { return asin(x)    * 180.0 / M_PI; }
double arccosd(double x)           { return acos(x)    * 180.0 / M_PI; }
double arctand(double x)           { return atan(x)    * 180.0 / M_PI; }
double arctand2(double y,double x) { return atan2(y,x) * 180.0 / M_PI; }

int sign(double x)                { return (x>0)-(x<0); }
int cmp(double x,double y)        { return (x>y)-(x<y); }
double frac(double x)             { return x-(int)x;    }

double degtorad(double x)         { return x*(M_PI/180.0);}
double radtodeg(double x)         { return x*(180.0/M_PI);}

double lengthdir_x(double len,double dir) { return len *  cosd(dir); }
double lengthdir_y(double len,double dir) { return len * -sind(dir); }

double lerp(double x, double y, double a) { return x + ((y-x)*a); }

double direction_difference(double dir1,double dir2) {
	dir1=fmod(dir2-dir1,360)+180;
	return dir1-(dir1>360?540:(dir1<0?360:180));
}
double point_direction(double x1,double y1,double x2,double y2) { return fmod((atan2(y1-y2,x2-x1)*(180/M_PI))+360,360); }
double point_distance(double x1,double y1,double x2,double y2)  { return hypot(x2-x1,y2-y1); }

#include "dynamic_args.h"
#include <float.h> //maxiumum values for certain datatypes. Useful for min()
#include <list>
double min(double x, double y) { return x < y ? x : y; }
double max(double x, double y) { return x > y ? x : y; }

double max(const enigma::varargs &t)
{
  register double ret = -DBL_MAX, tst;
  for (int i = 0; i < t.argc; i++)
    if ((tst = t.get(i)) > ret)
      ret = tst;
  return ret;
}

double min(const enigma::varargs &t)
{
  register double ret = DBL_MAX, tst;
  for (int i = 0; i < t.argc; i++)
    if ((tst = t.get(i)) < ret)
      ret = tst;
  return ret;
}

double median(const enigma::varargs &t)
{
  std::list<double> numbers;
  for (int i = 0; i < t.argc; i++)
    numbers.push_back(t.get(i));
  numbers.sort();
  std::list<double>::iterator it = numbers.begin();
  advance(it,(numbers.size()-1)/2);
  return *it;
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
};

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
   email: matumoto@math.keio.ac.jp*/

unsigned int random32()
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
	return ((random32()>>5)*67108864.+(random32()>>6))/9007199254740992.;
}

int mtrandom_seed(int x){
	enigma::mt[0]=x&0xffffffff;
	for(int mti=1;mti<624;mti++)
		enigma::mt[mti]=1812433253*(enigma::mt[mti-1]^(enigma::mt[mti-1]>>30))+mti;
	enigma::mt[624] = 624;
	return 0;
}

int random_integer(int x) {return x>0?random32()*(x/0xFFFFFFFF):0;}
//END MERSENNE

double random(double n) //Do not fix. Based off of Delphi prng
{
	double rval=frac(
		0.031379939289763571*(enigma::Random_Seed%32)
		+0.00000000023283064365387*(enigma::Random_Seed/32+1)
		+0.004158057505264878*(enigma::Random_Seed/32))*n;
	enigma::Random_Seed=random32();
	return rval;
}

int random_set_seed(int seed){return enigma::Random_Seed=seed;}
int random_get_seed(){return enigma::Random_Seed;}
int randomize(){return enigma::Random_Seed=random32();}
