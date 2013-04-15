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

/** description
@param backId The index of the background asset.
@return
**/
int background_get_texture(int backId);
/** description
@param backId The index of the background asset.
@return
**/
int background_get_width(int backId);
/** description
@param backId The index of the background asset.
@return
**/
int background_get_height(int backId);
/** description
@param backId The index of the background asset.
@return
**/
double background_get_texture_width_factor(int backId);
/** description
@param backId The index of the background asset.
@return
**/
double background_get_texture_height_factor(int backId);
/** description
@param back The index of the background asset.
@param x
@param y
**/
void draw_background(int back, double x, double y);
/** description
@param back The index of the background asset.
@param x
@param y
@param w
@param h
**/
void draw_background_stretched(int back, double x, double y, double w, double h);
/** description
@param back The index of the background asset.
@param left
@param top
@param width
@param height
@param x
@param y
**/
void draw_background_part(int back,double left,double top,double width,double height,double x,double y);
/** description
@param back The index of the background asset.
@param x
@param y
**/
void draw_background_tiled(int back,double x,double y);
/** description
@param back The index of the background asset.
@param x
@param y
@param x1
@param y1
@param x2
@param y2
**/
void draw_background_tiled_area(int back,double x,double y,double x1,double y1,double x2,double y2);
/** description
@param back The index of the background asset.
@param x
@param y
@param xscale
@param yscale
@param rot
@param color
@param alpha
**/
void draw_background_ext(int back,double x,double y,double xscale,double yscale,double rot,int color,double alpha);
/** description
@param back The index of the background asset.
@param x
@param y
@param w
@param h
@param color
@param alpha
**/
void draw_background_stretched_ext(int back,double x,double y,double w,double h,int color,double alpha);
/** description
@param back The index of the background asset.
@param left
@param top
@param width
@param height
@param x
@param y
@param xscale
@param yscale
@param color
@param alpha
**/
void draw_background_part_ext(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,int color,double alpha);
/** description
@param back The index of the background asset.
@param x
@param y
@param xscale
@param yscale
@param color
@param alpha
**/
void draw_background_tiled_ext(int back,double x,double y,double xscale,double yscale,int color,double alpha);
/** description
@param back The index of the background asset.
@param x
@param y
@param x1
@param y1
@param x2
@param y2
@param xscale
@param yscale
@param color
@param alpha
**/
void draw_background_tiled_area_ext(int back,double x,double y,double x1,double y1,double x2,double y2, double xscale, double yscale, int color, double alpha);
/** description
@param back The index of the background asset.
@param left
@param top
@param width
@param height
@param x
@param y
@param xscale
@param yscale
@param rot
@param c1
@param c2
@param c3
@param c4
@param a1
@param a2
@param a3
@param a4
**/
void draw_background_general(int back,double left,double top,double width,double height,double x,double y,double xscale,double yscale,double rot,int c1,int c2,int c3,int c4,double a1,double a2,double a3,double a4);
