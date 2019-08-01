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

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_FONTS_INTERNAL_H
#define ENIGMA_FONTS_INTERNAL_H

#include "Graphics_Systems/graphics_mandatory.h"
#include "AssetArray.h"

#include <string>
#include <vector>
#include <stdint.h>

namespace enigma
{
  struct fontglyph
  {
    fontglyph() : x(0), y(0), x2(0), y2(0), tx(0), ty(0), tx2(0), ty2(0), xs(0) {}
    bool empty();
    int   x,  y,  x2,  y2; // Draw coordinates, relative to the top-left corner of a full glyph. Added to xx and yy for draw.
    float tx, ty, tx2, ty2; // Texture coords: used to locate glyph on bound font texture
    float xs; // Spacing: used to increment xx
  };
  struct fontglyphrange {
    fontglyphrange() : glyphstart(0) {}
    unsigned int glyphstart;
    std::vector<fontglyph> glyphs;
  };
  class SpriteFont
  {
   public:
     SpriteFont() : name(""), fontname(""), fontsize(0),
       bold(false), italic(false), height(0), yoffset(0), texture(-1),
       twid(0), thgt(0) {}
    // Trivia
    std::string name, fontname;
    int fontsize; 
    bool bold, italic;

    // Metrics and such
    std::vector<fontglyphrange> glyphRanges;
    unsigned int height, yoffset;

    // Texture layer
    int texture;
    int twid, thgt;

    void destroy() { 
      glyphRanges.clear();
      if (texture >= 0) graphics_delete_texture(texture);
      texture = -1;
    }
    bool isDestroyed() const { return texture == -1 || glyphRanges.empty(); }

    static const char* getAssetTypeName() { return "font"; }
  };
  struct rawfont {
    std::string name;
    int id;

    std::string fontname;
    int fontsize; bool bold, italic;
    unsigned int glyphRangeCount;
  };
  extern std::vector<rawfont> rawfontdata;
  extern AssetArray<SpriteFont, -1> sprite_fonts;

  extern int rawfontcount, rawfontmaxid;
  int font_new(uint32_t gs, uint32_t gc); // Creates a new font, allocating 'gc' glyphs
  int font_pack(SpriteFont *font, int spr, uint32_t gcount, bool prop, int sep);
  fontglyph findGlyph(const SpriteFont& fnt, uint32_t character);
} //namespace enigma

#endif //ENIGMA_FONTS_INTERNAL_H
