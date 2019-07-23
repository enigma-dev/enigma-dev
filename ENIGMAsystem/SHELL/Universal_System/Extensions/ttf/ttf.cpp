#include "fonts/fonts.h"
#include "include.h"
#include "Universal_System/Resources/fonts_internal.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>

using enigma::GlyphRange;
using enigma::Glyph;

namespace enigma_user {

int font_add(std::string name, unsigned size, bool bold, bool italic, unsigned first, unsigned last) {
  if (last < first) {
    DEBUG_MESSAGE("Freetype error invalid glyph range last < first", MESSAGE_TYPE::M_ERROR);
    return -1;
  }

  unsigned w, h; //size of texture 
  unsigned fontHeight = 0;
  int yoffset;
  std::vector<GlyphRange> glyphRanges = {GlyphRange(first, last)};

  unsigned char* pxdata = enigma::font_add(name, size, bold, italic, w, h, yoffset, fontHeight, glyphRanges);
  if (pxdata != nullptr) {
    int fontid = enigma::font_new(first, glyphRanges.back().glyphs.size());

    enigma::font* fnt = enigma::fontstructarray[fontid];
    fnt->name = name;
    fnt->fontsize = size;
    fnt->bold = bold;
    fnt->italic = italic;

    fnt->glyphRanges[0] = glyphRanges.back();
    fnt->texture = enigma::graphics_create_texture(w, h, w, h, pxdata, false);
    fnt->twid = w;
    fnt->thgt = h;
    fnt->yoffset = yoffset;
    fnt->height = fontHeight;


    delete[] pxdata;

    return fontid;

  } else return -1;

}

}
