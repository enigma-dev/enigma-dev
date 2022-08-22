/** Copyright (C) 2010-2013 Alasdair Morrison, Robert B. Colton
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
#ifndef ENIGMA_GSBACKGROUND_H
#define ENIGMA_GSBACKGROUND_H

#include "GScolors.h"

#include "Universal_System/scalar.h"

#if GM_COMPATIBILITY_VERSION <= 81
#  define DEFAULT_ALPHA 1
#else
#  define DEFAULT_ALPHA draw_get_alpha()
#endif

namespace enigma_user
{
  double background_get_texture_width_factor(int backId);
  double background_get_texture_height_factor(int backId);
  void draw_background(int back, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_tiled(int back, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_tiled_area(int back,gs_scalar x, gs_scalar y,gs_scalar x1, gs_scalar y1,float x2,float y2, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale = 1, gs_scalar yscale = 1, double rot = 0, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int color, gs_scalar alpha);
  void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale = 1, gs_scalar yscale = 1,int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale = 1, gs_scalar yscale = 1, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA, bool htiled = true, bool vtiled = true);
  void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale = 1, gs_scalar yscale = 1, int color = 16777215, gs_scalar alpha = DEFAULT_ALPHA);
  void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha);

  void d3d_draw_background(int back, gs_scalar x, gs_scalar y, gs_scalar z = 0);

  int background_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload = true);
}

#undef DEFAULT_ALPHA
#endif
