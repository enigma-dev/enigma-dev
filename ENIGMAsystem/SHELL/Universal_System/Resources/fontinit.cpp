/** Copyright (C) 2008-2011 Josh Ventura
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

#include "backgrounds_internal.h"
#include "fonts_internal.h"
#include "libEGMstd.h"
#include "resinit.h"
#include "Universal_System/zlib.h"

#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <cstring>
#include <cstdio>
#include <string>

#include <iostream>

namespace enigma {

using namespace fonts;

unsigned char* exe_get_pxdata(FILE* exe, unsigned expectedSize) {
  
  unsigned int compressedSize;
  if (!fread(&compressedSize, 4, 1, exe)) {
    DEBUG_MESSAGE("Failed to load size of compressed pixel data", MESSAGE_TYPE::M_ERROR);
  }
  
  unsigned char* cpxdata = new unsigned char[compressedSize];
  if (!cpxdata) {
    DEBUG_MESSAGE("Failed to load pixel data: Cannot allocate enough memory " + enigma_user::toString(compressedSize), MESSAGE_TYPE::M_ERROR);
    return nullptr;
  }
  
  unsigned int sz2 = fread(cpxdata, 1, compressedSize, exe);
  if (compressedSize != sz2) {
    DEBUG_MESSAGE("Failed to load pixel data: Data is truncated before exe end. Read " + 
      enigma_user::toString(sz2) + " out of expected " + enigma_user::toString(compressedSize), MESSAGE_TYPE::M_ERROR);
    delete[] cpxdata;
    return nullptr;
  }

  unsigned char* pxdata=new unsigned char[expectedSize];
  if (zlib_decompress(cpxdata, compressedSize, expectedSize, pxdata) != (int)expectedSize) {
    DEBUG_MESSAGE("Failed to load pixel data: Data does not match expected size", MESSAGE_TYPE::M_ERROR);
    delete[] cpxdata;
    delete[] pxdata;
    return nullptr;
  }
  
  delete[] cpxdata;
  return pxdata;
}

void exe_loadfonts(FILE* exe) {
  int nullhere;
  unsigned fontcount;

  if (!fread(&nullhere, 4, 1, exe)) return;
  if (memcmp(&nullhere, "FNT ", sizeof(int)) != 0) return;

  if (!fread(&fontcount, 4, 1, exe)) return;
  if (fontcount != exeFonts.size()) {
    DEBUG_MESSAGE("Resource data does not match up with game metrics. Unable to improvise.", MESSAGE_TYPE::M_ERROR);
    return;
  }

  sprite_fonts.resize(exeFontsMaxID + 1);
  for (const std::pair<int, SpriteFont>& index : exeFonts) {
    int id = index.first;
    sprite_fonts[id] = index.second;
    SpriteFont& font = sprite_fonts[id];
    font.texture.pxdata = exe_get_pxdata(exe, font.texture.width * font.texture.height);
    font.texture.init();
  }
}
}  //namespace enigma
