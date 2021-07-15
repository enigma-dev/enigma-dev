/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_GSPOLYGON_H
#define ENIGMA_GSPOLYGON_H

#include "GScolors.h"
#include "Universal_System/scalar.h"

#if GM_COMPATIBILITY_VERSION <= 81
#  define DEFAULT_ALPHA 1
#else
#  define DEFAULT_ALPHA draw_get_alpha()
#endif

namespace enigma_user
{
  void draw_polygon(int polygon_id, gs_scalar x, gs_scalar y);
  void draw_polygon_bbox(int polygon_id, gs_scalar x, gs_scalar y);
  void draw_polygon_color(int polygon_id, gs_scalar x, gs_scalar y, int color);
  void draw_polygon_bbox_color(int polygon_id, gs_scalar x, gs_scalar y, int color);
}

#undef DEFAULT_ALPHA
#endif
