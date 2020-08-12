#ifndef ENIGMA_SHARED_FONTS_H
#define ENIGMA_SHARED_FONTS_H

#include "rectpacker/rectpack.h" // pvrect
#include "texture.h"

#include <vector>
#include <string>
#include <filesystem>

namespace enigma {

namespace fonts {

/// Struct holding the location of Glyph on packed texture
struct GlyphMetrics {
  GlyphMetrics(int x = 0, int y = 0, int x2 = 0, int y2 = 0, float tx = 0, float ty = 0, float tx2 = 0, float ty2 = 0, float xs = 0) : 
    x(x), y(y), x2(x2), y2(y2), tx(tx), ty(ty), tx2(tx2), ty2(ty2), xs(xs) {}
  bool empty() {  return !(std::abs(x2-x) > 0 && std::abs(y2-y) > 0); }
  int   x,  y,  x2,  y2; // Draw coordinates, relative to the top-left corner of a full glyph. Added to xx and yy for draw.
  float tx, ty, tx2, ty2; // Texture coords: used to locate glyph on bound font texture
  float xs; // Spacing: used to increment xx
};

/// Range of characters (eg: a-z)
struct GlyphRange {
  GlyphRange() : start(0) {}
  GlyphRange(unsigned first, unsigned last) : start(first) { glyphs.resize(last-first+1); }
  GlyphRange(unsigned first, unsigned /*last*/, std::initializer_list<GlyphMetrics> glyphs) : start(first), glyphs(glyphs) {}
  unsigned int start;
  size_t size() const { return glyphs.size(); }
  std::vector<GlyphMetrics> glyphs;
};

/// Struct holding the pixel data and dimensions for individual glyphs
struct RawGlyph {
  RawGlyph() : character(0), pxdata(nullptr), dimensions(0,0,0,0,-1), left(0), top(0), horiBearingX(0), horiBearingY(0), range(nullptr) {}
  void destroy() { delete[] pxdata; }
  unsigned character; // the letter/character we want to render
  unsigned char* pxdata; // raw pixel data of glyph
  rect_packer::pvrect dimensions; // rectange(x,y,w,h) holding the dimensions of our glyph in pixels
  unsigned left, top;
  int x() const { return dimensions.x; }
  int y() const { return dimensions.y; }
  unsigned w() const { return dimensions.w; }
  unsigned h() const { return dimensions.h; }
  double horiBearingX; // left side bearing for horizontal layout
  double horiBearingY; // top side bearing for horizontal layout
  double linearHoriAdvance; // the advance width of the unhinted glyph
  GlyphRange* range; // range that owns the glyph
};

/// Ready to be packed font data
struct RawFont {
  RawFont() : name(""), channels(1), lineHeight(0), yOffset(0) {}
  std::string name;
  unsigned channels;
  unsigned lineHeight; // space between lines
  int yOffset;
  std::vector<RawGlyph> glyphs;
  std::vector<GlyphRange> ranges;
};

/// Font with a packed texture of all glyphs (this is the one used throughout most the engine)
struct SpriteFont {
  SpriteFont(std::string name, std::string fontName, unsigned fontSize, bool bold, 
    bool italic, unsigned lineHeight, int yOffset, unsigned textureW, unsigned textureH,
    unsigned char* pxdata, unsigned channels, std::initializer_list<GlyphRange> ranges) :
     name(name), fontName(fontName), fontSize(fontSize), bold(bold), italic(italic), lineHeight(lineHeight), 
     yOffset(yOffset), texture(textureW, textureH, pxdata, channels), ranges(ranges) {}

  SpriteFont(std::string name = "", std::string fontName = "", unsigned fontSize = 0, bool bold = false, 
    bool italic = false, unsigned lineHeight = 0, int yOffset = 0, unsigned textureW = 0, unsigned textureH = 0,
    unsigned char* pxdata = nullptr, unsigned channels = 1, std::vector<GlyphRange> ranges = {}) :
     name(name), fontName(fontName), fontSize(fontSize), bold(bold), italic(italic), lineHeight(lineHeight), 
     yOffset(yOffset), texture(textureW, textureH, pxdata, channels), ranges(ranges) {}

  // The folowing functions are for use & implemented exclusively in the engine
  //void init(); // loads our pixel data into memory and deletes it
  void destroy(); // deletes the texture
  bool isDestroyed() const;
  static const char* getAssetTypeName();

  std::string name;  // name of asset 
  std::string fontName; // name of font (typically derived from the ttf)
  unsigned fontSize; // point size of font
  bool bold, italic;
  unsigned lineHeight; // height of text line in pixels
  int yOffset; // idk?
  Texture texture;
  std::vector<GlyphRange> ranges; // range of characters (eg: a-z)
};

/// Loads a ttf file into a RawFont
bool font_load(const std::filesystem::path& fName, unsigned size, RawFont& font);

/// Packs a vector of RawGlyph(s) on to a single texture and returns that texture
unsigned char* font_pack(RawFont& font, unsigned& textureW, unsigned& textureH);

} //namespace fonts
} //namespace enigma

namespace enigma_user {
  /// Searches and indexes all ttf files by name in the given path 
  void font_add_search_path(const std::filesystem::path& path, bool recursive = false);
  
  /// Returns a ttf file based on the given font name
  std::filesystem::path font_find(std::string name, bool bold, bool italic, bool exact_match = false);
}
#endif
