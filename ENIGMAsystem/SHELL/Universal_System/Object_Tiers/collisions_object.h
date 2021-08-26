/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2021 Nabeel Danish                                            **
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
  @summary Adds a collisions-related tier following the transform tier.
*/

#ifndef ENIGMA_COLLISIONS_OBJECT_H
#define ENIGMA_COLLISIONS_OBJECT_H

#include "transform_object.h"
#include "Universal_System/Resources/sprites_internal.h" //bbox_rect

namespace enigma
{
  struct object_collisions: object_transform
  {
    // Bit Mask
      int mask_index;
      bool solid;

    // Polygon related attributes
    int polygon_index;
    gs_scalar polygon_xscale, polygon_yscale;
    gs_scalar polygon_angle;

    //Bounding box
      #ifdef JUST_DEFINE_IT_RUN
        int bbox_left, bbox_right, bbox_top, bbox_bottom;
      #else
        int $bbox_left()   const;
        int $bbox_right()  const;
        int $bbox_top()    const;
        int $bbox_bottom() const;
        const BoundingBox $bbox_relative() const;
        const BoundingBox& $bbox() const;
        #define bbox_left   $bbox_left()
        #define bbox_right  $bbox_right()
        #define bbox_top    $bbox_top()
        #define bbox_bottom $bbox_bottom()
      #endif
      
    //Constructors
      object_collisions();
      object_collisions(unsigned, int);
      virtual ~object_collisions();
  };
} //namespace enigma

#endif //ENIGMA_COLLISIONS_OBJECT_H
