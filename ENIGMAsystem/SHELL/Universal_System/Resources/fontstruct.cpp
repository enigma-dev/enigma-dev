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

#include "sprites_internal.h"
#include "fonts_internal.h"
#include "rectpacker/rectpack.h"
#include "Universal_System/image_formats.h"
#include "libEGMstd.h"

#include "Graphics_Systems/graphics_mandatory.h"

#include <list>
#include <string>
#include <string.h>
#include <stdio.h>
#include <vector>

using std::list;
using std::string;

namespace enigma
{
  AssetArray<SpriteFont, -1> sprite_fonts;

  bool fontglyph::empty() {
    return !(std::abs(x2-x) > 0 && std::abs(y2-y) > 0);
  }

  int font_new(uint32_t gs, uint32_t gc) // Creates a new font, allocating 'gc' glyphs
  {
    SpriteFont ret;

    fontglyphrange fgr;
    fgr.glyphstart = gs;

    ret.glyphRanges.push_back(fgr);
    ret.height = 0;

    return sprite_fonts.add(std::move(ret));
  }

  int font_pack(SpriteFont *font, int spr, uint32_t gcount, bool prop, int sep)
  {
      // Implement packing algorithm.
      // This algorithm will try to fit as many glyphs as possible into
      // a square space based on the max height of the font.

      sprite *sspr = spritestructarray[spr];
      std::vector<unsigned char*> glyphdata(gcount); // Raw SpriteFont image data
      std::vector<rect_packer::pvrect> glyphmetrics(gcount);
      std::vector<int> glyphx(gcount), glyphy(gcount);

      int gwm = sspr->width, // Glyph width max: sprite width
          ghm = sspr->height, // Glyph height max: sprite height
          gtw = 0;

      font->height = ghm;

      fontglyphrange& fgr = font->glyphRanges[0];

      for (unsigned i = 0; i < gcount; i++)
      {
        fontglyph fg;
        unsigned fw, fh;
        unsigned char* data = graphics_copy_texture_pixels(sspr->texturearray[i], &fw, &fh);
        //NOTE: Following line replaced gtw = int((double)sspr->width / sspr->texturewarray[i]);
        //this was to fix non-power of two subimages
        //NTOE2: The commented out code was actually wrong - the width was divided by y instead of x. That is why it only worked with power of two.
        gtw = int((double)sspr->width / sspr->texturewarray[i]);
        //gtw = fw;
        glyphdata[i] = data;

        // Here we calculate the bbox
        if (!prop)
           glyphmetrics[i].x = 0,   glyphmetrics[i].y = 0,
           glyphmetrics[i].w = gwm-1, glyphmetrics[i].h = ghm-1;
        else
        {
          glyphmetrics[i].x = gwm, glyphmetrics[i].y = ghm,
          glyphmetrics[i].w = 0,   glyphmetrics[i].h = 0;
          for (int bx = 0; bx < gwm; bx++)
          for (int by = 0; by < ghm; by++)
          {
            if (data[(by*gtw + bx)<<2]) // If this pixel isn't completely transparent
            {
              if (bx < glyphmetrics[i].x) glyphmetrics[i].x = bx;
              if (bx > glyphmetrics[i].w) glyphmetrics[i].w = bx; // Treat width as right for now
              if (by < glyphmetrics[i].y) glyphmetrics[i].y = by;
              if (by > glyphmetrics[i].h) glyphmetrics[i].h = by; // Treat height as bottom for now
            }
          }
          if (glyphmetrics[i].x > glyphmetrics[i].w)
            glyphmetrics[i].x = 0, glyphmetrics[i].y = 0,
            glyphmetrics[i].w = 0, glyphmetrics[i].h = 0;
        }
        fg.x = glyphmetrics[i].x; // Save these metrics while x and y are still relative to each glyph
        fg.y = glyphmetrics[i].y;
        fg.x2 = glyphmetrics[i].w + 1; // And while w and h are still the right and bottom edge coordinates
        fg.y2 = glyphmetrics[i].h + 1;

        fg.xs = glyphmetrics[i].w + sep; // This is just user-specified for sprite-loaded fonts

        glyphmetrics[i].w -= glyphmetrics[i].x - 1; // Fix width and height to be such
        glyphmetrics[i].h -= glyphmetrics[i].y - 1; // instead of right and bottom
        glyphx[i] = glyphmetrics[i].x, glyphy[i] = glyphmetrics[i].y;
        glyphmetrics[i].placed = -1;

        fgr.glyphs.push_back(fg);
      }

      list<unsigned int> boxes;
      for (unsigned i = 0; i < gcount; i++)
        boxes.push_back((glyphmetrics[i].w * glyphmetrics[i].h << 8) + i);
      boxes.sort();

      unsigned w = 64, h = 64;
      rect_packer::rectpnode *rectplane = new rect_packer::rectpnode(0,0,w,h);
      for (list<unsigned int>::reverse_iterator i = boxes.rbegin(); i != boxes.rend() and w and h; )
      {
        rect_packer::rectpnode *nn = rect_packer::rninsert(rectplane, *i & 0xFF, &glyphmetrics.front());
        if (nn)
          rect_packer::rncopy(nn, &glyphmetrics.front(), *i & 0xFF),
          i++;
        else
        {
          w > h ? h <<= 1 : w <<= 1,
          rectplane = rect_packer::expand(rectplane, w, h);
          if (!w or !h) return false;
        }
      }

      std::vector<int> bigtex(w*h);
      for (unsigned i = 0; i < gcount; i++)
      {
        fontglyph& fg = fgr.glyphs[i];
        // Copy the SpriteFont glyph image into the big texture we just allocated
        for (int yy = 0; yy < glyphmetrics[i].h; yy++) {
          for (int xx = 0; xx < glyphmetrics[i].w; xx++) {
            bigtex[w*(glyphmetrics[i].y + yy) + glyphmetrics[i].x + xx] = ((unsigned int*)glyphdata[i])[gtw*(glyphy[i] + yy) + xx + glyphx[i]];
          }
        }
        delete[] glyphdata[i]; // Delete the image data we just copied

        fg.tx = glyphmetrics[i].x / double(w);
        fg.ty = glyphmetrics[i].y / double(h);
        fg.tx2 = (glyphmetrics[i].x + glyphmetrics[i].w) / double(w);
        fg.ty2 = (glyphmetrics[i].y + glyphmetrics[i].h) / double(h);
      }

      font->texture = graphics_create_texture(w,h,w,h,bigtex.data(),false);
      font->twid = w;
      font->thgt = h;
      font->yoffset = 0;

      return true;
  }

fontglyph findGlyph(const SpriteFont& fnt, uint32_t character) {
  for (size_t i = 0; i < fnt.glyphRanges.size(); i++) {
    fontglyphrange fgr = fnt.glyphRanges[i];
    if (character >= fgr.glyphstart && character < fgr.glyphstart + fgr.glyphs.size()) {
      return fgr.glyphs[character - fgr.glyphstart];
    }
  }
  return fontglyph();
}

} // namespace enigma

