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

namespace enigma {
namespace fonts {
static std::vector<std::string> fontSearchPaths;
static std::map<std::string, std::string> fontNames;
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
    enigma_user::font_add_search_path(p, true);
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

bool font_load(std::string fName, unsigned size, RawFont& rawFont) {

  if (!enigma::fonts::FreeTypeAlive)
    return false;

  FT_Face face;
  FT_GlyphSlot slot;
  FT_Error error;

  error = FT_New_Face(enigma::fonts::FontManager::GetLibrary(), fName.c_str(), 0, &face );

  if (error != 0)
    return false;

  // GameMaker has always generated fonts at 96 dpi, default Windows dpi since 1980s
  error = FT_Set_Char_Size( face, size * 64, 0, 96, 0); // 96 dpi, 64 is 26.6 fixed point conversion

  if (error != 0)
    return false;

  slot = face->glyph;
  rawFont.lineHeight = face->size->metrics.height / 64;

  // sum of all glyphs in all ranges
  size_t gcount = 0;
  for (const GlyphRange& range : rawFont.ranges) gcount += range.glyphs.size();
 
  // vector of all (raw) glyphs used later for sorting
  rawFont.glyphs.resize(gcount);
   
  size_t currOffset = 0; // our location in the glyphInfo vector
  // loop over all ranges
  for (GlyphRange& range : rawFont.ranges) {
    // loop over each character per range
    for (size_t c = 0; c < range.size(); ++c) {
      // open character to get metrics and pixel data
      error = FT_Load_Char(face, range.start + c, FT_LOAD_RENDER);

      #if defined(DEBUG_MODE) || !defined(ENIGMA_GAME)
      if (error != 0)
        DEBUG_MESSAGE("Freetype error loading char: " + std::to_string(range.start + c), MESSAGE_TYPE::M_ERROR);
      #endif
      
       FT_Bitmap& charBitmap = slot->bitmap;
       FT_Glyph_Metrics fftMetrics = face->glyph->metrics;

       RawGlyph rg;
       rg.character = range.start + c;
       rg.index = currOffset + c;
       rg.dimensions = pvrect(0, 0, charBitmap.width, charBitmap.rows, -1);
       rg.range = &range;
       rg.horiBearingX = fftMetrics.horiBearingX / 64;
       rg.horiBearingY = fftMetrics.horiBearingY / 64;
       rg.linearHoriAdvance = face->glyph->linearHoriAdvance / 65536; // more fixed point conversion
       rg.pxdata = new unsigned char[charBitmap.width * charBitmap.rows /* 4*/];
       
       // copy glyph to texture
       for (unsigned y = 0; y < charBitmap.rows; ++y) {
         for (unsigned x = 0; x < charBitmap.width; ++x) {
           unsigned index = (y * charBitmap.width + x);
           rg.pxdata[index] = charBitmap.buffer[index];
          }
        }
       
       rawFont.glyphs[currOffset + c] = rg;
    }
    // done with one range so update offset
    currOffset += range.size();
  }

  FT_Done_Face(face);
  
  return true;
}

unsigned char* font_pack(RawFont& font, unsigned& textureW, unsigned& textureH) {

  // sort all glyphs by area
  std::sort(font.glyphs.begin(), font.glyphs.end(), [](const RawGlyph &a, const RawGlyph &b) {
    return (a.area() > b.area());
  });

  // Josh's rectpacker code needs access to an array so we copy all pvrects into one here 
  std::vector<pvrect> dimensions(font.glyphs.size());
  size_t currIndex = 0;
  for (RawGlyph& rg : font.glyphs) {
    dimensions[currIndex++] = rg.dimensions;
  }

  // insert the metrics and calculate a minimum (power of 2) texture size
  textureW = 64, textureH = 64; // starter size for our texture
  rectpnode* rootNode = new rectpnode(0, 0, textureW, textureH);
  for (std::vector<RawGlyph>::reverse_iterator ii = font.glyphs.rbegin(); ii != font.glyphs.rend();) {
    rectpnode* node = rninsert(rootNode, ii->index, dimensions.data());

    if (node) {
      rncopy(node, dimensions.data(), ii->index);
      ii++;
    } else {
      textureW > textureH ? textureH <<= 1 : textureW <<= 1,
      rootNode = expand(rootNode, textureW, textureH);
      if (!textureW or !textureH) return nullptr;
    }
  }

  // Copy our updated pvrects back to our glyphs
  currIndex = 0;
  for (RawGlyph& rg : font.glyphs) {
     rg.dimensions = dimensions[currIndex++];
  }

  // intialize blank image (the caller is responisible for deleting pxdata)
  unsigned char* pxdata = new unsigned char[textureW * textureH];
  for (unsigned y = 0; y < textureH; ++y) {
    for (unsigned x = 0; x < textureW; ++x) {
      unsigned index = y * textureW + x;
      pxdata[index] = 255;
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
    rg.range->glyphs[rg.character - rg.range->start] = glyph;

    // write glyph pixel data to the texture
    for (unsigned y = 0; y < rg.h(); ++y) {
      for (unsigned x = 0; x < rg.w(); ++x) {
        unsigned index = (y * rg.w() + x);
        unsigned index_out = ((y + rg.y()) * textureW + (x + rg.x()));
        pxdata[index_out] = rg.pxdata[index];
      }
    }
  }


  delete rootNode;
  return pxdata;
}

} // namespace fonts
} // namespace enigma

using namespace enigma::fonts;

namespace enigma_user {

#if __cplusplus > 201402L
static inline void font_add_search_path_helper(const fs::path& p) {
  if (stringtolower(p.extension()) == ".ttf") {
    FT_Face face;
    FT_Error error;
    error = FT_New_Face(enigma::fonts::FontManager::GetLibrary(), p.c_str(), 0, &face );

    if (error != 0)
      DEBUG_MESSAGE("Error opening font: " + p.string(), MESSAGE_TYPE::M_ERROR);
    
     FT_SfntName name;
     FT_Get_Sfnt_Name(face, TT_NAME_ID_FULL_NAME, &name);
     std::string fontName(reinterpret_cast<char*>(name.string), name.string_len);
     fontNames[fontName] = p;
  }
}
#endif

void font_add_search_path(std::string path, bool recursive) {
  fontSearchPaths.push_back(path);
  #if __cplusplus > 201402L
  if (fs::exists(path)) {
    if (recursive) {
      for (auto& p: fs::recursive_directory_iterator(fs::path(path)))
        font_add_search_path_helper(p.path());
    } else {
      for (auto& p: fs::directory_iterator(fs::path(path)))
        font_add_search_path_helper(p.path());
    }
  } else DEBUG_MESSAGE("Freetype warning searching non-existing directory: " + path, MESSAGE_TYPE::M_WARNING);
  
  #endif
}

std::string font_find(std::string name, bool bold, bool italic, bool exact_match) {
  std::string fullName = name + ((bold) ? " Bold" : "") + ((italic) ? " Italic" : "");
  if (fontNames.count(fullName) > 0) {
     return fontNames[fullName];
  } else if (!exact_match) {
    // search for base name (not italics / bold)
    if (fontNames.count(name) > 0) return fontNames[name]; 
  }
  // TODO: return a default font?
  return "";
}

} //namespace enigma_user
