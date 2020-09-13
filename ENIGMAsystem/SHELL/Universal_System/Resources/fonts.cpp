/** Copyright (C) 2008-2011 Josh Ventura
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

#include "sprites.h"
#include "sprites_internal.h"
#include "fonts_internal.h"
#include "rectpacker/rectpack.h"
#include "Universal_System/image_formats.h"
#include "libEGMstd.h"
#include "Graphics_Systems/General/GScolors.h"

#include <list>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <iostream>

#include "fonts/font_pack.cpp"

using std::list;
using std::string;
using enigma::rect_packer::pvrect;
using namespace enigma::fonts;

namespace enigma {
namespace fonts {

AssetArray<SpriteFont, -1> sprite_fonts;

void SpriteFont::destroy() {
  ranges.clear();
  if (texture.ID >= 0) graphics_delete_texture(texture.ID);
  texture.ID = -1;
}

bool SpriteFont::isDestroyed() const { return texture.ID == -1 || ranges.empty(); }

const char* SpriteFont::getAssetTypeName() { return "font"; }

GlyphMetrics findGlyph(const SpriteFont& fnt, uint32_t character) {
  for (size_t i = 0; i < fnt.ranges.size(); i++) {
    const GlyphRange& fgr = fnt.ranges[i];
    if (character >= fgr.start && character < fgr.start + fgr.glyphs.size()) {
      return fgr.glyphs[character - fgr.start];
    }
  }
  return GlyphMetrics();
}

} // namespace fonts
} // namespace enigma


namespace enigma_user {

int font_get_size(int fnt)
{
    return sprite_fonts[fnt].fontSize;
}

bool font_get_bold(int fnt)
{
    return sprite_fonts[fnt].bold;
}

bool font_get_italic(int fnt)
{
    return sprite_fonts[fnt].italic;
}

uint32_t font_get_first(int fnt, int range)
{
    return sprite_fonts[fnt].ranges[range].start;
}

uint32_t font_get_last(int fnt, int range)
{
    return sprite_fonts[fnt].ranges[range].start + sprite_fonts[fnt].ranges[range].glyphs.size();
}

int font_get_range_count(int fnt) {
	return sprite_fonts[fnt].ranges.size();
}

string font_get_fontname(int fnt)
{
    return sprite_fonts[fnt].fontName;
}

void font_delete(int fnt)
{
    sprite_fonts.destroy(fnt);
}

bool font_exists(int fnt)
{
    return sprite_fonts.exists(fnt);
}

bool font_replace(int ind, string name, int size, bool bold, bool italic, uint32_t first, uint32_t last)
{
  SpriteFont *fnt = &sprite_fonts[ind];
  fnt->name = name;
  fnt->fontSize = size;
  fnt->bold = bold;
  fnt->italic = italic;

  GlyphRange fgr;
  fgr.start = first;

  fnt->ranges.push_back(fgr);

  return true;
}

static pvrect get_image_bounds(unsigned char* pxdata, unsigned width, unsigned height, bool alpha) {
  pvrect rect(0, 0, 0, 0, -1);
  
  int transparenyColor = make_color_rgb(pxdata[0], pxdata[1], pxdata[2]);

  for (unsigned x = 0; x < width; ++x) {
    for (unsigned y = 0; y < height; ++y) {
      bool transparent = false;
      int index = width * x + y;
      if (alpha) transparent = (pxdata[index + 3] == 255);
      else transparent = (make_color_rgb(pxdata[index], pxdata[index + 1], pxdata[index + 2]) == transparenyColor);

      if (!transparent) {
        if (rect.x == 0) rect.x = x;
        if (rect.y == 0) rect.y = y;
        rect.w = x;
        rect.h = y;
      }
    }
  }
  
  return rect;
}

static SpriteFont font_sprite_helper(const enigma::Sprite& sprite, uint32_t first, bool prop, int sep) {
  size_t glyphCount = sprite.SubimageCount();
  
  RawFont rawFont;
  rawFont.channels = 4;
  rawFont.ranges = { GlyphRange(first, first + glyphCount) };
  
  rawFont.glyphs.resize(glyphCount);
  
  size_t index = 0;
  for (RawGlyph& g : rawFont.glyphs) {
    g.character = first + index;
    unsigned w, h;
    g.pxdata = enigma::graphics_copy_texture_pixels(sprite.GetSubimage(index).textureID, &w, &h);
    g.horiBearingX = sep;
    g.horiBearingY = sep;
    (prop) ? g.dimensions = pvrect(0, 0, sprite.width, sprite.height, -1) : g.dimensions = get_image_bounds(g.pxdata, sprite.width, sprite.height, true);
    g.left = g.dimensions.x;
    g.top = g.dimensions.y;
    g.linearHoriAdvance = g.dimensions.w + sep;
    index++;
  }

  unsigned textureW, textureH;
  unsigned char* pxdata = font_pack(rawFont, textureW, textureH);
  
  SpriteFont font("SpriteFont", "SpriteFont", 1, false, false, rawFont.yOffset, rawFont.lineHeight, textureW, textureH, pxdata, 4, rawFont.ranges);
  font.texture.init();
  
  return font;
}

bool font_replace_sprite(int sprid1, int sprid2, uint32_t first, bool prop, int sep)
{
  const enigma::Sprite& spr = enigma::sprites.get(sprid2);
  return (sprite_fonts.replace(sprid1, font_sprite_helper(spr, first, prop, sep)) > 0);
}

int font_add_sprite(int sprid, uint32_t first, bool prop, int sep)
{
  const enigma::Sprite& spr = enigma::sprites.get(sprid);
  return (sprite_fonts.add(font_sprite_helper(spr, first, prop, sep)) > 0);
}

float font_get_glyph_texture_left(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.tx;
}

float font_get_glyph_texture_top(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.ty;
}

float font_get_glyph_texture_right(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.tx2;
}

float font_get_glyph_texture_bottom(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.ty2;
}

float font_get_glyph_left(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.x;
}

float font_get_glyph_top(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.y;
}

float font_get_glyph_right(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.x2;
}

float font_get_glyph_bottom(int fnt, uint32_t character) {
  const GlyphMetrics glyph = findGlyph(sprite_fonts[fnt], character);
  return glyph.y2;
}

}
