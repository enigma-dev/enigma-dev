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
  void math_set_epsilon(double);

  // Random Functions
  variant choose(const varargs&);
  double random(double);
  double random_range(double, double);
  int irandom(int);
  int irandom_range(int, int);
  void random_set_seed(double);
  double random_get_seed();
  void randomize();

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
  inline int round(const variant&);
  int round(double);
  int round(int);
  inline int floor(const variant&);
  int floor(double);
  int floor(int);
  inline double frac(const variant&);
  double frac(double);
  double frac(int);
  inline variant abs(const variant&);
  double abs(double);
  int abs(int);
  inline int sign(const variant&);
  int sign(double);
  int sign(int);
  inline int ceil(const variant&);
  int ceil(double);
  int ceil(int);
  variant max(const varargs&);
  variant mean(const varargs&);
  variant median(const varargs&);
  variant min(const varargs&);
  double lerp(double, double, double);
  double clamp(double, double, double);

  // Miscellaneous Functions
  int is_real(const variant&);
  int is_string(const variant&);
  double exp(double);
  double ln(double);
  inline variant power(const variant&, double);
  inline variant power(const variant&, int);
  double power(double, double);
  int power(int, int);
  inline variant sqr(const variant&);
  double sqr(double);
  int sqr(int);
  double sqrt(double);
  double log2(double);
  double log10(double);
  double logn(double, double);
}

#endif // ERT_MATH_HPP_
