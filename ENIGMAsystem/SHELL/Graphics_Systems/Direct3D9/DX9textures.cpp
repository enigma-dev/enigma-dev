/** Copyright (C) 2013-2014,2019 Robert B. Colton
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

#include "DX9textures_impl.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"

#include <string.h> // for memcpy
using namespace enigma::dx9;

namespace {

void surface_copy_to_ram(IDirect3DSurface9 **src, IDirect3DSurface9 **dest, const RECT& rect, bool& subsurf) {
  D3DSURFACE_DESC desc;
  (*src)->GetDesc(&desc);

  const unsigned int width = rect.right-rect.left, height = rect.bottom-rect.top;

  // lockable render target is only faster when requested area is less than half of total area
  // this should make sense intuitively since the lockable render target will have to copy twice
  // lockable render target is also the only way to get back non-RT video memory texture
  if (width*height < (desc.Width*desc.Height)/2 || desc.Usage != D3DUSAGE_RENDERTARGET) {
    subsurf = true;
    d3ddev->CreateRenderTarget(width, height, desc.Format, D3DMULTISAMPLE_NONE, 0, true, dest, NULL);
    d3ddev->StretchRect(*src, &rect, *dest, NULL, D3DTEXF_NONE);
  } else {
    subsurf = false;
    d3ddev->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, dest, NULL);
    d3ddev->GetRenderTargetData(*src, *dest);
  }
}

unsigned char* surface_copy_pixels(LPDIRECT3DSURFACE9 pBuffer, int x, int y, int width, int height) {
  LPDIRECT3DSURFACE9 pRamBuffer=nullptr;

  RECT rect = {(LONG)x, (LONG)y, (LONG)(x+width), (LONG)(y+height)};

  bool subsurf = false;

  // use system memory surface to copy texture
  D3DSURFACE_DESC desc;
  pBuffer->GetDesc(&desc);
  if (desc.Pool == D3DPOOL_DEFAULT) {
    surface_copy_to_ram(&pBuffer,&pRamBuffer,rect,subsurf);
    pBuffer = pRamBuffer;
  }

  unsigned char* ret = new unsigned char[width*height*4];

  D3DLOCKED_RECT lock;
  pBuffer->LockRect(&lock, subsurf?NULL:&rect, D3DLOCK_READONLY);
  for (int i = 0; i < height; ++i) {
    memcpy((void*)((intptr_t)ret + i * width * 4), (void*)((intptr_t)lock.pBits + i * lock.Pitch), width * 4);
  }
  pBuffer->UnlockRect();

  if (pRamBuffer) pRamBuffer->Release();

  return ret;
}

} // namespace anonymous

namespace enigma {

unsigned char* graphics_copy_screen_pixels(int x, int y, int width, int height, bool* flipped) {
  if (flipped) *flipped = false;

  LPDIRECT3DSURFACE9 pBackBuffer;
  d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  return surface_copy_pixels(pBackBuffer, x, y, width, height);
}

unsigned char* graphics_copy_screen_pixels(unsigned* fullwidth, unsigned* fullheight, bool* flipped) {
  LPDIRECT3DSURFACE9 pBackBuffer;
  d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  D3DSURFACE_DESC desc;
  pBackBuffer->GetDesc(&desc);

  const int fw = desc.Width, fh = desc.Height;

  *fullwidth = fw, *fullheight = fh;
  return graphics_copy_screen_pixels(0,0,fw,fh,flipped);
}

LPDIRECT3DTEXTURE9 get_texture_peer(int texid) {
  return (size_t(texid) >= textures.size() || texid < 0) ? NULL : static_cast<DX9Texture*>(textures[texid].get())->peer;
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, int fullwidth, int fullheight, unsigned char* pxdata) {
  DX9Texture* d3dtex = static_cast<DX9Texture*>(enigma::textures[texture].get());
  auto peer = d3dtex->peer;

  LPDIRECT3DSURFACE9 pBuffer,pRamBuffer=nullptr;
  peer->GetSurfaceLevel(0,&pBuffer);

  // use system memory surface to update texture
  D3DSURFACE_DESC desc;
  pBuffer->GetDesc(&desc);
  if (desc.Pool == D3DPOOL_DEFAULT) {
    d3ddev->CreateOffscreenPlainSurface(width, height, desc.Format, D3DPOOL_SYSTEMMEM, &pRamBuffer, NULL);
  }

  RECT rect = {(LONG)x, (LONG)y, (LONG)(x+width), (LONG)(y+height)};
  D3DLOCKED_RECT lock;
  (pRamBuffer?pRamBuffer:pBuffer)->LockRect(&lock, pRamBuffer?NULL:&rect, D3DLOCK_DISCARD);
  for (int i = 0; i < height; ++i) {
    memcpy((void*)((intptr_t)lock.pBits + i * lock.Pitch), (void*)((intptr_t)pxdata + i * fullwidth * 4), width * 4);
  }
  (pRamBuffer?pRamBuffer:pBuffer)->UnlockRect();

  if (pRamBuffer) {
    const POINT p = {x,y};
    d3ddev->UpdateSurface(pRamBuffer,NULL,pBuffer,&p);
    pRamBuffer->Release();
  }
}

int graphics_create_texture(unsigned width, unsigned height, void* pxdata, bool mipmap, unsigned* fullwidth, unsigned* fullheight)
{
  LPDIRECT3DTEXTURE9 texture = NULL;

  DWORD usage = mipmap?D3DUSAGE_AUTOGENMIPMAP:0;
  d3ddev->CreateTexture(fullwidth, fullheight, 1, usage, D3DFMT_A8R8G8B8, Direct3D9Managed ? D3DPOOL_MANAGED : D3DPOOL_DEFAULT, &texture, 0);

  if (mipmap) texture->GenerateMipSubLevels();
  
  unsigned fw, fh;
  if (fullwidth == nullptr) fullwidth = &fw; 
  if (fullheight == nullptr) fullheight = &fh;
  
  *fullwidth  = nlpo2dc(width)+1;
  *fullheight = nlpo2dc(height)+1;
  
  RawImage padded = image_pad((unsigned char*)pxdata, width, height, *fullwidth, *fullheight);

  const int id = textures.size();
  textures.push_back(std::make_unique<DX9Texture>(texture));
  auto& textureStruct = textures.back();
  textureStruct->width = width;
  textureStruct->height = height;
  textureStruct->fullwidth = *fullwidth;
  textureStruct->fullheight = *fullheight;
  if (padded.pxdata != nullptr) graphics_push_texture_pixels(id, 0, 0, width, height, *fullwidth, *fullheight, padded.pxdata);
  return id;
}

void graphics_delete_texture(int texid) {
  if (texid >= 0) {
    DX9Texture* texture = static_cast<DX9Texture*>(enigma::textures[texid].get());
    texture->peer->Release(), texture->peer = NULL;
  }
}

unsigned char* graphics_copy_texture_pixels(int texture, int x, int y, int width, int height) {
  DX9Texture* d3dtex = static_cast<DX9Texture*>(enigma::textures[texture].get());
  auto peer = d3dtex->peer;
  LPDIRECT3DSURFACE9 pBuffer;
  peer->GetSurfaceLevel(0,&pBuffer);
  return surface_copy_pixels(pBuffer, x, y, width, height);
}

unsigned char* graphics_copy_texture_pixels(int texture, unsigned* fullwidth, unsigned* fullheight) {
  DX9Texture* d3dtex = static_cast<DX9Texture*>(enigma::textures[texture].get());
  const unsigned fw = d3dtex->fullwidth, fh = d3dtex->fullheight;
  *fullwidth = fw, *fullheight = fh;
  return graphics_copy_texture_pixels(texture, 0, 0, fw, fh);
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {
  graphics_push_texture_pixels(texture, x, y, width, height, width, height, pxdata);
}

void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {
  graphics_push_texture_pixels(texture, 0, 0, width, height, pxdata);
}

} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  draw_batch_flush(batch_flush_deferred);
  // Deprecated in ENIGMA and GM: Studio
  enigma::get_texture_peer(texid)->SetPriority(prio);
}

bool texture_mipmapping_supported()
{
  D3DCAPS9 caps;
  d3ddev->GetDeviceCaps(&caps);
  return caps.TextureCaps & D3DPTEXTURECAPS_MIPMAP;
}

bool texture_anisotropy_supported()
{
  D3DCAPS9 caps;
  d3ddev->GetDeviceCaps(&caps);
  return caps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY;
}

float texture_anisotropy_maxlevel()
{
  D3DCAPS9 caps;
  d3ddev->GetDeviceCaps(&caps);
  return caps.MaxAnisotropy;
}

} // namespace enigma_user
