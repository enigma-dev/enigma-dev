#include "fonts.h"

using enigma::rect_packer::pvrect;
using enigma::rect_packer::pack_rectangles;

namespace enigma {
namespace fonts {
unsigned char* font_pack(RawFont& font, unsigned& textureW, unsigned& textureH) {
 
  if (!pack_rectangles(font.glyphs, textureW, textureH))
    return nullptr;

  // intialize blank image (the caller is responisible for deleting pxdata)
  unsigned char* pxdata = new unsigned char[textureW * textureH * font.channels];
  for (unsigned y = 0; y < textureH; ++y) {
    for (unsigned x = 0; x < textureW; ++x) {
      unsigned index = y * textureW + x;
      for (unsigned i = 0; i < font.channels; ++i)
        pxdata[index+1] = 0;
    }
  }

  // now we can copy all our glyphs onto single texture
  for (RawGlyph& rg : font.glyphs) {

    GlyphMetrics glyph;
    glyph.x = rg.horiBearingX;
    glyph.y = -rg.horiBearingY;
    glyph.x2 = rg.horiBearingX + rg.w();
    glyph.y2 = -rg.horiBearingY + rg.h();
    glyph.tx = rg.x() / double(textureW);
    glyph.ty = rg.y() / double(textureH);
    glyph.tx2 = (rg.x() + rg.w()) / double(textureW);
    glyph.ty2 = (rg.y() + rg.h()) / double(textureH);
    glyph.xs = rg.linearHoriAdvance;

    // insert our glyph into the glyph range
    if (rg.range == nullptr) rg.range = &font.ranges[0];
    rg.range->glyphs[rg.character - rg.range->start] = glyph;

    // write glyph pixel data to the texture
    for (unsigned y = 0; y < rg.h(); ++y) {
      for (unsigned x = 0; x < rg.w(); ++x) {
        unsigned index = (y * rg.w() + x) * font.channels;
        unsigned index_out = ((y + rg.y()) * textureW + (x + rg.x())) * font.channels;
        for (unsigned i = 0; i < font.channels; ++i)
          pxdata[index_out + i] = rg.pxdata[index + i];
      }
    }
  }

  return pxdata;
}

}
}
