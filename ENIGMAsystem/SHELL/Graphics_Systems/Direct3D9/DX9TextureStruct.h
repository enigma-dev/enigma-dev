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

#ifndef DX9_TEXTURESTRUCT__H
#define DX9_TEXTURESTRUCT__H

#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"

#include <vector>
using std::vector;

struct TextureStruct {
	LPDIRECT3DTEXTURE9 gTexture;
	bool isFont;
	unsigned width,height;
	unsigned fullwidth,fullheight;
  
  // for the purpose of restoring the texture
  D3DSURFACE_DESC backupdesc;
  
	TextureStruct(LPDIRECT3DTEXTURE9 gTex) {
		gTexture = gTex;
	}
  
  void Release() {
    if (gTexture != NULL) {
      gTexture->Release();
      gTexture = NULL;
    }
  }
	
	~TextureStruct() {
		Release();
	}
  
  void OnDeviceLost() {
    // backup texture data
    gTexture->GetLevelDesc(0, &backupdesc);
    Release();
  }
  
  void OnDeviceReset() {
    // restore texture data
    d3dmgr->CreateTexture(backupdesc.Width, backupdesc.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &gTexture, NULL);
   // delete &backupdesc;
  }
};
extern vector<TextureStruct*> textureStructs;

LPDIRECT3DTEXTURE9 get_texture(int texid);

#endif // DX9_TEXTURESTRUCT__H
