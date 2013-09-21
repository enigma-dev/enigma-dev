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

#ifndef _GL_SPRITE__H
#define _GL_SPRITE__H

#include "Universal_System/scalar.h"
#include "Universal_System/spritestruct.h"

namespace enigma_user
{

void draw_sprite(int spr, int subimg, gs_scalar x, gs_scalar y);
void draw_sprite(int spr, int subimg, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar alpha);
void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei);
//enigma::4args int draw_sprite_tiled(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y)
void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y);
void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y);
void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha);
//draw_sprite_tiled_ext(sprite,subimg,x,y,xscale,yscale,color,alpha)
void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar wid, gs_scalar hei, int blend, gs_scalar alpha);
void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha);
void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar wid, gs_scalar hei, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4);

void draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y);
void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha);

bool sprite_exists(int sprite);

}

#endif
