/** Copyright (C) 2008-2011 Josh Ventura
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
  @file  graphics_object.h
  @brief Adds a collisions-related tier following the transform tier.
*/

#include "collisions_object.h"

namespace enigma
{
  int object_collisions::$bbox_left()   const { return (mask_index >= 0 ? sprite_get_bbox_left_relative  (mask_index) : (sprite_index >= 0 ? sprite_get_bbox_left_relative  (sprite_index) : 0)) + round(x); }
  int object_collisions::$bbox_right()  const { return (mask_index >= 0 ? sprite_get_bbox_right_relative (mask_index) : (sprite_index >= 0 ? sprite_get_bbox_right_relative (sprite_index) : 0)) + round(x); }
  int object_collisions::$bbox_top()    const { return (mask_index >= 0 ? sprite_get_bbox_top_relative   (mask_index) : (sprite_index >= 0 ? sprite_get_bbox_top_relative   (sprite_index) : 0)) + round(y); }
  int object_collisions::$bbox_bottom() const { return (mask_index >= 0 ? sprite_get_bbox_bottom_relative(mask_index) : (sprite_index >= 0 ? sprite_get_bbox_bottom_relative(sprite_index) : 0)) + round(y); }
  object_collisions::object_collisions(): object_transform() {}
  object_collisions::object_collisions(unsigned _id,int _objid): object_transform(_id,_objid) {}
  object_collisions::~object_collisions() {}
}
