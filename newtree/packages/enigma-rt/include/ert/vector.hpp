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

#ifndef ERT_VECTOR_HPP_
#define ERT_VECTOR_HPP_

namespace ert {
  // Vector Functions
  double point_direction(double, double, double, double);
  double point_distance(double, double, double, double);
  double point_distance_3d(double, double, double, double, double, double);
  double dot_product(double, double, double, double);
  double dot_product_3d(double, double, double, double, double, double);
  double dot_product_normalised(double, double, double, double);
  double dot_product_normalised_3d(double, double, double, double, double, double);
}

#endif // ERT_VECTOR_HPP_