/** Copyright (C) 2011 Josh Ventura, Dave "biggoron", Harijs Grînbergs
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

int surface_create(int width, int height);
void surface_set_target(int id);
void surface_reset_target();
void surface_free(int id);
bool surface_exists(int id);
void draw_surface(int id, double x, double y);
void draw_surface_stretched(int id, double x, double y, double w, double h);
void draw_surface_part(int id,double left,double top,double width,double height,double x,double y);
void draw_surface_tiled(int id,double x,double y);
void draw_surface_tiled_area(int id,double x,double y,double x1,double y1,double x2,double y2);
void draw_surface_ext(int id,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
void draw_surface_stretched_ext(int id,double x,double y,double w,double h,int color,double alpha);
void draw_surface_part_ext(int id,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_surface_tiled_ext(int id,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_surface_tiled_area_ext(int id,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha);
void draw_surface_general(int id, double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4);
int surface_get_bound();
int surface_get_texture(int id);
int surface_get_width(int id);
int surface_get_height(int id);
int surface_getpixel(int id, int x, int y);
int surface_getpixel_alpha(int id, int x, int y);
int surface_save(int id, string filename);
int surface_save_part(int id, string filename,unsigned x,unsigned y,unsigned w,unsigned h);
void surface_copy(int destination,double x,double y,int source);
void surface_copy_part(int destination,double x,double y,int source,int xs,int ys,int ws,int hs);

int sprite_create_from_surface(int id,int x,int y,int w,int h,bool removeback,bool smooth,int xorig,int yorig);

bool surface_is_supported();
