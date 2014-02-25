/** Copyright (C) 2013-2014 Robert B. Colton
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
#include "Bridges/General/DX9Context.h"
#include <string.h>
//using std::string;
#include "../General/GStextures.h"
#include "DX9TextureStruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"

vector<TextureStruct*> textureStructs(0);

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
}

LPDIRECT3DTEXTURE9 get_texture(int texid) {
  return (size_t(texid) >= textureStructs.size() || texid < 0) ? NULL : textureStructs[texid]->gTexture;
}

inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

//NOTE: We should probably consider switching to BGRA internally since this is what most image formats utilize, including
//bmp,jpg,tga, and optimized PNG. Most x86 graphics hardware and API's also use BGRA internally making it much more optimal
//and it would also clean up our code below by keeping us from having to reorder the bytes. This exposes a major inefficiency
//in our code and it also applies to OpenGL. - Robert B. Colton

namespace enigma
{
  int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool isfont)
  {
    LPDIRECT3DTEXTURE9 texture = NULL;

	d3dmgr->CreateTexture(fullwidth, fullheight, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &texture, 0);

	D3DLOCKED_RECT rect;

	texture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);

	unsigned char* dest = static_cast<unsigned char*>(rect.pBits);
	for (unsigned x = 0; x < fullwidth * fullheight * 4; x += 4) {
		((unsigned char*)dest)[x]  =((unsigned char*)pxdata)[x + 2];   //A
		((unsigned char*)dest)[x+1]=((unsigned char*)pxdata)[x + 1];   //R
		((unsigned char*)dest)[x+2]=((unsigned char*)pxdata)[x];       //G
		((unsigned char*)dest)[x+3]=((unsigned char*)pxdata)[x + 3];   //B
	}

	texture->UnlockRect(0);
	delete[] dest;

	TextureStruct* textureStruct = new TextureStruct(texture);
	textureStruct->isFont = isfont;
	textureStruct->width = width;
	textureStruct->height = height;
	textureStruct->fullwidth = fullwidth;
	textureStruct->fullheight = fullheight;
    textureStructs.push_back(textureStruct);
    return textureStructs.size()-1;
  }

  int graphics_duplicate_texture(int tex)
  {
    unsigned w, h, fw, fh;
	w = textureStructs[tex]->width;
	h = textureStructs[tex]->height;
	fw = textureStructs[tex]->fullwidth;
	fh = textureStructs[tex]->fullheight;
	
	D3DLOCKED_RECT rect;

	textureStructs[tex]->gTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
	unsigned char* bitmap = new unsigned char[(fw*fh*4)];
	for (unsigned x = 0; x < fw * fh * 4; x += 4){
		bitmap[x]   = ((unsigned char*)rect.pBits)[x + 2];   //R B
		bitmap[x+1] = ((unsigned char*)rect.pBits)[x + 1];   //G G
		bitmap[x+2] = ((unsigned char*)rect.pBits)[x];       //B R
		bitmap[x+3] = ((unsigned char*)rect.pBits)[x + 3];   //A A
	}
	textureStructs[tex]->gTexture->UnlockRect(0);
	
    unsigned dup_tex = graphics_create_texture(w, h, fw, fh, bitmap, textureStructs[tex]->isFont);
    delete[] bitmap;
    return dup_tex;
  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {
    unsigned w, h, fw, fh, size;
	w = textureStructs[tex]->width;
	h = textureStructs[tex]->height;
	fw = textureStructs[tex]->fullwidth;
	fh = textureStructs[tex]->fullheight;
    size = (fh<<(lgpp2(fw)+2))|2;
	
	D3DLOCKED_RECT rect;
	
	textureStructs[copy_tex]->gTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
	unsigned char* bitmap_copy = static_cast<unsigned char*>(rect.pBits);
	textureStructs[copy_tex]->gTexture->UnlockRect(0);

	textureStructs[tex]->gTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);
	unsigned char* bitmap_orig = static_cast<unsigned char*>(rect.pBits);
	for (int i = 3; i < size; i += 4)
        ((unsigned char*)bitmap_orig)[i] = (bitmap_copy[i-3] + bitmap_copy[i-2] + bitmap_copy[i-1])/3;
	textureStructs[tex]->gTexture->UnlockRect(0);
	
    delete[] bitmap_orig;
    delete[] bitmap_copy;
  }

  void graphics_delete_texture(int tex)
  {
	 textureStructs.erase(textureStructs.begin() + tex);
  }

  unsigned char* graphics_get_texture_rgba(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
  {
    *fullwidth = textureStructs[texture]->fullwidth;
	*fullheight = textureStructs[texture]->fullheight;

	D3DLOCKED_RECT rect;

	textureStructs[texture]->gTexture->LockRect( 0, &rect, NULL, D3DLOCK_DISCARD);

	unsigned char* bitmap = new unsigned char[((*fullwidth)*(*fullheight)*4)];
	for (unsigned x = 0; x < (*fullwidth) * (*fullheight) * 4; x += 4){
		bitmap[x]   = ((unsigned char*)rect.pBits)[x + 2];   //R B
		bitmap[x+1] = ((unsigned char*)rect.pBits)[x + 1];   //G G
		bitmap[x+2] = ((unsigned char*)rect.pBits)[x];       //B R
		bitmap[x+3] = ((unsigned char*)rect.pBits)[x + 3];   //A A
	}

	textureStructs[texture]->gTexture->UnlockRect(0);

    return bitmap;
  }
}

namespace enigma_user
{

void texture_set_enabled(bool enable)
{

}

void texture_set_interpolation(int enable)
{
	enigma::interpolate_textures = enable;
	d3dmgr->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	d3dmgr->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	d3dmgr->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{

}

gs_scalar texture_get_width(int texid) {
	return textureStructs[texid]->width / textureStructs[texid]->fullwidth;
}

gs_scalar texture_get_height(int texid)
{
	return textureStructs[texid]->fullheight / textureStructs[texid]->fullheight;
}

unsigned texture_get_texel_width(int texid)
{
	return textureStructs[texid]->width;
}

unsigned texture_get_texel_height(int texid)
{
	return textureStructs[texid]->height;
}

void texture_set(int texid) {
	if (texid == -1) { d3dmgr->SetTexture(0, NULL); return; }
	d3dmgr->SetTexture(0, get_texture(texid));
	d3dmgr->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

void texture_set_stage(int stage, int texid) {
	d3dmgr->SetTexture(stage, get_texture(texid));
	d3dmgr->SetTextureStageState(stage,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
}

void texture_reset() {
	d3dmgr->SetTexture(0, NULL);
}

void texture_set_repeat(bool repeat)
{
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSU, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSV, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
	d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSW, repeat?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP );
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

