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

#include <math.h>
#include <stdlib.h>

//double log10(double x){return log10(x);}
//double sin(double x){return sin(x);}
//double cos(double x){return cos(x);}
//double tan(double x){return tan(x);}
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
double min(double x,double y)     { return fmin(x,y);   }
double max(double x,double y)     { return fmax(x,y);   }
int sign(double x)                { return (x>0)-(x<0); }
int cmp(double x,double y)        { return (x>y)-(x<y); }
double frac(double x)             { return x-(int)x;    }

double degtorad(double x)         { return x*(M_PI/180.0);}
double radtodeg(double x)         { return x*(180.0/M_PI);}

double lengthdir_x(double len,double dir) { return len*cos(degtorad(dir));}
double lengthdir_y(double len,double dir) { return len*-sin(degtorad(dir));}

double direction_difference(double dir1,double dir2) {
	dir1=fmod(dir2-dir1,360)+180;
	return dir1-(dir1>360?540:(dir1<0?360:180));
}
double point_direction(double x1,double y1,double x2,double y2) { return ((y1<y2?2*M_PI:0)-atan2(y2-y1,x2-x1))*(180/M_PI); }
double point_distance(double x1,double y1,double x2,double y2)  { return hypot(x2-x1,y2-y1); }

#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

namespace enigma {
  extern unsigned int Random_Seed,mt[625];
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

int random_integer(int x){return x>0?random32()*(x/0xFFFFFFFF):0;}
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
