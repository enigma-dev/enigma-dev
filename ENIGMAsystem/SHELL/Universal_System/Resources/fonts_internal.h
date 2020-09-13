/** Copyright (C) 2008 Josh Ventura
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

//FIXME: this should be ifdef shellmain but enigmas in a sorry state
#ifdef JUST_DEFINE_IT_RUN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_FONTS_INTERNAL_H
#define ENIGMA_FONTS_INTERNAL_H

#include "fonts/fonts.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "AssetArray.h"

#include <string>
#include <vector>

namespace enigma {
namespace fonts {
  GlyphMetrics findGlyph(const fonts::SpriteFont& fnt, uint32_t character);
  extern AssetArray<fonts::SpriteFont, -1> sprite_fonts;
  extern std::vector<std::pair<int, SpriteFont>> exeFonts;
  extern int exeFontsMaxID;
} // namespace fonts  
} //namespace enigma

#endif //ENIGMA_FONTS_INTERNAL_H
