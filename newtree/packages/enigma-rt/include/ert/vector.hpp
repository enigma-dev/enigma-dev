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

#include "ert/real.hpp"

namespace ert {
  namespace internal {
    real_t point_direction_rad(real_t x1, real_t y1, real_t x2, real_t y2);
    real_t vector_direction_rad(real_t x, real_t y);
    real_t vector_length(real_t x, real_t y);
    real_t vector_length_3d(real_t x, real_t y, real_t z);
  }
  
  // Vector Functions
  real_t point_direction(real_t, real_t, real_t, real_t);
  real_t point_distance(real_t, real_t, real_t, real_t);
  real_t point_distance_3d(real_t, real_t, real_t, real_t, real_t, real_t);
  real_t dot_product(real_t, real_t, real_t, real_t);
  real_t dot_product_3d(real_t, real_t, real_t, real_t, real_t, real_t);
  real_t dot_product_normalised(real_t, real_t, real_t, real_t);
  real_t dot_product_normalised_3d(real_t, real_t, real_t, real_t, real_t, real_t);
}

#endif // ERT_VECTOR_HPP_
