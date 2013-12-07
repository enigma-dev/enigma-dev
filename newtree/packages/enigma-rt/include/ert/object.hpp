/******************************************************************************

 ENIGMA
 Copyright (C) 2008-2013 Enigma Strike Force

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

******************************************************************************/

#ifndef ERT_OBJECT_HPP_
#define ERT_OBJECT_HPP_

#include "ert/real.hpp"

namespace ert {
  struct object {
    object(unsigned long id, real_t x, real_t y);
    virtual ~object() = 0;
    
    const unsigned long id;
    virtual unsigned long object_index() = 0;
    
    const real_t xstart;
    const real_t ystart;
    
    real_t solid;
    real_t visible;
    real_t persistent;
    real_t depth;
    
    // TODO: alarm events require variant array support
    
    real_t sprite_index;
    real_t sprite_width;
    real_t sprite_height;
    real_t sprite_xoffset;
    real_t sprite_yoffset;
    real_t image_alpha;
    real_t image_angle;
    real_t image_blend;
    real_t image_index;
    real_t image_number;
    real_t image_speed;
    real_t image_xscale;
    real_t image_yscale;
    
    real_t mask_index;
    real_t bbox_bottom;
    real_t bbox_left;
    real_t bbox_right;
    real_t bbox_top;
    
    real_t direction;
    real_t friction;
    real_t gravity;
    real_t gravity_direction;
    real_t hspeed;
    real_t vspeed;
    real_t speed;
    
    real_t x;
    real_t y;
    real_t xprevious;
    real_t yprevious;
  };
}

#endif // ERT_OBJECT_HPP_
