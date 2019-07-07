/** Copyright (C) 2014-2015 Harijs Grinbergs
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

#ifndef BGUI_COMMON_H
#define BGUI_COMMON_H

#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GSsprite.h"

namespace enigma {
namespace gui {
struct offset {
  gs_scalar x = 0.0, y = 0.0;
  offset() {}
  offset(gs_scalar ox, gs_scalar oy) : x(ox), y(oy) {}
  void set(gs_scalar ox, gs_scalar oy) { x = ox, y = oy; }
};

struct rect_offset {
  bool zero = true;
  gs_scalar top = 0.0, left = 0.0, bottom = 0.0, right = 0.0;
  rect_offset() {}
  rect_offset(gs_scalar l, gs_scalar t, gs_scalar r, gs_scalar b) : top(t), left(l), bottom(b), right(r) {}
  void set(gs_scalar l, gs_scalar t, gs_scalar r, gs_scalar b) {
    left = l, top = t, right = r, bottom = b;
    zero = ((((0 == l) == t) == r) == b);
  }
};

struct rect {
  bool zero = true;
  gs_scalar x = 0.0, y = 0.0, w = 0.0, h = 0.0;
  rect() {}
  rect(gs_scalar rx, gs_scalar ry, gs_scalar rw, gs_scalar rh) : x(rx), y(ry), w(rw), h(rh) {}
  void set(gs_scalar rx, gs_scalar ry, gs_scalar rw, gs_scalar rh) {
    x = rx, y = ry, w = rw, h = rh;
    zero = ((0 == w) == h);
  }
  bool point_inside(gs_scalar tx, gs_scalar ty) { return (tx > x && tx < x + w && ty > y && ty < y + h); }
};

struct font_style {
  int font = -1;
  unsigned int halign = enigma_user::fa_left;
  unsigned int valign = enigma_user::fa_top;
  int color = enigma_user::c_white;
  gs_scalar alpha = 1.0;
  gs_scalar textx = 0.0, texty = 0.0;
  void use() {
    enigma_user::draw_set_halign(halign);
    enigma_user::draw_set_valign(valign);
    enigma_user::draw_set_font(font);
    enigma_user::draw_set_color(color);
    enigma_user::draw_set_alpha(alpha);
  }
};

struct render_style {
  int color = enigma_user::c_white;
  gs_scalar alpha = 1.0;
  gs_scalar scale_x = 1.0, scale_y = 1.0;
  gs_scalar rotation = 1.0;
};

inline font_style get_current_draw_state() {
  font_style st;
  st.halign = enigma_user::draw_get_halign();
  st.valign = enigma_user::draw_get_valign();
  st.font = enigma_user::draw_get_font();
  st.color = enigma_user::draw_get_color();
  st.alpha = enigma_user::draw_get_alpha();
  return st;
}

inline void set_current_draw_state(const font_style &st) {
  enigma_user::draw_set_halign(st.halign);
  enigma_user::draw_set_valign(st.valign);
  enigma_user::draw_set_color(st.color);
  enigma_user::draw_set_alpha(st.alpha);
  enigma_user::draw_set_font(st.font);
}
}  //namespace gui
}  //namespace enigma

#endif
