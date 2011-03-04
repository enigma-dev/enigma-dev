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
  @file    graphics_object.cpp
  @summary Adds a graphics-related tier following the planar tier.
*/

#include "graphics_object.h"

namespace enigma
{
  object_graphics::object_graphics() {}
  object_graphics::object_graphics(unsigned x, int y): object_planar(x,y) {}
  object_graphics::~object_graphics() {};

  int object_graphics::bbox_left()
  {
    const l = -sprite_xoffset             * cosd(image_angle),
          t = -sprite_yoffset             * cosd(image_angle + 270),
          r = (width()  - sprite_xoffset) * cosd(image_angle + 180),
          b = (height() - sprite_xoffset) * cosd(image_angle + 90);
    return max(max(l, r), max(t, b));
  }

  int object_graphics::bbox_top()
  {
    const l = -sprite_xoffset             * sind(image_angle),
          t = -sprite_yoffset             * cosd(image_angle + 270),
          r = (width()  - sprite_xoffset) * cosd(image_angle + 180),
          b = (height() - sprite_xoffset) * cosd(image_angle + 90);
    return max(max(l, r), max(t, b));
  }

  int object_graphics::bbox_right()
  {
    const c = cosd(image_angle);
  }

  int object_graphics::bbox_bottom()
  {
    const s = sind(image_angle);
  }
}
