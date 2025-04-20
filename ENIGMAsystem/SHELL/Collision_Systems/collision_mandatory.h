/** Copyright (C) 2008-2013 Josh Ventura
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

#include "Universal_System/scalar.h"
#include "Collision_Systems/collision_types.h"

namespace enigma
{
  
  class Sprite;

  // This function fetches a collision mask from the collision system for a single subimage.
  // Examples of possible collision masks include bitmasks and polygon meshes.
  // It is called for every subimage of every sprite loaded.
  // The spr argument is the sprite containing the subimage,
  // the input_data argument contains the collision data for the subimage,
  // and the collision_type argument indicates the format of the collision data
  // and how it should be used.
  void *get_collision_mask(const Sprite& spr, void* input_data, collision_type ct);

  // This function frees a collision mask fetched using "get_collision_mask".
  // It is used to clean up on game termination.
  void free_collision_mask(void* mask);

  #ifdef ENIGMA_COLLISIONS_OBJECT_H
    // This function will be invoked each collision event to obtain a pointer to any
    // instance being collided with. It is expected to return NULL for no collision, or
    // an object_basic* pointing to the first instance found.
    object_basic *place_meeting_inst(cs_scalar x, cs_scalar y, int object);
  #endif
}
