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

#ifndef ERT_MATH_HPP_
#define ERT_MATH_HPP_

#include "ert/real.hpp"
#include "ert/variant.hpp"

namespace ert {
  real_t math_set_epsilon(real_t);

  // Random Functions
  variant_t choose(const varargs_t&);
  real_t random(real_t);
  real_t random_range(real_t, real_t);
  real_t irandom(real_t);
  real_t irandom_range(real_t, real_t);
  real_t random_set_seed(real_t);
  real_t random_get_seed();
  real_t randomize();

  // Trigonometric Functions
  real_t arccos(real_t);
  real_t arcsin(real_t);
  real_t arctan(real_t);
  real_t arctan2(real_t, real_t);
  real_t sin(real_t);
  real_t tan(real_t);
  real_t cos(real_t);
  real_t degtorad(real_t);
  real_t radtodeg(real_t);
  real_t lengthdir_x(real_t, real_t);
  real_t lengthdir_y(real_t, real_t);

  // Rounding Functions
  real_t round(real_t);
  real_t floor(real_t);
  real_t frac(real_t);
  real_t abs(real_t);
  real_t sign(real_t);
  real_t ceil(real_t);
  variant_t max(const varargs_t&);
  variant_t mean(const varargs_t&);
  variant_t median(const varargs_t&);
  variant_t min(const varargs_t&);
  real_t lerp(real_t, real_t, real_t);
  real_t clamp(real_t, real_t, real_t);

  // Miscellaneous Functions
  real_t is_real(const variant_t&);
  real_t is_string(const variant_t&);
  real_t exp(real_t);
  real_t ln(real_t);
  real_t power(real_t, real_t);
  real_t sqr(real_t);
  real_t sqrt(real_t);
  real_t log2(real_t);
  real_t log10(real_t);
  real_t logn(real_t, real_t);
}

#endif // ERT_MATH_HPP_
