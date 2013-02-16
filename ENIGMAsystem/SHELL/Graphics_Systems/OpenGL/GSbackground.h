/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>, Josh Ventura        **
 **  Copyright (C) 2013 Robert B. Colton                                         **
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
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

enum {
  tx_none,
  tx_nearest,
  tx_bilinear,
  tx_trilinear
};

void draw_background(int back, double x, double y);
void draw_background_stretched(int back, double x, double y, double w, double h);
void draw_background_part(int back,double left,double top,double width,double height,double x,double y);
void draw_background_tiled(int back,double x,double y);
void draw_background_tiled_area(int back,double x,double y,double x1,double y1,double x2,double y2);
void draw_background_ext(int back,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
void draw_background_stretched_ext(int back,double x,double y,double w,double h,int color,double alpha);
void draw_background_part_ext(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha);
void draw_background_tiled_area_ext(int back,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha);
void draw_background_general(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4);

int background_get_texture(int backId);
int background_get_width(int backId);
int background_get_height(int backId);
double background_get_texture_width_factor(int backId);
double background_get_texture_height_factor(int backId);
void texture_set_interpolation(int enable);
bool texture_get_interpolation();
double texture_get_width(int texid);
double texture_get_height(int texid);
int texture_get_pixwidth(int texid);
int texture_get_pixheight(int texid);
void texture_set_blending(bool enable);
void texture_set_repeat(bool repeat);
void texture_set_repeat(int texid, bool repeat);
void texture_set_repeat(int texid, bool repeatu, bool repeatv, bool repeatw);
void texture_preload(int texid);
void texture_set_priority(int texid, double prio);
void texture_set_border(int texid, int r, int g, int b, double a);
void texture_mipmapping_filter(int texid, int enable);
void texture_mipmapping_generate(int texid, int levels);
bool  texture_anisotropy_supported();
float texture_anisotropy_maxlevel();
void  texture_anisotropy_filter(int texid, float levels);
bool texture_multitexture_supported();
void texture_multitexture_enable(bool enable);
