/********************************************************************************\
**                                                                              **
**  Copyright (C) 2010 Harijs Grînbergs                                         **
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

void draw_bezier_quadratic(float x1,float y1,float x2,float y2,float x3,float y3);
void draw_bezier_cubic(float x1,float y1,float x2,float y2,float x3,float y3, float x4, float y4);
void draw_set_curve_color(int c1, int c2);
void draw_set_curve_alpha(float a1, float a2);
void draw_set_curve_mode(int mode);
void draw_set_curve_detail(int detail);
void draw_set_curve_width(int width);
float draw_bezier_quadratic_x(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_quadratic_y(float x1,float y1,float x2,float y2,float x3,float y3, float t);
float draw_bezier_cubic_x(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
float draw_bezier_cubic_y(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float t);
void draw_spline_part(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int c1, int c2, float a1, float a2);
void draw_spline2c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline3(float x1, float y1, float x2, float y2, float x3, float y3);
void draw_spline3c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5);
void draw_spline4(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
void draw_spline4c(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float x5, float y5, float x6, float y6);
void draw_spline_begin(int mode);
int draw_spline_vertex(float x, float y);
int draw_spline_vertex_color(float x, float y, int col, float alpha);
void draw_spline_end();
int draw_spline_optimized_end();
