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
#include <floatcomp.h>

namespace enigma
{
  object_graphics::object_graphics() {}
  object_graphics::object_graphics(unsigned _x, int _y): object_planar(_x,_y) {}
  object_graphics::~object_graphics() {}

  INTERCEPT_DEFAULT_COPY(enigma::depthv)
  void depthv::function(variant oldval) {
    rval.d = floor(rval.d);
    if (fequal(oldval.rval.d, rval.d)) return;

    map<int,pair<double,double> >::iterator it = id_to_currentnextdepth.find(myiter->inst->id);
    if (it == id_to_currentnextdepth.end()) { // Insert a request to change in depth.
      id_to_currentnextdepth.insert(pair<int,pair<double,double> >(myiter->inst->id, pair<double,double>(oldval.rval.d,rval.d)));
    }
    else { // Update the request to change in depth.
      (*it).second.second = rval.d;
    }
  }
  void depthv::init(double d,object_basic* who) {
    myiter = drawing_depths[rval.d = floor(d)].draw_events->add_inst(who);
  }
  void depthv::remove() {
    map<int,pair<double,double> >::iterator it = id_to_currentnextdepth.find(myiter->inst->id);
    if (it == id_to_currentnextdepth.end()) { // Local value is valid, use it.
      drawing_depths[rval.d].draw_events->unlink(myiter);
    }
    else { // Local value is invalid, use the one in the map.
      drawing_depths[(*it).second.first].draw_events->unlink(myiter);
    }
    myiter = NULL;
  }
  depthv::~depthv() {}

  int object_graphics::$sprite_width()  const { return sprite_index == -1? 0 : enigma_user::sprite_get_width(sprite_index)*image_xscale; }
  int object_graphics::$sprite_height() const { return sprite_index == -1? 0 : enigma_user::sprite_get_height(sprite_index)*image_yscale; }
  int object_graphics::$sprite_xoffset() const { return sprite_index == -1? 0 : enigma_user::sprite_get_xoffset(sprite_index)*image_xscale; }
  int object_graphics::$sprite_yoffset() const { return sprite_index == -1? 0 : enigma_user::sprite_get_yoffset(sprite_index)*image_yscale; }
  int object_graphics::$image_number() const { return sprite_index == -1? 0 : enigma_user::sprite_get_number(sprite_index); }
}
