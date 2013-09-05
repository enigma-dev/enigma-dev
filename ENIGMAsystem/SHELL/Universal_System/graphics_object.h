/** Copyright (C) 2010-2011 Josh Ventura
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
  @file    graphics_object.h
  @summary Adds a graphics-related tier following the planar tier.
*/

#ifndef _GRAPHICS_OBJECT_H
#define _GRAPHICS_OBJECT_H

#ifndef INCLUDED_FROM_SHELLMAIN
  #include "var4.h"
  #include "spritestruct.h"
  #include <cstdlib>
  #include <cmath>
#endif

#include "planar_object.h"
#include "multifunction_variant.h"

namespace enigma
{
  extern bool gui_used;
  struct depthv: multifunction_variant {
    INHERIT_OPERATORS(depthv)
    struct inst_iter *myiter;
    void function(variant oldval);
    void init(double depth, object_basic* who);
    void remove();
    ~depthv();
  };
  struct object_graphics: object_planar
  {
    //Sprites: these are mostly for higher tiers...
      int sprite_index;
      double image_index;
      double image_speed;

      //Depth
      enigma::depthv  depth;
      bool visible;

    //Transformations: these are mostly for higher tiers...
      double image_xscale;
      double image_yscale;
      double image_angle;

    //Accessors
      #ifdef JUST_DEFINE_IT_RUN
        int sprite_width, sprite_height;
        int sprite_xoffset, sprite_yoffset;
        int image_number;
      #else
        int $sprite_width() const;
        int $sprite_height() const;
        int $sprite_xoffset() const;
        int $sprite_yoffset() const;
        int $image_number() const;
        #define sprite_width $sprite_width()
        #define sprite_height $sprite_height()
        #define sprite_xoffset $sprite_xoffset()
        #define sprite_yoffset $sprite_yoffset()
        #define image_number $image_number()
      #endif

    //Constructors
      object_graphics();
      object_graphics(unsigned x, int y);
      virtual ~object_graphics();
  };
}

#endif
