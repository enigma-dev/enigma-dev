/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
*** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_BACKGROUND_INTERNAL_H
#define ENIGMA_BACKGROUND_INTERNAL_H

#include "AssetArray.h"
#include "rect.h"
#include "Universal_System/scalar.h"

#include <string>

namespace enigma {
  
using TexRect = Rect<gs_scalar>;
  
struct Background {
  Background(unsigned w = 0, unsigned h = 0, int tex = -1, bool ts = false, 
    unsigned tw = 0, unsigned th = 0, int ho = 0, int vo = 0, int hs = 0, int vs = 0) : 
    width(w), 
    height(h), 
    textureID(tex),
    textureBounds({0, 0, 1, 1}), 
    isTileset(ts), 
    tileWidth(tw),
    tileHeight(th),
    hOffset(ho),
    vOffset(vo),
    hSep(hs),
    vSep(vs) {}
  Background(const Background& b, bool duplicateTexture = true);
  void FreeTexture();
  
  unsigned width, height;
  int textureID;
  TexRect textureBounds;
  bool isTileset;
  
  unsigned tileWidth, tileHeight;
  int hOffset, vOffset;
  int hSep, vSep;
  
  // AssArray mandatory
  static const char* getAssetTypeName() { return "Background"; }
  bool isDestroyed() const { return _destroyed; }
  void destroy() { _destroyed = true; }
  
protected:
  bool _destroyed = false;
};

extern AssetArray<Background> backgrounds;

} //namespace enigma


#endif //ENIGMA_BACKGROUND_INTERNAL_H
