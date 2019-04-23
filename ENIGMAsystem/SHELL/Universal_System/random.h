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

#ifndef ENIGMA_MATHRAND
#define ENIGMA_MATHRAND

#include "scalar.h"

#include <cstdlib> // random()

namespace enigma_user {
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

#endif
