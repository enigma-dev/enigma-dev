#include "rectpacker/rectpack.h"
#include "strings_util.h"
#include "fonts.h"

#ifdef ENIGMA_GAME
#include "Widget_Systems/widgets_mandatory.h"
#else 
#include <iostream>
#define DEBUG_MESSAGE(msg, severity) std::cerr << msg << std::endl;
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H
#include FT_SFNT_NAMES_H

#include <map>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdlib>

#if __cplusplus > 201402L
  #include <filesystem>
  namespace fs = std::filesystem;
#endif

using namespace enigma::rect_packer;

static std::vector<std::string> fontSearchPaths;
static std::map<std::string, std::string> fontNames;


namespace enigma {

void init_font_search_dirs() {

  std::vector<std::string> intialPaths = {
    #ifdef WIN32
    std::string(std::getenv("windir")) + "/Fonts",
    std::string(std::getenv("windir")) + "/boot/Fonts",
    #else
    std::string(std::getenv("HOME")) + "/.local/share/fonts",
    std::string(std::getenv("HOME")) + "/.fonts",
    "/usr/share/fonts/",
    #endif
  };
  
  for (std::string& p : intialPaths) {
    enigma_user::font_add_search_path(p);
  }
}

class FontManager {
public:
  static bool Init() {

    FT_Error error = FT_Init_FreeType(&library);
    if (error != 0)
      DEBUG_MESSAGE("Error initializing freetype!", MESSAGE_TYPE::M_ERROR);

   init_font_search_dirs();

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

struct GlyphInfo {
  unsigned character; // the char we want to render
  size_t index; // our index in vector
  pvrect dimensions; // rectange(x,y,w,h)
  unsigned area() const { return dimensions.w * dimensions.h; }
  int x() const { return dimensions.x; }
  int y() const { return dimensions.y; }
  int w() const { return dimensions.w; }
  int h() const { return dimensions.h; }
  GlyphRange* range; // range that owns the glyph
};

unsigned char* font_add(std::string name, unsigned size, bool bold, bool italic, 
  unsigned& w, unsigned& h, int& yoffset, unsigned& height, std::vector<GlyphRange>& glyphRanges) {

  if (!enigma::FreeTypeAlive)
    return nullptr;

  FT_Face face;
  FT_GlyphSlot slot;
  FT_Error error;

  error = FT_New_Face(enigma::FontManager::GetLibrary(), name.c_str(), 0, &face );

  if (error != 0)
    return nullptr;

  // GameMaker has always generated fonts at 96 dpi, default Windows dpi since 1980s
  error = FT_Set_Char_Size( face, size * 64, 0, 96, 0); // 96 dpi, 64 is 26.6 fixed point conversion

  if (error != 0)
    return nullptr;

  slot = face->glyph;

  size_t gcount = 0;
  for (const GlyphRange& range : glyphRanges) gcount += range.glyphs.size();
 
  // vector of all Glyphs used for sorting
  std::vector<GlyphInfo> glyphInfo(gcount);
   
  size_t currOffset = 0; // our location in the glyphInfo vector
  // loop over all ranges
  for (GlyphRange& range : glyphRanges) {
    // loop over each character per range
    for (size_t c = 0; c < range.size(); ++c) {
      // open character to get metrics
      error = FT_Load_Char(face, range.start + c, FT_LOAD_DEFAULT);

      #if defined(DEBUG_MODE) || !defined(ENIGMA_GAME)
      if (error != 0)
        DEBUG_MESSAGE("Freetype error loading metrics for char: " + std::to_string(range.start + c), MESSAGE_TYPE::M_ERROR);
      #endif
      
       FT_Bitmap& charBitmap = slot->bitmap;

       GlyphInfo gi;
       gi.character = range.start + c;
       gi.index = currOffset + c;
       gi.dimensions = pvrect(0, 0, charBitmap.width, charBitmap.rows, -1);
       gi.range = &range;
       glyphInfo[currOffset + c] = gi;
    }
    // done with one range so update offset
    currOffset += range.size();
  }

  // sort all glyphs by area
  std::sort(glyphInfo.begin(), glyphInfo.end(), [](const GlyphInfo &a, const GlyphInfo &b) {
    return (a.area() > b.area());
  });

  // Josh's rectpacker code needs access to an array so we copy all pvrects into one here 
  std::vector<pvrect> dimensions(gcount);
  size_t currIndex = 0;
  for (GlyphInfo& gi : glyphInfo) {
    dimensions[currIndex++] = gi.dimensions;
  }

  // insert the metrics and calculate a minimum (power of 2) texture size
  w = 64, h = 64; // starter size for our texture
  rectpnode* rootNode = new rectpnode(0, 0, w, h);
  for (std::vector<GlyphInfo>::reverse_iterator ii = glyphInfo.rbegin(); ii != glyphInfo.rend();) {
    rectpnode* node = rninsert(rootNode, ii->index, dimensions.data());

    if (node) {
      rncopy(node, dimensions.data(), ii->index);
      ii++;
    }
    else {
      w > h ? h <<= 1 : w <<= 1,
      rootNode = expand(rootNode, w, h);
      if (!w or !h) return nullptr;
    }
  }

  // Copy our updated pvrects back to our glyphs
  currIndex = 0;
  for (GlyphInfo& gi : glyphInfo) {
     gi.dimensions = dimensions[currIndex++];
  }

  // intialize blank image (the caller is responisible for deleting pxdata)
  unsigned char* pxdata = new unsigned char[w * h * 4];
  for (unsigned y = 0; y < h; ++y) {
    for (unsigned x = 0; x < w; ++x) {
      unsigned index = y * w + x;
      pxdata[index] = 255;
    }
  }
  

  // now we can render all our glyphs
  for (GlyphInfo &g : glyphInfo) {
    error = FT_Load_Char(face, g.character, FT_LOAD_RENDER);

    #if defined(DEBUG_MODE) || !defined(ENIGMA_GAME) 
    if (error != 0) {
      DEBUG_MESSAGE("Freetype error loading bitmap for char: " + g.character, MESSAGE_TYPE::M_ERROR);
      continue;
    }
    #endif

    FT_Bitmap& charBitmap = slot->bitmap;
    FT_Glyph_Metrics fftMetrics = face->glyph->metrics;

    Glyph glyph;
    glyph.x = fftMetrics.horiBearingX / 64;
    glyph.y = -fftMetrics.horiBearingY / 64;
    glyph.x2 = fftMetrics.horiBearingX / 64 + charBitmap.width;
    glyph.y2 = -fftMetrics.horiBearingY / 64 + charBitmap.rows;
    glyph.tx = g.x() / double(w);
    glyph.ty = g.y() / double(h);
    glyph.tx2 = (g.x() + charBitmap.width) / double(w);
    glyph.ty2 = (g.y() + charBitmap.rows) / double(h);
    glyph.xs = face->glyph->linearHoriAdvance / 65536; // more fixed point conversion

    // Copy our glyph into the glyph range
    g.range->glyphs[g.character - g.range->start] = glyph;

    // write glyph to the texture
    for (unsigned y = 0; y < charBitmap.rows; ++y) {
      for (unsigned x = 0; x < charBitmap.width; ++x) {
        unsigned index = (y * charBitmap.width + x);
        unsigned index_out = ((y + g.y()) * w + (x + g.x())) * 4;
        pxdata[index_out] = pxdata[index_out + 1] = pxdata[index_out + 2] = 255;
        pxdata[index_out + 3] =  charBitmap.buffer[index];
      }
    }
  }

  yoffset = face->glyph->linearVertAdvance / 65536;
  height = face->glyph->linearVertAdvance / 65536;

  delete rootNode;
  FT_Done_Face(face);

  return pxdata;
}
}

namespace enigma_user {
void font_add_search_path(std::string path) {
  fontSearchPaths.push_back(path);
  #if __cplusplus > 201402L
  if (fs::exists(path)) {
    for (auto& p: fs::recursive_directory_iterator(fs::path(path))) {
      if (stringtolower(p.path().extension()) == ".ttf") {
        FT_Face face;
        FT_Error error;
        error = FT_New_Face(enigma::FontManager::GetLibrary(), p.path().c_str(), 0, &face );

        if (error != 0) {
          DEBUG_MESSAGE("Error opening font: " + p.path().string(), MESSAGE_TYPE::M_ERROR);
        }
        
         FT_SfntName name;
         FT_Get_Sfnt_Name(face, TT_NAME_ID_FULL_NAME, &name);
         std::string fontName(reinterpret_cast<char*>(name.string), name.string_len);
         fontNames[fontName] = p.path();
         std::cerr << fontName << std::endl;
      }
    }
  } else DEBUG_MESSAGE("Freetype warning searching non-existing directory: " + path, MESSAGE_TYPE::M_WARNING);
  #endif
}

std::string font_find(std::string name, bool bold, bool italic, bool exact_match) {
  std::string fullName = name + ((bold) ? " Bold" : "") + ((italic) ? " Italic" : "");
  if (fontNames.count(fullName) > 0) {
     return fontNames[fullName];
  } else if (!exact_match) {
    // search for base name
    if (fontNames.count(name) > 0) return fontNames[name]; 
    // default to first font we have //TODO: we should probabably include some font to use as the default
    return fontNames.begin()->second;
  }
  return "";
}

} //namespace enigma_user
