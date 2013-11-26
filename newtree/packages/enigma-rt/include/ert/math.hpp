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

namespace ert {
  double math_set_epsilon(double);

  // Random Functions
  variant choose(const varargs&);
  double random(double);
  double random_range(double, double);
  double irandom(double);
  double irandom_range(double, double);
  double random_set_seed(double);
  double random_get_seed();
  double randomize();

  // Trigonometric Functions
  double arccos(double);
  double arcsin(double);
  double arctan(double);
  double arctan2(double, double);
  double sin(double);
  double tan(double);
  double cos(double);
  double degtorad(double);
  double radtodeg(double);
  double lengthdir_x(double, double);
  double lengthdir_y(double, double);

  // Rounding Functions
  double round(double);
  double floor(double);
  double frac(double);
  double abs(double);
  double sign(double);
  double ceil(double);
  variant max(const varargs&);
  variant mean(const varargs&);
  variant median(const varargs&);
  variant min(const varargs&);
  double lerp(double, double, double);
  double clamp(double, double, double);

  // Miscellaneous Functions
  double is_real(const variant&);
  double is_string(const variant&);
  double exp(double);
  double ln(double);
  double power(double, double);
  double sqr(double);
  double sqrt(double);
  double log2(double);
  double log10(double);
  double logn(double, double);
}

#endif // ERT_MATH_HPP_
