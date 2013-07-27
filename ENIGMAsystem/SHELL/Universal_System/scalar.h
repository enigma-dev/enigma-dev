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

// This header implements scalar data types for double and floating point precision
// for major systems and coordinate space. Uncomment the three lines below to switch
// to double.

//#define GS_SCALAR_64
//#define AS_SCALAR_64
//#define CS_SCALAR_64

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
