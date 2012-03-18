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
  @file  graphics_object.cpp
  @brief Adds a graphics-related tier following the planar tier.
*/

#include <math.h>
#include "depth_draw.h"
#include "graphics_object.h"

namespace enigma
{
  object_graphics::object_graphics() {}
  object_graphics::object_graphics(unsigned _x, int _y): object_planar(_x,_y) {}
  object_graphics::~object_graphics() {};
  
  INTERCEPT_DEFAULT_COPY(enigma::depthv);
  void depthv::function() {
    rval.d = floor(rval.d);
    drawing_depths[rval.d].draw_events->unlink(myiter);
    inst_iter* mynewiter = drawing_depths[rval.d].draw_events->add_inst(myiter->inst);
     if (instance_event_iterator == myiter)
       instance_event_iterator = myiter->prev;
     myiter = mynewiter;
  }
  void depthv::init(double d,object_basic* who) {
    myiter = drawing_depths[rval.d = floor(d)].draw_events->add_inst(who);
  }
  void depthv::remove() {
     drawing_depths[rval.d].draw_events->unlink(myiter);
     myiter = NULL;
  }
  depthv::~depthv() {}
  
  
  int object_graphics::$sprite_width()  const { return sprite_index == -1? 0 : sprite_get_width(sprite_index); }
  int object_graphics::$sprite_height() const { return sprite_index == -1? 0 : sprite_get_width(sprite_index); }
}
