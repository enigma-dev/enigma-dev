/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#ifndef ENIGMA_GSSTDRAW_H
#define ENIGMA_GSSTDRAW_H

#include "Universal_System/scalar.h"

#include <list>

namespace enigma {

extern bool lineStippleEnable, msaaEnabled, alphaBlend, alphaTest;
extern unsigned short lineStipplePattern;
extern unsigned char alphaTestRef;
extern float drawPointSize, drawLineWidth;
extern int drawFillMode, lineStippleScale;

void draw_set_state_dirty(bool dirty=true);
bool draw_get_state_dirty();

void graphics_state_flush();

} // namespace enigma

namespace enigma_user
{
  enum {
    rs_point, // Render vertices as points
    rs_line,  // Render in wireframe mode
    rs_solid  // Normal render mode
  };

  void draw_state_flush();
  void draw_set_fill_mode(int fill);
  void draw_set_line_width(float value);
  void draw_set_point_size(float value);
  int draw_get_msaa_maxlevel();
  bool draw_get_msaa_supported();
  void draw_set_msaa_enabled(bool enable);
  bool draw_get_alpha_test();
  unsigned draw_get_alpha_test_ref_value();
  void draw_set_alpha_test(bool enable);
  void draw_set_alpha_test_ref_value(unsigned val);
  void draw_enable_alphablend(bool enable);
  void draw_set_line_stipple(bool enable);
  void draw_set_line_pattern(int scale, unsigned short pattern);
  void draw_point(gs_scalar x, gs_scalar y);
  void draw_point_color(gs_scalar x, gs_scalar y, int color);
  void draw_line(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2);
  void draw_line_width(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar wid);
  void draw_line_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color1, int color2);
  void draw_line_width_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar wid, int color1, int color2);
  void draw_rectangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline);
  void draw_rectangle_angle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float angle, bool outline);
  void draw_rectangle_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int color1, int color2, int color3, int color4, bool outline = false);
  void draw_set_circle_precision(float precision);
  float draw_get_circle_precision(void);
  void draw_circle(gs_scalar x, gs_scalar y, float rad, bool outline);
  void draw_circle_color(gs_scalar x, gs_scalar y, float rad, int color1, int color2, bool outline = false);
  void draw_circle_perfect(gs_scalar x, gs_scalar y, float rad, bool outline);
  void draw_circle_color_perfect(gs_scalar x, gs_scalar y, float rad, int color1, int color2, bool outline = false);
  void draw_ellipse(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline);
  void draw_ellipse_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int col1, int col2, bool outline = false);
  void draw_ellipse_perfect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline);
  void draw_sector(gs_scalar x, gs_scalar y, gs_scalar r, float start_angle, float end_angle, bool outline = false);
  void draw_sector(gs_scalar x, gs_scalar y, gs_scalar rx, gs_scalar ry, float start_angle, float end_angle, bool outline = false);
  void draw_triangle(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, bool outline);
  void draw_triangle_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar x3, gs_scalar y3, int col1, int col2, int col3, bool outline = false);
  void draw_roundrect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, bool outline);
  void draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline);
  void draw_roundrect_ext(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float xrad, float yrad, bool outline);
  void draw_roundrect_color_ext(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float xrad, float yrad, int col1, int col2, bool outline);
  void draw_roundrect_precise(gs_scalar x1, gs_scalar y1,gs_scalar x2, gs_scalar y2, float rad, bool outline);
  void draw_roundrect_precise_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float rad, int col1, int col2, bool outline);

  inline void draw_roundrect(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline) {
    draw_roundrect(x1, y1, x2, y2, 10, outline);
  }

  inline void draw_roundrect_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int col1, int col2, bool outline) {
    draw_roundrect_color(x1, y1, x2, y2, 10, col1, col2, outline);
  }

  inline void draw_roundrect_precise(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool outline) {
    draw_roundrect_precise(x1, y1, x2, y2, 10, outline);
  }

  inline void draw_roundrect_precise_color(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, int col1, int col2, bool outline) {
    draw_roundrect_precise_color(x1, y1, x2, y2, 10, col1, col2, outline);
  }

  void draw_healthbar(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float amount, int backcol, int mincol, int maxcol, int direction, bool showback, bool showborder);
  void draw_arrow(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float arrow_size, float line_size = 1, bool outline = false);
  void draw_button(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, float border_width, bool up);
  void draw_button(gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, bool up);
  int draw_mandelbrot(int x, int y, float w, double Zx, double Zy, double Zw, unsigned iter);
  int draw_getpixel(int,int);
  int draw_getpixel_ext(int, int);

  #define draw_get_pixel draw_getpixel
  #define draw_get_pixel_ext draw_getpixel_ext
}

#endif //ENIGMA_GSSTDRAW_H
