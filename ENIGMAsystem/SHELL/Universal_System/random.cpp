/** Copyright (C) 2008-2018 Josh Ventura
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

#include "random.h"

// Note: This hack is justifiable in that it was put here to prevent
// around contributors' bad habits, not because of developers' bad habits.
#define INCLUDED_FROM_SHELLMAIN Not really.
#include "mathnc.h"
#undef INCLUDED_FROM_SHELLMAIN

// For added randomness
// ...................................................

#define UPPER_MASK 0x80000000 // most significant w-r bits
#define LOWER_MASK 0x7fffffff // least significant r bits

namespace enigma {
	int Random_Seed;
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
    // signed overflow is undefined, so we use unsigned overflow
    enigma::Random_Seed = (int)((unsigned int)enigma::Random_Seed * 0x8088405U + 1U);
    return ((unsigned int)enigma::Random_Seed/(double)0x100000000) * n;
  }

  int mtrandom_integer(int x) {
    return x > 0? mtrandom32() * (x/0xFFFFFFFF) : 0;
  }

  int random_set_seed(int seed) { return enigma::Random_Seed = seed; }
  int random_get_seed() { return enigma::Random_Seed; }
  int randomize() { return enigma::Random_Seed = mtrandom32(); }
}
