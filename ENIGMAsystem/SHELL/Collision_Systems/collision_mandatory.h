/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "Universal_System/spritestruct.h"
#include "Collision_Systems/collision_types.h"
#include "Collision_Systems/box2d.h"

namespace enigma
{

  // This function fetches a collision mask from the collision system for a single subimage.
  // Examples of possible collision masks include bitmasks and polygon meshes.
  // It is called for every subimage of every sprite loaded.
  // The spr argument is the sprite containing the subimage,
  // the input_data argument contains the collision data for the subimage,
  // and the collision_type argument indicates the format of the collision data
  // and how it should be used.
  void *get_collision_mask(sprite* spr, unsigned char* input_data, collision_type ct);

  // This function frees a collision mask fetched using "get_collision_mask".
  // It is used to clean up on game termination.
  void free_collision_mask(void* mask);
  
  #ifdef _COLLISIONS_OBJECT_H
    // This function will be invoked each collision event to obtain a pointer to any
    // instance being collided with. It is expected to return NULL for no collision, or
    // an object_basic* pointing to the first instance found.
    object_basic *place_meeting_inst(double x, double y, int object);
  #endif
}

