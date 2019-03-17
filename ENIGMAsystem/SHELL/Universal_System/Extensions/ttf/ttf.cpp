#include "Universal_System/fonts_internal.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/sprites.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "rectpacker/rectpack.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <algorithm>


namespace enigma {

class FontManager {
public:
  static bool Init() {
    
    FT_Error error = FT_Init_FreeType(&library);
    if (error != 0)
      std::cerr << "Error initializing freetype!" << std::endl;
      
    return (error == 0);
  }
  
  static const FT_Library& GetLibrary() {
    return library;
  }
  
  ~FontManager() {
    FT_Done_FreeType(library);
  }

private:
  FontManager() = default;
  static FT_Library library;
};

FT_Library FontManager::library;
static bool FreeTypeAlive = FontManager::Init();

}

namespace enigma_user {
  
  using enigma::rect_packer::pvrect;
  using enigma::rect_packer::rectpnode;
  
  struct GlyphPair {
    GlyphPair(unsigned index = 0, unsigned area = 0) : index(index), area(area) {}
    unsigned index;
    unsigned area;
  };
   
  int font_add(std::string name, int size, bool bold, bool italic, unsigned first, unsigned last) {
    
    if (!enigma::FreeTypeAlive)
      return -1;
    
    FT_Face face;
    FT_GlyphSlot slot;
    FT_Error error;

    error = FT_New_Face(enigma::FontManager::GetLibrary(), name.c_str(), 0, &face );
    
    if (error != 0)
      return -1;
    
    error = FT_Set_Char_Size( face, size * 64, 0, 72, 0); // 72 dpi, 64 is 26.6 fixed point conversion
    
    if (error != 0)
      return -1;

    slot = face->glyph;
    
    unsigned gcount = last-first;
    int fontid = enigma::font_new(first, gcount);
    
    enigma::font* fnt = enigma::fontstructarray[fontid];
    fnt->name = name;
    fnt->fontsize = size;
    fnt->bold = bold;
    fnt->italic = italic;
    fnt->glyphRangeCount = 1;
    enigma::fontglyphrange fgr;
    fgr.glyphstart = first;
    fgr.glyphcount = gcount;
    fgr.glyphs.resize(gcount);
    
    std::vector<pvrect> glyphmetrics(gcount);
    std::vector<GlyphPair> glyphPairs(gcount);  
    for (unsigned c = first; c < last; ++c) {
      error = FT_Load_Char(face, c, FT_LOAD_DEFAULT);
      
      #ifdef DEBUG_MODE
      if (error != 0)
        std::cerr << "Freetype error loading metrics for char: " <<  static_cast<char>(c) << std::endl;
      #endif
      
      FT_Bitmap& charBitmap = slot->bitmap;
      
      glyphmetrics[c-first] = pvrect(0, 0, charBitmap.width, charBitmap.rows, -1);
      glyphPairs[c-first] = GlyphPair(c-first, charBitmap.width * charBitmap.rows);
    
    }
    
    std::sort(glyphPairs.begin(), glyphPairs.end(), [](const GlyphPair &a, const GlyphPair &b) {
      return (a.area > b.area);
    });
    
    unsigned w = 64, h = 64; // starter size for our texture
    rectpnode* rootNode = new rectpnode(0, 0, w, h);
    for (std::vector<GlyphPair>::reverse_iterator ii = glyphPairs.rbegin(); ii != glyphPairs.rend();) {
      rectpnode* node = rninsert(rootNode, ii->index, glyphmetrics.data());
      
      if (node) {
        rncopy(node, glyphmetrics.data(), ii->index);
        ii++;
      }
      else {
        w > h ? h <<= 1 : w <<= 1,
        rootNode = expand(rootNode, w, h);
        //printf("Expanded to %d by %d\n", w, h);
        if (!w or !h) return -1;
      }
    }
    
    unsigned char* pxdata = new unsigned char[w * h * 4];
    
    for (unsigned y = 0; y < h; ++y) {
      for (unsigned x = 0; x < w; ++x) {
        unsigned index = y * w + x;
        pxdata[index] = 255;
      }
    }
    
    for (const GlyphPair &g : glyphPairs) {
      error = FT_Load_Char(face, g.index + first, FT_LOAD_RENDER);
      
      #ifdef DEBUG_MODE
      if (error != 0) {
        std::cerr << "Freetype error loading bitmap for char: " <<  static_cast<char>(g.index) << std::endl;
        continue;
      }
      #endif
      
      FT_Bitmap& charBitmap = slot->bitmap;
      FT_Glyph_Metrics fftMetrics = face->glyph->metrics;
      
      pvrect glyphInfo = glyphmetrics[g.index];
      
      enigma::fontglyph glyph;
      glyph.x = fftMetrics.horiBearingX / 64;
      glyph.y = -fftMetrics.horiBearingY / 64;
      glyph.x2 = fftMetrics.horiBearingX / 64 + charBitmap.width;
      glyph.y2 = -fftMetrics.horiBearingY / 64 + charBitmap.rows;
      glyph.tx = glyphInfo.x / double(w);
      glyph.ty = glyphInfo.y / double(h);
      glyph.tx2 = (glyphInfo.x +charBitmap.width) / double(w);
      glyph.ty2 = (glyphInfo.y + charBitmap.rows) / double(h);
      glyph.xs = face->glyph->linearHoriAdvance / 65536; // more fixed point conversion
      
      fgr.glyphs[g.index] = glyph;
      
      for (unsigned y = 0; y < charBitmap.rows; ++y) {
        for (unsigned x = 0; x < charBitmap.width; ++x) {
          unsigned index = (y * charBitmap.width + x);
          unsigned index_out = ((y + glyphInfo.y) * w + (x + glyphInfo.x)) * 4;
          pxdata[index_out] = pxdata[index_out + 1] = pxdata[index_out + 2] = 255;
          pxdata[index_out + 3] =  charBitmap.buffer[index];
        }
      }
    }
    
    fnt->glyphRanges[0] = fgr;
    fnt->texture = enigma::graphics_create_texture(w, h, w, h, pxdata, false);
    fnt->twid = w;
    fnt->thgt = h;
    fnt->yoffset = face->glyph->linearVertAdvance / 65536;
    fnt->height = face->glyph->linearVertAdvance / 65536;

    delete rootNode;
    delete[] pxdata;
    FT_Done_Face(face);

    return fontid;
  }
}
