/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_DX11TEXTURESTRUCT_H
#define ENIGMA_DX11TEXTURESTRUCT_H

#include "Direct3D11Headers.h"

#include <vector>
using std::vector;

struct TextureStruct {
  ID3D11Texture2D *texture;
  ID3D11ShaderResourceView *view;
  unsigned width,height;
	unsigned fullwidth,fullheight;

  TextureStruct(ID3D11Texture2D *ntexture, ID3D11ShaderResourceView *view): texture(ntexture), view(view) {

  }

  ~TextureStruct() {
    if (texture != NULL) {
      texture->Release();
      texture = NULL;
    }
  }

};
extern vector<TextureStruct*> textureStructs;

#endif
