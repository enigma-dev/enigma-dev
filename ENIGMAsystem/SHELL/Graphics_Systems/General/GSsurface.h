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

#ifndef _SURFSTRUCT__H
#define _SURFSTRUCT__H

#include <string>
#include "OpenGLHeaders.h"
using std::string;

namespace enigma
{
  struct surface
  {
    GLuint tex, fbo;
    int width, height;
  };
}

namespace enigma_user
{

int surface_create_msaa(int width, int height, int samples);
int surface_create(int width, int height);
void surface_set_target(int id);
void surface_reset_target();
void surface_free(int id);
bool surface_exists(int id);
void draw_surface(int id, float x, float y);
void draw_surface_stretched(int id, float x, float y, float w, float h);
void draw_surface_part(int id, float left, float top, float width, float height, float x, float y);
void draw_surface_tiled(int id, float x, float y);
void draw_surface_tiled_area(int id, float x, float y, float x1, float y1, double x2,double y2);
void draw_surface_ext(int id,float x, float y,float xscale, float yscale, double rot, int color, double alpha);
void draw_surface_stretched_ext(int id, float x, float y, float w, float h, int color, double alpha);
void draw_surface_part_ext(int id, float left, float top, float width, float height, float x, float y, float xscale, float yscale, int color, double alpha);
void draw_surface_tiled_ext(int id, float x, float y, float xscale, float yscale, int color, double alpha);
void draw_surface_tiled_area_ext(int id, float x, float y, float x1, float y1, double x2, double y2, float xscale, float yscale, int color, double alpha);
void draw_surface_general(int id, float left, float top, float width, float height, float x, float y, float xscale, float yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4);
int surface_get_bound();
int surface_get_texture(int id);
int surface_get_width(int id);
int surface_get_height(int id);
int surface_getpixel(int id, int x, int y);
int surface_getpixel_alpha(int id, int x, int y);
int surface_save(int id, string filename);
int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h);
void surface_copy(int destination,float x, float y,int source);
void surface_copy_part(int destination, float x, float y, int source ,int xs, int ys, int ws, int hs);
int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
bool surface_is_supported();
#define surface_destroy surface_free

}

#endif
