#include "strings_util.h"
#include "fonts.h"

#ifdef ENIGMA_GAME
  #include "Widget_Systems/widgets_mandatory.h"
#else
  #include <iostream>
  #if !defined(DEBUG_MESSAGE)
    #define DEBUG_MESSAGE(msg, severity) std::cout << msg << std::endl;
  #endif
#endif

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_IDS_H
#include FT_SFNT_NAMES_H

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <filesystem>
namespace fs = std::filesystem;
  
using namespace enigma::rect_packer;

namespace enigma {
namespace fonts {

static std::vector<std::string> fontSearchPaths;
static std::unordered_map<std::string, std::string> fontNames;
static std::unordered_map<std::string, std::string> fontFamilies;

// FreeType supports a few more obscure font types but I'm not sure to what extent so I put what looked like main ones
// https://www.freetype.org/freetype2/docs/index.html for full list of supported fomats
static const std::unordered_set<std::string> supportedExtensions = {
  ".ttf",  // truetype font
  ".ttc",  // truetype collection
  ".otf",  // opentype font
  ".otc",  // opentype collection
  ".fnt",  // windows font
  ".woff", // web open font format
  ".woff2",
};

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

  FT_SfntName name;
  FT_Get_Sfnt_Name(face, TT_NAME_ID_FULL_NAME, &name);
  rawFont.name = std::string(reinterpret_cast<char*>(name.string), name.string_len);
  rawFont.name.erase(std::remove_if(rawFont.name.begin(), rawFont.name.end(), [](auto& c) { return (static_cast<int>(c) > 127 || static_cast<int>(c) < 32); } ), rawFont.name.end());

  // GameMaker has always generated fonts at 96 dpi, default Windows dpi since 1980s
  error = FT_Set_Char_Size( face, size * 64, 0, 96, 0); // 96 dpi, 64 is 26.6 fixed point conversion

  if (error != 0)
    return false;

  slot = face->glyph;

  // sum of all glyphs in all ranges
  size_t gcount = 0;
  for (const GlyphRange& range : rawFont.ranges) gcount += range.glyphs.size();
 
  // vector of all (raw) glyphs used later for sorting
  rawFont.glyphs.resize(gcount);
   
  // loop over all ranges
  size_t currOffset = 0;
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
       rg.dimensions = pvrect(0, 0, charBitmap.width, charBitmap.rows, -1);
       rg.range = &range;
       rg.horiBearingX = fftMetrics.horiBearingX / 64;
       rg.horiBearingY = fftMetrics.horiBearingY / 64;
       rg.linearHoriAdvance = face->glyph->linearHoriAdvance / 65536; // more fixed point conversion

       if (charBitmap.width > 0 && charBitmap.rows > 0) {
         rg.pxdata = new unsigned char[charBitmap.width * charBitmap.rows];
       
         // copy glyph to texture
         for (unsigned y = 0; y < charBitmap.rows; ++y) {
           for (unsigned x = 0; x < charBitmap.width; ++x) {
             unsigned index = (y * charBitmap.pitch + x);
             rg.pxdata[index] = charBitmap.buffer[index];
            }
          }
       }
       
       rawFont.glyphs[currOffset++] = rg;
    }
  }

  // FIXME: idk how GM calculates the line height 
  // this produces similar results for me but freetype docs recommends face->size->metrics.height / 64 but thats way off compared to GM
  rawFont.lineHeight = rawFont.yOffset = face->glyph->linearVertAdvance / 65536;

  FT_Done_Face(face);
  
  return true;
}

unsigned char* font_pack(RawFont& font, unsigned& textureW, unsigned& textureH) {
 
  if (!pack_rectangles(font.glyphs, textureW, textureH))
    return nullptr;

  // intialize blank image (the caller is responisible for deleting pxdata)
  unsigned char* pxdata = new unsigned char[textureW * textureH];
  for (unsigned y = 0; y < textureH; ++y) {
    for (unsigned x = 0; x < textureW; ++x) {
      unsigned index = y * textureW + x;
      pxdata[index] = 0;
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

  return pxdata;
}

} // namespace fonts
} // namespace enigma

