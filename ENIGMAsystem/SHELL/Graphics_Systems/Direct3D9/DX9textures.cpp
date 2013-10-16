/** Copyright (C) 2013 Robert B. Colton
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

#include <stdio.h>
#include "Direct3D9Headers.h"
#include "Bridges/General/DX9Device.h"
#include <string.h>
//using std::string;
#include "../General/GStextures.h"
#include "DX9TextureStruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "DX9binding.h"

vector<GmTexture*> GmTextures(0);

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
}

GmTexture::GmTexture(LPDIRECT3DTEXTURE9 gTex)
{
	gTexture = gTex;
}

GmTexture::~GmTexture()
{

}

LPDIRECT3DTEXTURE9 get_texture(int texid) {
  return (size_t(texid) >= GmTextures.size())? NULL : GmTextures[texid]->gTexture;
}

inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

namespace enigma
{
  int graphics_create_texture(int fullwidth, int fullheight, void* pxdata, bool isfont)
  {
    LPDIRECT3DTEXTURE9 texture = NULL;
	HRESULT hr;

	hr = d3ddev->CreateTexture(
		fullwidth,
		fullheight,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&texture,
		0
	);

	D3DLOCKED_RECT rect;

	texture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);

	unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
	for(int x = 0; x < fullwidth * fullheight * 4; x += 4){
		((unsigned char*)dest)[x]  =((unsigned char*)pxdata)[x + 2];   //A
		((unsigned char*)dest)[x+1]=((unsigned char*)pxdata)[x + 1];   //R
		((unsigned char*)dest)[x+2]=((unsigned char*)pxdata)[x];       //G
		((unsigned char*)dest)[x+3]=((unsigned char*)pxdata)[x + 3];   //B
	}

	texture->UnlockRect(0);

	GmTexture* gmTexture = new GmTexture(texture);
	gmTexture->isFont = isfont;
    GmTextures.push_back(gmTexture);
    return GmTextures.size()-1;
  }

  int graphics_duplicate_texture(int tex)
  {

  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {

  }

// V when this is called its passing the Gluint, but the Gluint is also stored by my GmTexture struct
  void graphics_delete_texture(int tex)
  {
    delete GmTextures[tex];
  }

  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {

  }
}

namespace enigma_user
{

void texture_set_enabled(bool enable)
{

}

void texture_set_interpolation(int enable)
{
	d3ddev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	d3ddev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	d3ddev->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{

}

double texture_get_width(int texid)
{

}

double texture_get_height(int texid)
{

}

int texture_get_texel_width(int texid)
{

}

int texture_get_texel_height(int texid)
{

}

void texture_set_stage(int stage, int texid) {
	d3ddev->SetTexture(stage, enigma::bound_texture = get_texture(texid));
}

void texture_set_repeat(bool repeat)
{
	enigma::interpolate_textures = repeat;
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
}

void texture_set_repeat(int texid, bool repeat)
{

}

void texture_set_wrap(int texid, bool wrapr, bool wraps, bool wrapt)
{

}

void texture_preload(int texid)
{

}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{

}

void texture_set_border(int texid, int r, int g, int b, double a)
{

}

void texture_set_swizzle(int texid, int r, int g, int b, double a)
{

}

void texture_set_levelofdetail(int texid, double minlod, double maxlod, int maxlevel)
{

}

void texture_mipmapping_filter(int texid, int filter)
{

}

void texture_mipmapping_generate(int texid, int levels)
{

}

bool  texture_anisotropy_supported()
{

}

float texture_anisotropy_maxlevel()
{

}

void  texture_anisotropy_filter(int texid, gs_scalar levels)
{

}

bool  texture_multitexture_supported()
{

}

void texture_multitexture_enable(bool enable)
{

}

}

