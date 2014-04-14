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

#ifndef _FONTSTRUCT__H
#define _FONTSTRUCT__H

#include <vector>
#include <stdint.h>

namespace enigma
{
  struct fontglyph
  {
    int   x,  y,  x2,  y2; // Draw coordinates, relative to the top-left corner of a full glyph. Added to xx and yy for draw.
    float tx, ty, tx2, ty2; // Texture coords: used to locate glyph on bound font texture
    float xs; // Spacing: used to increment xx
  };
  struct fontglyphrange {
    unsigned int glyphstart, glyphcount;
    std::vector<fontglyph*> glyphs;
  };
  struct font
  {
    // Trivia
    string name, fontname;
    int fontsize; bool bold, italic;

    // Metrics and such
	unsigned glyphRangeCount;
    std::vector<fontglyphrange*> glyphRanges;
    unsigned int height, yoffset;

    // Texture layer
    int texture;
    int twid, thgt;
  };
  struct rawfont {
    string name;
    int id;

    string fontname;
    int fontsize; bool bold, italic;
    unsigned int glyphRangeCount;
  };
  extern rawfont rawfontdata[];
  extern font **fontstructarray;

  extern int rawfontcount, rawfontmaxid;
  int font_new(uint32_t gs, uint32_t gc); // Creates a new font, allocating 'gc' glyphs
  int font_pack(enigma::font *font, int spr, uint32_t gcount, bool prop, int sep);
}

namespace enigma_user {
int font_add(string name, int size, bool bold, bool italic, uint32_t first, uint32_t last);
bool font_replace(int ind, string name, int size, bool bold, bool italic, uint32_t first, uint32_t last);
int  font_add_sprite(int spr, uint32_t first, bool prop, int sep);
bool font_replace_sprite(int ind, int spr, uint32_t first, bool prop, int sep);
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
string font_get_fontname(int fnt);
void font_delete(int fnt);
bool font_exists(int fnt);
}

#endif
