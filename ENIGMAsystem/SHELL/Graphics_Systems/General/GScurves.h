/** Copyright (C) 2013 Harijs Grinbergs
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

#include "Universal_System/scalar.h"

namespace enigma_user
{

/** description
**/
void draw_bezier_quadratic(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3);
/** description
**/
void draw_bezier_quadratic_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3,int c1,int c2,float a1,float a2);

/** description
**/
void draw_bezier_cubic(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
/** description
**/
void draw_bezier_cubic_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2);
/** description
**/
void draw_set_curve_mode(int mode);
/** description
**/
void draw_set_curve_detail(int detail);
/** description
**/
void draw_set_curve_width(int width);
/** description
@return
**/
float draw_bezier_quadratic_x(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, float t);
/** description
@return
**/
float draw_bezier_quadratic_y(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, float t);
/** description
@return
**/
float draw_bezier_cubic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t);
/** description
@return
**/
float draw_bezier_cubic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t);
/** description
**/
void draw_bezier_quadratic_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, float a1, float a2);
/** description
**/
void draw_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
**/
void draw_spline2c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline2c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
**/
void draw_spline3(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline3_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param x5
@param y5
**/
void draw_spline3c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param x5
@param y5
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline3c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
**/
void draw_spline4(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline4_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param x5
@param y5
@param x6
@param y6
**/
void draw_spline4c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param x5
@param y5
@param x6
@param y6
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline4c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
**/
void draw_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
@param x4
@param y4
@param c1
@param c2
@param a1
@param a2
**/
void draw_spline_part_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, float a1, float a2);
/** description
@param mode
**/
void draw_spline_begin(int mode);
/** description
@param x
@param y
@return
**/
int draw_spline_vertex(gs_scalar x, gs_scalar y);
/** description
@param x
@param y
@param col
@param alpha
@return
**/
int draw_spline_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha);
/** description
**/
void draw_spline_end();
/** description
**/
void draw_bezier_quadratic_spline_end();
/** description
@return
**/
int draw_spline_optimized_end();

}

