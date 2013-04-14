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

void draw_bezier_quadratic(float x1,float y1,float x2,float y2,float x3,float y3);
void draw_bezier_quadratic_color(float x1,float y1,float x2,float y2,float x3,float y3,int c1,int c2,float a1,float a2);

void draw_bezier_cubic(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4);
void draw_bezier_cubic_color(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_set_curve_mode(int mode);
void draw_set_curve_detail(int detail);
void draw_set_curve_width(int width);
float draw_bezier_quadratic_x(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_quadratic_y(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_cubic_x(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
float draw_bezier_cubic_y(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
void draw_bezier_quadratic_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline2c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline2c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline3(float x1, float y1, float x2, float y2, float x3, float y3);
void draw_spline3_color(float x1, float y1, float x2, float y2, float x3, float y3, int c1, int c2, float a1, float a2);
void draw_spline3c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5);
void draw_spline3c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, int c1, int c2, float a1, float a2);
void draw_spline4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline4_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline4c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6);
void draw_spline4c_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6, int c1, int c2, float a1, float a2);
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline_part_color(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline_begin(int mode);
int draw_spline_vertex(float x, float y);
int draw_spline_vertex_color(float x, float y, int col, float alpha);
void draw_spline_end();
void draw_bezier_quadratic_spline_end();
int draw_spline_optimized_end();
