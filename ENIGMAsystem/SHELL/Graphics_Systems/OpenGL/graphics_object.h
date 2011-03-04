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
#include "../../Universal_System/mathnc.h"
#include "spritestruct.h"
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

    //Bounding boxes
      int width()  { return (sprite_index >= 0 ? spritestructarray[sprite_index]->width  : 0); }
      int height() { return (sprite_index >= 0 ? spritestructarray[sprite_index]->height : 0); }

      double bbox_left()
      {
        const l = -sprite_xoffset             * cosd(image_angle),
              t = -sprite_yoffset             * cosd(image_angle + 90.0),
              r = (width()  - sprite_xoffset) * cosd(image_angle + 180.0),
              b = (height() - sprite_xoffset) * cosd(image_angle + 270.0);
        return min(min(l, r), min(t, b));
      }

      // NOTE: sind(x + 180.0) == -sind(x)
      double bbox_top()
      {
        const l = -sprite_xoffset             * sind(image_angle + 90.0),
              t = -sprite_yoffset             * sind(image_angle + 180.0),
              r = (width()  - sprite_xoffset) * sind(image_angle + 270.0),
              b = (height() - sprite_xoffset) * sind(image_angle);
        return min(min(l, r), min(t, b));
      }

      double bbox_right()
      {
        const l = -sprite_xoffset             * cosd(image_angle + 180.0),
              t = -sprite_yoffset             * cosd(image_angle + 270.0),
              r = (width()  - sprite_xoffset) * cosd(image_angle),
              b = (height() - sprite_xoffset) * cosd(image_angle + 90.0);
        return max(max(l, r), max(t, b));
      }

      // NOTE: sind(x + 180.0) == -sind(x)
      double bbox_bottom()
      {
        const l = -sprite_xoffset             * sind(image_angle + 270.0),
              t = -sprite_yoffset             * sind(image_angle),
              r = (width()  - sprite_xoffset) * sind(image_angle + 90.0),
              b = (height() - sprite_xoffset) * sind(image_angle + 180.0);
        return max(max(l, r), max(t, b));
      }


    //Constructors
      object_graphics();
      object_graphics(unsigned x, int y);
      virtual ~object_graphics();
  };
}

#endif
