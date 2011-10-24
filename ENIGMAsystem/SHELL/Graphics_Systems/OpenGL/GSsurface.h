/********************************************************************************\
**                                                                              **
**  Copyright (C) 2011 Josh Ventura, Dave "biggoron", Harijs Grînbergs          **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/
int surface_create(int width, int height);
int surface_set_target(int id);
int surface_reset_target();
int surface_destroy(int id);
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
int surface_get_texture(int id);
int surface_get_width(int id);
int surface_get_height(int id);