using enigma::sprite_fonts;

namespace enigma_user
{

int font_get_size(int fnt)
{
    return sprite_fonts[fnt].fontsize;
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
    return sprite_fonts[fnt].glyphRanges[range].glyphstart;
}

uint32_t font_get_last(int fnt, int range)
{
    return sprite_fonts[fnt].glyphRanges[range].glyphstart + sprite_fonts[fnt].glyphRanges[range].glyphs.size();
}

int font_get_range_count(int fnt) {
	return sprite_fonts[fnt].glyphRanges.size();
}

string font_get_fontname(int fnt)
{
    return sprite_fonts[fnt].fontname;
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
  enigma::SpriteFont *fnt = &sprite_fonts[ind];
  fnt->name = name;
  fnt->fontsize = size;
  fnt->bold = bold;
  fnt->italic = italic;

  enigma::fontglyphrange fgr;
  fgr.glyphstart = first;

  fnt->glyphRanges.push_back(fgr);

  return true;
}

bool font_replace_sprite(int ind, int spr, uint32_t first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return false;

  unsigned char gcount = sspr->subcount;
  enigma::SpriteFont *fnt = &sprite_fonts[ind];
  fnt->glyphRanges.clear(); //TODO: Delete glyphs for each range or add it to the destructor?

  enigma::fontglyphrange fgr;
  fgr.glyphstart = first;
  fnt->glyphRanges.push_back(fgr);

  return enigma::font_pack(fnt, spr, gcount, prop, sep);
}

int font_add_sprite(int spr, uint32_t first, bool prop, int sep)
{
  enigma::sprite *sspr = enigma::spritestructarray[spr];
  if (!sspr) return -1;

  unsigned char gcount = sspr->subcount;
  int id = enigma::font_new(first, gcount);
  enigma::SpriteFont* font = &sprite_fonts[id];
  if (!enigma::font_pack(font, spr, gcount, prop, sep)) return -1;
  return id;
}

float font_get_glyph_texture_left(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.tx;
}

float font_get_glyph_texture_top(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.ty;
}

float font_get_glyph_texture_right(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.tx2;
}

float font_get_glyph_texture_bottom(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.ty2;
}

float font_get_glyph_left(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.x;
}

float font_get_glyph_top(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.y;
}

float font_get_glyph_right(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.x2;
}

float font_get_glyph_bottom(int fnt, uint32_t character) {
  const enigma::fontglyph glyph = enigma::findGlyph(sprite_fonts[fnt], character);
  return glyph.y2;
}

}
