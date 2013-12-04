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
#include "ert/math.hpp"

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <numeric>
#include <initializer_list>

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
    const unsigned char(&chunks)[N] = reinterpret_cast<unsigned char(&)[N]>(val);
    unsigned hash = hash_const<sizeof(unsigned)>::base;
    for (unsigned i = 0; i < N; i++) {
      hash ^= chunks[i];
      hash *= hash_const<sizeof(unsigned)>::prime;
    }
    return hash;
  }

  static real_t epsilon = 1e-16;
  const real_t pi = 3.14159265358979323846;

  real_t math_set_epsilon(real_t eps) {
    epsilon = eps;
    return 0;
  }
    
  variant_t choose(const std::initializer_list<variant_t> vars) {
    return *(vars.begin() + static_cast<unsigned>(irandom(vars.size())));
  }

  real_t random(real_t ub) {
    return (std::rand() * ub) / RAND_MAX;
  }

  real_t random_range(real_t lb, real_t ub) {
    return lb + (std::rand() * (ub - lb)) / RAND_MAX;
  }

  real_t irandom(real_t ub) {
    return std::floor(std::fmod(std::rand(), ub));
  }

  real_t irandom_range(real_t lb, real_t ub) {
    return lb + std::floor(std::fmod(std::rand(), ub - lb));
  }

  static real_t rand_seed = 0;

  real_t random_set_seed(real_t seed) {
    rand_seed = seed;
    std::srand(hash_real(rand_seed));
    return 0;
  }

  real_t random_get_seed() {
    return rand_seed;
  }

  real_t randomize() {
    random_set_seed(static_cast<real_t>(std::time(NULL)));
    return 0;
  }

  real_t arccos(real_t x) {
    return std::acos(x);
  }

  real_t arcsin(real_t x) {
    return std::asin(x);
  }

  real_t arctan(real_t x) {
    return std::atan(x);
  }

  real_t arctan2(real_t x, real_t y) {
    return std::atan2(x, y);
  }

  real_t sin(real_t x) {
    return std::sin(x);
  }

  real_t tan(real_t x) {
    return std::tan(x);
  }

  real_t cos(real_t x) {
    return std::cos(x);
  }

  real_t degtorad(real_t d) {
    return pi / 180 * d;
  }

  real_t radtodeg(real_t r) {
    return 180 / pi * r;
  }

  real_t lengthdir_x(real_t len, real_t dir) {
    return len * std::cos(dir);
  }

  real_t lengthdir_y(real_t len, real_t dir) {
    return len * std::sin(dir);
  }

  real_t round(real_t x) {
    return std::round(x);
  }

  real_t floor(real_t x) {
    return std::floor(x);
  }

  real_t frac(real_t x) {
    double i;
    return std::modf(x, &i);
  }

  real_t abs(real_t x) {
    return std::fabs(x);
  }

  real_t sign(real_t x) {
    return (x > 0) - (x < 0);
  }

  real_t ceil(real_t x) {
    return std::ceil(x);
  }

  variant_t max(const std::initializer_list<variant_t> vars) {
    return *std::max_element(vars.begin(), vars.end());
  }

  variant_t min(const std::initializer_list<variant_t> vars) {
    return *std::min_element(vars.begin(), vars.end());
  }

  real_t mean(const std::initializer_list<real_t> vars) {
    return std::accumulate(vars.begin(), vars.end(), 0, std::plus<double>()) / vars.size();
  }

  real_t median(std::initializer_list<real_t> vars) {
    const unsigned x = vars.size() / 2;
    if (x & 1) {
      std::nth_element(vars.begin(), vars.begin() + x, vars.end());
      return *(vars.begin() + x);
    }
    const unsigned y = x + 1;
    std::nth_element(vars.begin(), vars.begin() + y, vars.end());
    return (*(vars.begin() + x) + *(vars.begin() + y)) * 0.5;
  }
  
  real_t is_real(const variant_t& var) {
    return var.type == variant::vt_real;
  }

  real_t is_string(const variant_t& var) {
    return var.type == variant::vt_string;
  }

  real_t exp(real_t x) {
    return std::exp(x);
  }

  real_t ln(real_t x) {
    return std::log(x);
  }

  real_t power(real_t b, real_t e) {
    return std::pow(b, e);
  }

  real_t sqr(real_t x) {
    return x * x;
  }

  real_t sqrt(real_t x) {
    return std::sqrt(x);
  }

  real_t log2(real_t x) {
    return std::log2(x);
  }

  real_t log10(real_t x) {
    return std::log10(x);
  }

  real_t logn(real_t base, real_t val) {
    return std::log(val) / std::log(base);
  }
}
