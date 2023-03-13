/** Copyright (C) 2013 Robert B. Colton
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

#include "math_consts.h"
#include "rect.h"

// This header implements scalar data types for double and floating point precision
// for major systems and coordinate space. Angular unit functions are also implemented
// to allow you to easily switch major systems between degrees and radians.

// Uncomment the following lines below to switch to double.
#define MA_SCALAR_64
//#define GS_SCALAR_64
//#define AS_SCALAR_64
#define CS_SCALAR_64

// Controls mathematics scalar precision, where possible.
// Note that many methods offer arbitrary precision, or are locked into the C++
// standard library's choice of precision (double).
#ifdef MA_SCALAR_64
typedef double ma_scalar;
#else
typedef float ma_scalar;
#endif

// Controls graphics, particles, and other rendering precision
#ifdef GS_SCALAR_64
typedef double gs_scalar;
#else
typedef float gs_scalar;
#endif

// Controls audio coordinate space precision
#ifdef AS_SCALAR_64
typedef double as_scalar;
#else
typedef float as_scalar;
#endif

// Controls collision, path, and motion planning precision
#ifdef CS_SCALAR_64
typedef double cs_scalar;
#else
typedef float cs_scalar;
#endif

// Uncomment the following lines below to switch major systems to radians for the internal angular unit.
//#define MA_ANGULAR_UNIT_RADIANS
//#define GS_ANGULAR_UNIT_RADIANS
//#define AS_ANGULAR_UNIT_RADIANS
//#define CS_ANGULAR_UNIT_RADIANS

#include <math.h>

#ifdef MA_ANGULAR_UNIT_RADIANS
#define ma_angle_from_radians(radians) (radians)
#define ma_angle_to_radians(degrees) (degrees)
#else
#define ma_angle_from_radians(radians) ((radians) * 180 / M_PI)
#define ma_angle_to_radians(degrees) ((degrees) / 180 * M_PI)
#endif

#ifdef GS_ANGULAR_UNIT_RADIANS
#define gs_angle_from_radians(radians) (radians)
#define gs_angle_to_radians(degrees) (degrees)
#else
#define gs_angle_from_radians(radians) ((radians) * 180 / M_PI)
#define gs_angle_to_radians(degrees) ((degrees) / 180 * M_PI)
#endif

#ifdef AS_ANGULAR_UNIT_RADIANS
#define as_angle_from_radians(radians) (radians)
#define as_angle_to_radians(degrees) (degrees)
#else
#define as_angle_from_radians(radians) ((radians) * 180 / M_PI)
#define as_angle_to_radians(degrees) ((degrees) / 180 * M_PI)
#endif

#ifdef CS_ANGULAR_UNIT_RADIANS
#define cs_angle_from_radians(radians) (radians)
#define cs_angle_to_radians(degrees) (degrees)
#else
#define cs_angle_from_radians(radians) ((radians) * 180 / M_PI)
#define cs_angle_to_radians(degrees) ((degrees) / 180 * M_PI)
#endif

namespace enigma {
  
using TexRect = Rect<gs_scalar>;

}
