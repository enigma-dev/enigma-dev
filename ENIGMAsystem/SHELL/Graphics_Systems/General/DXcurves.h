/** Copyright (C) 2013 Robert B. Colton
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

namespace enigma_user
{

/** description
**/
void draw_bezier_quadratic(float x1,float y1,float x2,float y2,float x3,float y3);
/** description
**/
void draw_bezier_quadratic_color(float x1,float y1,float x2,float y2,float x3,float y3,int c1,int c2,float a1,float a2);

/** description
**/
void draw_bezier_cubic(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4);
/** description
**/
void draw_bezier_cubic_color(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4, int c1, int c2, float a1, float a2);
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
float draw_bezier_quadratic_x(float x1,float y1,float x2,float y2,float x3,float y3, float t);
/** description
@return
**/
float draw_bezier_quadratic_y(float x1,float y1,float x2,float y2,float x3,float y3, float t);
/** description
@return
**/
float draw_bezier_cubic_x(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
/** description
@return
**/
float draw_bezier_cubic_y(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
/** description
**/
void draw_bezier_quadratic_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
/** description
**/
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
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
void draw_spline2c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
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
void draw_spline2c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
/** description
@param x1
@param y1
@param x2
@param y2
@param x3
@param y3
**/
void draw_spline3(float x1, float y1, float x2, float y2, float x3, float y3);
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
void draw_spline3_color(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
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
void draw_spline3c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5);
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
void draw_spline3c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, int c1, int c2, float a1, float a2);
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
void draw_spline4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
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
void draw_spline4_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
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
void draw_spline4c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6);
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
void draw_spline4c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int c1, int c2, float a1, float a2);
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
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
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
void draw_spline_part_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
/** description
@param mode
**/
void draw_spline_begin(int mode);
/** description
@param x
@param y
@return
**/
int draw_spline_vertex(float x, float y);
/** description
@param x
@param y
@param col
@param alpha
@return
**/
int draw_spline_vertex_color(float x, float y, int col, float alpha);
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

