/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Dave "biggoron", Harijs Grinbergs
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

#ifndef ENIGMA_GSSURFACE_H
#define ENIGMA_GSSURFACE_H

#include "GScolors.h"

#include "Universal_System/scalar.h"

#include <string>
using std::string;

namespace enigma_user
{

  int surface_create_msaa(int width, int height, int samples);
  int surface_create(int width, int height, bool depthbuffer = false, bool stencilbuffer = false, bool writeonly = true);
  int surface_get_target();
  void surface_set_target(int id);
  void surface_reset_target();
  void surface_free(int id);
  bool surface_exists(int id);
  void draw_surface(int id, gs_scalar x, gs_scalar y, int color=c_white, gs_scalar alpha=1.0);
  void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int color=c_white, gs_scalar alpha=1.0);
  void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, int color=c_white, gs_scalar alpha=1.0);
  void draw_surface_tiled(int id, gs_scalar x, gs_scalar y, int color=c_white, gs_scalar alpha=1.0);
  void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color=c_white, gs_scalar alpha=1.0);
  void draw_surface_ext(int id,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha);
  void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int color, gs_scalar alpha);
  void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha);
  void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha);
  void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha);
  void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha);
  int surface_get_texture(int id);
  int surface_get_depth_texture(int id);
  int surface_get_width(int id);
  int surface_get_height(int id);
  int surface_getpixel(int id, int x, int y);
  int surface_getpixel_ext(int id, int x, int y);
  int surface_getpixel_alpha(int id, int x, int y);

  #define surface_get_pixel        surface_getpixel
  #define surface_get_pixel_ext    surface_getpixel_ext
  #define surface_get_pixel_alpha  surface_getpixel_alpha

  int surface_save(int id, string filename);
  int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h);
  void surface_copy(int destination,gs_scalar x, gs_scalar y,int source);
  void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source ,int xs, int ys, int ws, int hs);

  int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload=true);
  int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
  int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig);
  void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth);

  bool surface_is_supported();
  #define surface_destroy surface_free

}

#endif
