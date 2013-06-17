/** Copyright (C) 2013 Josh Ventura
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

#ifndef _FLOATCMP__H
#define _FLOATCMP__H
#include <cmath>
static inline bool fzero(double x)   { return fabs(x) < 1e-8; }
static inline bool fnzero(double x)  { return fabs(x) >= 1e-8; }
static inline bool ftrueGM(double x) { return x >= .5; }
static inline bool fequal(double x, double y) { return fabs(x - y) < 1e-8; }
#endif
