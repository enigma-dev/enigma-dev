/** Copyright (C) 2008-2011 Josh Ventura
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

#include "Universal_System/spritestruct.h"

void draw_sprite(int spr,int subimg,double x,double y);
void draw_sprite_stretched(int spr,int subimg,double x,double y,double w,double h);
//enigma::4args int draw_sprite_tiled(ARG sprite,ARG2 subimg,ARG3 x,ARG4 y)
void draw_sprite_part(int spr,int subimg,double left,double top,double width,double height,double x,double y);
void draw_sprite_part_offset(int spr,int subimg,double left,double top,double width,double height,double x,double y);
void draw_sprite_ext(int spr,int subimg,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
//draw_sprite_tiled_ext(sprite,subimg,x,y,xscale,yscale,color,alpha)
void draw_sprite_stretched_ext(int spr,int subimg,double x,double y,double w,double h, int blend, double alpha);
void draw_sprite_part_ext(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_sprite_general(int spr,int subimg,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1, double a2, double a3, double a4);

void draw_sprite_tiled(int spr,int subimg,double x,double y);
void draw_sprite_tiled_ext(int spr,int subimg,double x,double y,double xscale,double yscale,int color,double alpha);

bool sprite_exists(int sprite);
