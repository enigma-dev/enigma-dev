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

#ifndef ENIGMA_GSSPRITE__H
#define ENIGMA_GSSPRITE__H

#include "Universal_System/scalar.h"
#include "Universal_System/spritestruct.h"

namespace enigma_user
{

void draw_sprite(int spr, int subimg, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_pos(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha = 1);
void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar alpha = 1);
void draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale = 1, gs_scalar yscale = 1, double rot = 0, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int blend = 16777215, gs_scalar alpha = 1);
void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale = 1, gs_scalar yscale = 1, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color = 16777215, gs_scalar alpha = 1);
void draw_sprite_padded(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color = 16777215, gs_scalar alpha = 1.0);

void d3d_draw_sprite(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar z = 0);

int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig);
int sprite_create_from_screen(int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig);
void sprite_add_from_screen(int id, int x, int y, int w, int h, bool removeback, bool smooth);
}

#endif
