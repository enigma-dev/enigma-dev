/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Harijs Grinbergs
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

#ifndef ENIGMA_GSFONT_H
#define ENIGMA_GSFONT_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"

namespace enigma  {
  extern int currentfont;
}

namespace enigma_user
{
  const unsigned fa_left = 0;
  const unsigned fa_center = 1;
  const unsigned fa_right = 2;
  const unsigned fa_top = 0;
  const unsigned fa_middle = 1;
  const unsigned fa_bottom = 2;

  void draw_set_halign(unsigned align);
  unsigned draw_get_halign();
  void draw_set_valign(unsigned align);
  unsigned draw_get_valign();
  void draw_set_font(int fnt);
  int draw_get_font();
  void draw_text(gs_scalar x, gs_scalar y, variant str);
  void draw_text_skewed(gs_scalar x, gs_scalar y, variant str, gs_scalar top, gs_scalar bottom);
  void draw_text_color(gs_scalar x, gs_scalar y, variant str, int c1, int c2, int c3, int c4, gs_scalar a);

  void draw_text_sprite(gs_scalar x, gs_scalar y, variant vstr, int sep, int lineWidth, int sprite, int firstChar, int scale);

  void draw_text_ext(gs_scalar x, gs_scalar y, variant str, gs_scalar sep, gs_scalar wid);
  void draw_text_ext_color(gs_scalar x, gs_scalar y, variant str, gs_scalar sep, gs_scalar wid, int c1, int c2, int c3, int c4, gs_scalar a);

  void draw_text_transformed(gs_scalar x, gs_scalar y, variant str, gs_scalar xscale, gs_scalar yscale, double rot);
  void draw_text_transformed_color(gs_scalar x, gs_scalar y, variant str, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a);

  void draw_text_ext_transformed(gs_scalar x, gs_scalar y, variant str, gs_scalar sep, gs_scalar wid, gs_scalar xscale, gs_scalar yscale, double rot);
  void draw_text_ext_transformed_color(gs_scalar x, gs_scalar y, variant str, gs_scalar sep, gs_scalar wid, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a);

  unsigned int font_get_texture(int fnt);
  unsigned int font_get_texture_width(int fnt);
  unsigned int font_get_texture_height(int fnt);
  unsigned int font_height(int fnt);

  double string_char_width(variant vstr);

  unsigned int string_width(variant str);
  unsigned int string_height(variant str);

  unsigned int string_width_ext(variant str, gs_scalar sep, gs_scalar wid);
  unsigned int string_height_ext(variant str, gs_scalar sep, gs_scalar wid);

  unsigned int string_width_line(variant str, int line);
  unsigned int string_width_ext_line(variant str, gs_scalar wid, int line);
  unsigned int string_width_ext_line_count(variant str, gs_scalar wid);
}

#endif // ENIGMA_GSFONT_H
