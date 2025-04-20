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

#ifndef ENIGMA_GSCURVES_H
#define ENIGMA_GSCURVES_H

#include "Universal_System/scalar.h"

namespace enigma_user
{
  void draw_bezier_quadratic(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3);
  void draw_bezier_quadratic_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3,int c1,int c2,gs_scalar a1,gs_scalar a2);
  void draw_bezier_cubic(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
  void draw_bezier_cubic_color(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_set_curve_mode(int mode);
  void draw_set_curve_detail(int detail);
  void draw_set_curve_width(gs_scalar width);

  gs_scalar draw_bezier_quadratic_x(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, float t);
  gs_scalar draw_bezier_quadratic_y(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2,gs_scalar x3, gs_scalar y3, float t);
  gs_scalar draw_bezier_cubic_x(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t);
  gs_scalar draw_bezier_cubic_y(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, float t);
  void draw_bezier_quadratic_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, gs_scalar a1, gs_scalar a2);

  void draw_spline2c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
  void draw_spline2c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline3(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3);
  void draw_spline3_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline3c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5);
  void draw_spline3c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline4(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
  void draw_spline4_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline4c(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6);
  void draw_spline4c_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, gs_scalar x5, gs_scalar y5, gs_scalar x6, gs_scalar y6, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline_part(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4);
  void draw_spline_part_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, gs_scalar x4, gs_scalar y4, int c1, int c2, gs_scalar a1, gs_scalar a2);
  void draw_spline_begin(int mode);

  void draw_spline_vertex(gs_scalar x, gs_scalar y);
  void draw_spline_vertex_color(gs_scalar x, gs_scalar y, int col, gs_scalar alpha);
  void draw_spline_end();
  void draw_bezier_quadratic_spline_end();
  int draw_spline_optimized_end();
}

#endif
