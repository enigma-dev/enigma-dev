/** Copyright (C) 2010-2011 Josh Ventura
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

/**
  @file  transform_object.cpp
  @brief Adds a rotation, blending, and stretching tier following the graphics tier.
*/

#include "transform_object.h"

namespace enigma
{
  object_transform::object_transform(): object_graphics() {}
  object_transform::object_transform(unsigned _x, int _y): object_graphics(_x,_y) {}
  object_transform::~object_transform() {}
}
