#include "fonts/fonts.h"
#include "include.h"
#include "Universal_System/Resources/fonts_internal.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>
#include <filesystem>

#include "fonts/fonts.cpp"

using namespace enigma::fonts;

namespace enigma_user {

int font_add(std::string name, unsigned size, bool bold, bool italic, unsigned first, unsigned last) {
  if (last < first) {
    DEBUG_MESSAGE("Freetype error invalid glyph range last < first", MESSAGE_TYPE::M_USER_ERROR);
    return -1;
  }
  
  if (std::filesystem::path(name).extension().empty()) {
    name = font_find(name, bold, italic, false);
    if (name.empty()) return -1;
  }

  RawFont rawFont;
  rawFont.ranges = {GlyphRange(first, last)};

  if (!font_load(name, size, rawFont))
    return -1;

  unsigned w, h; // size of texture 
  unsigned char* pxdata = font_pack(rawFont, w, h);
  if (pxdata == nullptr)
    return -1;

  SpriteFont fnt(name, rawFont.name, size, bold, italic, rawFont.lineHeight, rawFont.yOffset, w ,h, pxdata, rawFont.ranges);
  fnt.texture.init();
  return sprite_fonts.add(std::move(fnt));
}

} //namespace enigma_user
