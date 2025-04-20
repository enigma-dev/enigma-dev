/** Copyright (C) 2010-2011 Josh Ventura
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
  @file    graphics_object.h
  @summary Adds a graphics-related tier following the timelines tier.
*/

#ifndef ENIGMA_GRAPHICS_OBJECT_H
#define ENIGMA_GRAPHICS_OBJECT_H

#ifndef INCLUDED_FROM_SHELLMAIN
  #include "Universal_System/var4.h"
  #include "Universal_System/Resources/sprites.h"
#endif

#include "Universal_System/scalar.h"
#include "timelines_object.h"
#include "Universal_System/multifunction_variant.h"

namespace enigma
{
  extern long gui_used;
  struct depthv: multifunction_variant<depthv> {
    INHERIT_OPERATORS(depthv)
    struct inst_iter *myiter;
    void function(const evariant &oldval);
    void init(gs_scalar depth, object_basic* who);
    void remove();
    depthv();
    ~depthv();
  };
  struct image_singlev: multifunction_variant<image_singlev> {
    INHERIT_OPERATORS(image_singlev)
    gs_scalar *image_index, *image_speed;
    void function(const evariant &oldval);
    image_singlev(): multifunction_variant<image_singlev>(-1) {}
  };
  struct object_graphics: object_timelines
  {
    //Sprites: these are mostly for higher tiers...
      int sprite_index;
      gs_scalar image_index;
      gs_scalar image_speed;
      image_singlev image_single;

      //Depth
      enigma::depthv  depth;
      bool visible;

    //Transformations: these are mostly for higher tiers...
      gs_scalar image_xscale;
      gs_scalar image_yscale;
      gs_scalar image_angle;

      virtual evariant myevent_draw();
      virtual bool myevent_draw_subcheck();
      virtual evariant myevent_drawgui();
      virtual bool myevent_drawgui_subcheck();
      virtual evariant myevent_drawresize();

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
} //namespace enigma

#endif
