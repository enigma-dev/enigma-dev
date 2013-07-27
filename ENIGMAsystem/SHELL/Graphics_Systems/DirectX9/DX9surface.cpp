/** Copyright (C) 2013 Robert B. Colton
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

#include "../General/DirectXHeaders.h"
using namespace std;
#include <cstddef>
#include <iostream>
#include <math.h>

#include "../General/DXbinding.h"
#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
extern int room_width, room_height/*, sprite_idmax*/;
}
#include "../General/GSsurface.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_surface(surf,id)\
    if (id < 0 or id >= enigma::surface_max or !enigma::surface_array[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return;\
    }\
    enigma::surface* surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    if (id < 0 or size_t(id) >= enigma::surface_max or !enigma::surface_array[id]) {\
      show_error("Attempting to use non-existing surface " + toString(id), false);\
      return r;\
    }\
    enigma::surface* surf = enigma::surface_array[id];
#else
  #define get_surface(surf,id)\
    enigma::surface* surf = enigma::surface_array[id];
  #define get_surfacev(surf,id,r)\
    enigma::surface* surf = enigma::surface_array[id];
#endif

namespace enigma
{
  surface **surface_array;
  int surface_max=0;
}

namespace enigma_user
{

bool surface_is_supported()
{

}

int surface_create(int width, int height)
{

}

int surface_msaa_create(int width, int height, int levels)
{

}

void surface_set_target(int id)
{

}

void surface_reset_target(void)
{

}

void surface_free(int id)
{

}

bool surface_exists(int id)
{
    return !((id<0) or (id>enigma::surface_max) or (enigma::surface_array[id]==NULL));
}

void draw_surface(int id, gs_scalar x, gs_scalar y)
{

}

void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, float w, float h)
{

}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{

}

void draw_surface_tiled(int id, gs_scalar x, gs_scalar y)
{

}

void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2)
{

}

void draw_surface_ext(int id,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale,double rot,int color,double alpha)
{

}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, float w, float h, int color, double alpha)
{

}

void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale,int color, double alpha)
{

}

void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{

}

int surface_get_texture(int id)
{

}

int surface_get_width(int id)
{
    get_surfacev(surf,id,-1);
    return (surf->width);
}

int surface_get_height(int id)
{
    get_surfacev(surf,id,-1);
    return (surf->height);
}

int surface_getpixel(int id, int x, int y)
{

}

int surface_getpixel_alpha(int id, int x, int y)
{

}

int surface_get_bound()
{

}

}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef DX_BGR
  #define DX_BGR 0x80E0
#endif

#include "Universal_System/estring.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{

}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{

}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{

}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{

}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{

}

}

