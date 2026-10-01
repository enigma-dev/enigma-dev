/** This file is a part of the ENIGMA Development Environment.
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

// GML number helpers shared by the runtime and generated code.

#ifndef ENIGMA_GML_OPS_H
#define ENIGMA_GML_OPS_H

#include <cmath>

namespace enigma {

// Round half to even, independent of the floating-point rounding mode. GM8
// (Delphi's Round) uses this for round(), array indices, views and integer
// conversions.
inline double round_half_even(double x) {
  double r = std::floor(x + 0.5);
  if (r - x == 0.5 && std::fmod(r, 2.0) != 0) r -= 1;
  return r;
}

}  // namespace enigma

#endif  // ENIGMA_GML_OPS_H
