/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#include "ert/real.hpp"
#include "ert/variant.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>

namespace ert {
  template <int N>
  struct hash_const;

  template <>
  struct hash_const<4> {
    static const unsigned long base = 2166136261u;
    static const unsigned long prime = 16777619u;
  };

  template <>
  struct hash_const<8> {
    static const unsigned long base = 14695981039346656037u;
    static const unsigned long prime = 1099511628211u;
  };

  static unsigned hash_real(real_t val) {
    const unsigned N = sizeof(double) / sizeof(unsigned char);
    const unsigned char(&chunks)[] = reinterpret_cast<unsigned char(&)[]>(val);
    unsigned hash = hash_const<sizeof(unsigned)>::base;
    for (int i = 0; i < N; i++) {
        hash ^= chunks[i];
        hash *= hash_const<sizeof(unsigned)>::prime;
    }
    return hash;
  }

  static real_t epsilon = 1e-16;
  real_t pi = 3.14159265358979323846;

  real_t math_set_epsilon(real_t eps) {
    epsilon = eps;
    return 0;
  }

  real_t random(real_t ub) {
    return (::rand() * ub) / RAND_MAX;
  }

  real_t random_range(real_t lb, real_t ub) {
    return lb + (::rand() * (ub - lb)) / RAND_MAX;
  }

  real_t irandom(real_t ub) {
    return ::fmod(::rand(), ub) + 1;
  }

  real_t irandom_range(real_t lb, real_t ub) {
    return lb + ::fmod(::rand(), ub - lb) + 1;
  }

  static real_t rand_seed = 0;

  real_t random_set_seed(real_t seed) {
    rand_seed = seed;
    ::srand(hash_real(rand_seed));
    return 0;
  }

  real_t random_get_seed() {
    return rand_seed;
  }

  real_t randomize() {
    random_set_seed(::time(NULL));
    return 0;
  }

  real_t arccos(real_t x) {
    return ::acos(x);
  }

  real_t arcsin(real_t x) {
    return ::asin(x);
  }

  real_t arctan(real_t x) {
    return ::atan(x);
  }

  real_t arctan2(real_t x, real_t y) {
    return ::atan2(x, y);
  }

  real_t sin(real_t x) {
    return ::sin(x);
  }

  real_t tan(real_t x) {
    return ::tan(x);
  }

  real_t cos(real_t x) {
    return ::cos(x);
  }

  real_t degtorad(real_t d) {
    return d * pi / 180;
  }

  real_t radtodeg(real_t r) {
    return r * 180 / pi;
  }

  real_t lengthdir_x(real_t len, real_t dir) {
    return len * cos(dir);
  }

  real_t lengthdir_y(real_t len, real_t dir) {
    return len * sin(dir);
  }
}
