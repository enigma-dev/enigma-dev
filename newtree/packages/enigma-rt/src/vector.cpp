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
#include "ert/math.hpp"

#include <cmath>

namespace ert {
  real_t point_direction(real_t x1, real_t y1, real_t x2, real_t y2) {
    return 180 / pi * std::atan2(y2 - y1, x2 - x1) + 360 * (y2 > y1);
  }

  real_t point_distance(real_t x1, real_t y1, real_t x2, real_t y2) {
    real_t dx = x2 - x1;
    real_t dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
  }

  real_t point_distance_3d(real_t x1, real_t y1, real_t z1, real_t x2, real_t y2, real_t z2) {
    real_t dx = x2 - x1;
    real_t dy = y2 - y1;
    real_t dz = z2 - z1;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
  }

  real_t dot_product(real_t x1, real_t y1, real_t x2, real_t y2) {
    return x1 * x2 + y1 * y2;
  }

  real_t dot_product_3d(real_t x1, real_t y1, real_t z1, real_t x2, real_t y2, real_t z2) {
    return x1 * x2 + y1 * y2 + z1 * z2;
  }

  real_t dot_product_normalised(real_t x1, real_t y1, real_t x2, real_t y2) {
    real_t dx = x2 - x1;
    real_t dy = y2 - y1;
    return (x1 * x2 + y1 * y2) / std::sqrt(dx * dx + dy * dy);
  }

  real_t dot_product_normalised_3d(real_t x1, real_t y1, real_t z1, real_t x2, real_t y2, real_t z2) {
    real_t dx = x2 - x1;
    real_t dy = y2 - y1;
    real_t dz = z2 - z1;
    return (x1 * x2 + y1 * y2 + z1 * z2) / std::sqrt(dx * dx + dy * dy + dz * dz);
  }
}
