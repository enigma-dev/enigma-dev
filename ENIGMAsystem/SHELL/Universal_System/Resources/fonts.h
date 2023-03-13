/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
*** Copyright (C) 2014 Robert B. Colton
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

#ifndef ENIGMA_FONTS_H
#define ENIGMA_FONTS_H

#include <string>
#include <stdint.h>

namespace enigma_user 
{
  bool font_replace(int ind, std::string name, int size, bool bold, bool italic, uint32_t first, uint32_t last);
  int  font_add_sprite(int spr, uint32_t first, bool prop, int sep);
  bool font_replace_sprite(int ind, int spr, uint32_t first, bool prop, int sep);
  std::string font_get_fontname(int fnt);
  void font_delete(int fnt);
  bool font_exists(int fnt);
  bool font_get_bold(int fnt);
  bool font_get_italic(int fnt);
  int font_get_size(int fnt);
  uint32_t font_get_first(int fnt, int range=0);
  uint32_t font_get_last(int fnt, int range=0);
  int font_get_range_count(int fnt);
  float font_get_glyph_left(int fnt, uint32_t character);
  float font_get_glyph_right(int fnt, uint32_t character);
  float font_get_glyph_top(int fnt, uint32_t character);
  float font_get_glyph_bottom(int fnt, uint32_t character);
  float font_get_glyph_texture_left(int fnt, uint32_t character);
  float font_get_glyph_texture_top(int fnt, uint32_t character);
  float font_get_glyph_texture_right(int fnt, uint32_t character);
  float font_get_glyph_texture_bottom(int fnt, uint32_t character);
} //namespace enigma_user

#endif //ENIGMA_FONTS_H
