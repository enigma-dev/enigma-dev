#ifndef ENIGMA_SHARED_FONTS_H
#define ENIGMA_SHARED_FONTS_H

#include <vector>
#include <string>

namespace enigma {

struct Glyph {
  Glyph() : x(0), y(0), x2(0), y2(0), tx(0), ty(0), tx2(0), ty2(0), xs(0) {}
  bool empty();
  int   x,  y,  x2,  y2; // Draw coordinates, relative to the top-left corner of a full glyph. Added to xx and yy for draw.
  float tx, ty, tx2, ty2; // Texture coords: used to locate glyph on bound font texture
  float xs; // Spacing: used to increment xx
};

struct GlyphRange {
  GlyphRange() : start(0) {}
  GlyphRange(unsigned first, unsigned last) : start(first) { glyphs.resize(last-first+1); }
  unsigned int start;
  size_t size() { return glyphs.size(); }
  std::vector<Glyph> glyphs;
};

unsigned char* font_add(std::string name, unsigned size, bool bold, bool italic, unsigned& w, unsigned& h, 
  int& yoffset, unsigned& height, std::vector<GlyphRange>& glyphRanges);

}

namespace enigma_user {
  void font_add_search_path(std::string path);
  std::string font_find(std::string name, bool bold, bool italic, bool exact_match = false);
}
#endif