using namespace enigma::fonts;

namespace enigma_user {
  
static inline void font_add_search_path_helper(const fs::path& p) {
   
  auto it = supportedExtensions.find(ToLower(p.extension()));
  if (it != supportedExtensions.end()) {
    FT_Face face;
    FT_Error error;
    error = FT_New_Face(enigma::fonts::FontManager::GetLibrary(), p.c_str(), 0, &face );

    if (error != 0)
      DEBUG_MESSAGE("Error opening font: " + p.string(), MESSAGE_TYPE::M_ERROR);
    
     FT_SfntName name;
     FT_Get_Sfnt_Name(face, TT_NAME_ID_FULL_NAME, &name);
     std::string fontName(reinterpret_cast<char*>(name.string), name.string_len);
     fontName.erase(std::remove_if(fontName.begin(), fontName.end(), [](auto& c) { return (static_cast<int>(c) > 127 || static_cast<int>(c) < 32); } ), fontName.end());
     fontNames[fontName] = p;

     FT_SfntName family;
     FT_Get_Sfnt_Name(face, TT_NAME_ID_FONT_FAMILY, &family);
     std::string fontFamily(reinterpret_cast<char*>(family.string), family.string_len);
     fontFamily.erase(std::remove_if(fontFamily.begin(), fontFamily.end(), [](auto& c) { return (static_cast<int>(c) > 127 || static_cast<int>(c) < 32); } ), fontFamily.end());
     fontFamilies[fontFamily] = p;
  }
}

void font_add_search_path(std::string path, bool recursive) {
  if (std::find(fontSearchPaths.begin(), fontSearchPaths.end(), path) == fontSearchPaths.end()) { 
    fontSearchPaths.emplace_back(path);
    if (fs::exists(path)) {
      if (recursive) {
        for (auto& p: fs::recursive_directory_iterator(fs::path(path)))
          font_add_search_path_helper(p.path());
      } else {
        for (auto& p: fs::directory_iterator(fs::path(path)))
          font_add_search_path_helper(p.path());
      }
    } else DEBUG_MESSAGE("Freetype warning searching non-existing directory: " + path, MESSAGE_TYPE::M_WARNING);
  }
}

std::string font_find(std::string name, bool bold, bool italic, bool exact_match) {
  // LGM default font
  if (name == "Dialog.plain")
    name = "Arial";
  
  std::string fullName = name + ((bold) ? " Bold" : "") + ((italic) ? " Italic" : "");
  if (!bold && !italic) fullName += " Regular";
  
  if (fontNames.count(fullName) > 0) {
     return fontNames[fullName];
  } else if (!exact_match) {
    std::string ret;
    // search for base font name
    if (fontNames.count(name) > 0) ret = fontNames[name];
    // search for name by font family
    if (fontFamilies.count(name) > 0) ret = fontFamilies[name];
    
    if (!ret.empty()) {
      DEBUG_MESSAGE("Freetype warning couild not find exact match for font: \"" + fullName + "\" using best match: " + ret, MESSAGE_TYPE::M_WARNING);
      return ret;
    }
  }
  
  // GM's default is Arial
  if (name != "Arial") {
    DEBUG_MESSAGE("Freetype error could not find font: \"" + name + "\" attempting fallback to Arial", MESSAGE_TYPE::M_WARNING);
    std::string arial = font_find("Arial", false, false, false);
    if (!arial.empty()) return arial;
  }
  
  DEBUG_MESSAGE("Freetype error could not find Arial falling back to fist available font: " + fontNames.begin()->second, MESSAGE_TYPE::M_WARNING);
  
  return fontNames.begin()->second; // Couldn't find arial. Guess well try to give it some kind of font
}

} //namespace enigma_user
