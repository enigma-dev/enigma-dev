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

/**
  @file    graphics_object.h
  @summary Adds a graphics-related tier following the planar tier.
*/

#ifndef _GRAPHICS_OBJECT_H
#define _GRAPHICS_OBJECT_H

#include "../../Universal_System/planar_object.h"
namespace enigma
{
  struct object_graphics: object_planar
  {
    //Sprites: these are mostly for higher tiers...
      int sprite_index;
      int image_index;
    
    //Depth
      int  depth;
      bool visible;
    
    //Transformations: these are mostly for higher tiers...
      int    sprite_xoffset;
      int    sprite_yoffset;
      double image_xscale;
      double image_yscale;
      double image_angle;
    
    //Constructors
      object_graphics();
      object_graphics(unsigned x, int y);
      virtual ~object_graphics();
  };
}

#endif
