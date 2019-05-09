/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2014 Seth N. Hetu
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
  @file  graphics_object.cpp
  @brief Adds a graphics-related tier following the planar tier.
*/

#include "depth_draw.h"
#include "graphics_object.h"

#include <math.h>
#include <floatcomp.h>

namespace enigma
{
  object_graphics::object_graphics() {
    image_single.image_index = &image_index;
    image_single.image_speed = &image_speed;
  }
  object_graphics::object_graphics(unsigned _x, int _y): object_timelines(_x,_y) {
    image_single.image_index = &image_index;
    image_single.image_speed = &image_speed;
  }
  object_graphics::~object_graphics() {}
  
  variant object_graphics::myevent_draw()      { return 0; }
  bool object_graphics::myevent_draw_subcheck() { return 0; }
  variant object_graphics::myevent_drawgui()   { return 0; }
  bool object_graphics::myevent_drawgui_subcheck() { return 0; }
  variant object_graphics::myevent_drawresize()   { return 0; }

  void depthv::function(const variant &oldval) {
    if (!myiter) { return; }

    rval.d = floor(rval.d);
    if (fequal(oldval.rval.d, rval.d)) return;

    std::map<int,std::pair<double,double> >::iterator it = id_to_currentnextdepth.find(myiter->inst->id);
    if (it == id_to_currentnextdepth.end()) { // Insert a request to change in depth.
      id_to_currentnextdepth.insert(std::pair<int,std::pair<double,double> >(myiter->inst->id, std::pair<double,double>(oldval.rval.d,rval.d)));
    }
    else { // Update the request to change in depth.
      (*it).second.second = rval.d;
    }
  }
  void depthv::init(gs_scalar d,object_basic* who) {
    myiter = drawing_depths[rval.d = floor(d)].draw_events->add_inst(who);
  }
  void depthv::remove() {
    std::map<int,std::pair<double,double> >::iterator it = id_to_currentnextdepth.find(myiter->inst->id);
    if (it == id_to_currentnextdepth.end()) { // Local value is valid, use it.
      drawing_depths[rval.d].draw_events->unlink(myiter);
    }
    else { // Local value is invalid, use the one in the map.
      drawing_depths[(*it).second.first].draw_events->unlink(myiter);
    }
    myiter = NULL;
  }

  depthv::depthv() : multifunction_variant<depthv>(0), myiter(0) {}
  depthv::~depthv() {}

  void image_singlev::function(const variant&) {
    if (rval.d == -1) {
      *image_speed = 1;
    } else {
      *image_index = rval.d;
      *image_speed = 0;
    }
  }

  int object_graphics::$sprite_width()  const { return sprite_index == -1? 0 : enigma_user::sprite_get_width(sprite_index)*image_xscale; }
  int object_graphics::$sprite_height() const { return sprite_index == -1? 0 : enigma_user::sprite_get_height(sprite_index)*image_yscale; }
  int object_graphics::$sprite_xoffset() const { return sprite_index == -1? 0 : enigma_user::sprite_get_xoffset(sprite_index)*image_xscale; }
  int object_graphics::$sprite_yoffset() const { return sprite_index == -1? 0 : enigma_user::sprite_get_yoffset(sprite_index)*image_yscale; }
  int object_graphics::$image_number() const { return sprite_index == -1? 0 : enigma_user::sprite_get_number(sprite_index); }
}
