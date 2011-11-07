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

#ifndef _TRANSFORM_OBJECT_H
#define _TRANSFORM_OBJECT_H

#include "graphics_object.h"

namespace enigma
{
  struct object_transform: object_graphics
  {
    //Other attributes that logically don't belong here
        double image_alpha;
        int    image_blend;
        int    image_single;
    //Constructors
      object_transform();
      object_transform(unsigned x, int y);
      virtual ~object_transform();
  };
}

#endif
