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

#include "Bridges/General/DX9Context.h"
#include "DX9SurfaceStruct.h"
#include "DX9TextureStruct.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/background_internal.h"
#include "Collision_Systems/collision_types.h"

#include <iostream>
#include <cstddef>
#include <math.h>
#include <stdio.h> //for file writing (surface_save)

using namespace std;

namespace enigma {

vector<Surface*> Surfaces(0);
D3DCOLOR get_currentcolor();

} // namespace enigma

namespace enigma_user {

bool surface_is_supported()
{
  //TODO: Implement with IDirect3D9::CheckDeviceFormat
  return true;
}

int surface_create(int width, int height, bool depthbuffer, bool, bool)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	d3dmgr->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	enigma::Surface* surface = new enigma::Surface();
	TextureStruct* gmTexture = new TextureStruct(texture);
  textureStructs.push_back(gmTexture);
  //d3dmgr->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
  texture->GetSurfaceLevel(0,&surface->surf);
  surface->tex = textureStructs.size() - 1;
  surface->width = width; surface->height = height;
  enigma::Surfaces.push_back(surface);
  return enigma::Surfaces.size() - 1;
}

int surface_create_msaa(int width, int height, int levels)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	d3dmgr->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
	enigma::Surface* surface = new enigma::Surface();
	TextureStruct* gmTexture = new TextureStruct(texture);
  textureStructs.push_back(gmTexture);
  d3dmgr->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
  surface->tex = textureStructs.size() - 1;
  surface->width = width; surface->height = height;
  enigma::Surfaces.push_back(surface);
  return enigma::Surfaces.size() - 1;
}

LPDIRECT3DSURFACE9 pBackBuffer;

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surface,id);
  d3dmgr->device->GetRenderTarget(0, &pBackBuffer);
	d3dmgr->device->SetRenderTarget(0, surface->surf);

  d3d_set_projection_ortho(0, 0, surface->width, surface->height, 0);
}

void surface_reset_target()
{
  draw_batch_flush(batch_flush_deferred);

  //d3dmgr->ResetRenderTarget();
  d3dmgr->device->SetRenderTarget(0, pBackBuffer);
  pBackBuffer->Release();
  pBackBuffer = NULL;
}

int surface_get_target()
{

}

void surface_free(int id)
{
  get_surface(surf, id);
  delete surf;
}

bool surface_exists(int id)
{
  return !((id < 0) or (id > enigma::Surfaces.size()) or (enigma::Surfaces[id] == NULL));
}

int surface_get_texture(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->tex);
}

int surface_get_width(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->width);
}

int surface_get_height(int id)
{
  get_surfacev(surf,id,-1);
  return (surf->height);
}

int surface_getpixel(int id, int x, int y)
{
  get_surfacev(surface,id,-1);
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x > surface->width || y > surface->height) return 0;
  draw_batch_flush(batch_flush_deferred);

  LPDIRECT3DSURFACE9 pBuffer = surface->surf;
  d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  D3DSURFACE_DESC desc;
  pBackBuffer->GetDesc(&desc);

	D3DLOCKED_RECT rect;

	pBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	unsigned offset = y * rect.Pitch + x * 4;
	int ret = bitmap[offset + 1] | (bitmap[offset + 2] << 8) | (bitmap[offset + 3] << 16);
	pBuffer->UnlockRect();
	delete[] bitmap;

	return ret;
}

int surface_getpixel_ext(int id, int x, int y)
{
  get_surfacev(surface,id,-1);
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x > surface->width || y > surface->height) return 0;
  draw_batch_flush(batch_flush_deferred);

  LPDIRECT3DSURFACE9 pBuffer = surface->surf;
  d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  D3DSURFACE_DESC desc;
  pBackBuffer->GetDesc(&desc);

	D3DLOCKED_RECT rect;

	pBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	unsigned offset = y * rect.Pitch + x * 4;
	int ret = bitmap[offset + 0] | (bitmap[offset + 1] << 8) | (bitmap[offset + 2] << 16) | (bitmap[offset + 3] << 24);
	pBuffer->UnlockRect();
	delete[] bitmap;

	return ret;
}

int surface_getpixel_alpha(int id, int x, int y)
{
  get_surfacev(surface,id,-1);
  if (x < 0) x = 0;
  if (y < 0) y = 0;
  if (x > surface->width || y > surface->height) return 0;
  draw_batch_flush(batch_flush_deferred);

  LPDIRECT3DSURFACE9 pBuffer = surface->surf;
  d3dmgr->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  D3DSURFACE_DESC desc;
  pBackBuffer->GetDesc(&desc);

	D3DLOCKED_RECT rect;

	pBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	unsigned offset = y * rect.Pitch + x * 4;
	int ret = bitmap[offset];
	pBuffer->UnlockRect();
	delete[] bitmap;

	return ret;
}

}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef DX_BGR
  #define DX_BGR 0x80E0
#endif

#include "Universal_System/estring.h"
#include "Universal_System/image_formats.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{
  draw_batch_flush(batch_flush_deferred);

  get_surfacev(surface,id,-1);
	string ext = enigma::image_get_format(filename);

  LPDIRECT3DSURFACE9 pDestBuffer;
  D3DSURFACE_DESC desc;
  surface->surf->GetDesc(&desc);

	d3dmgr->device->CreateOffscreenPlainSurface( desc.Width, desc.Height, desc.Format, D3DPOOL_SYSTEMMEM, &pDestBuffer, NULL );
	d3dmgr->device->GetRenderTargetData(surface->surf, pDestBuffer);

	D3DLOCKED_RECT rect;

	pDestBuffer->LockRect(&rect, NULL, D3DLOCK_READONLY);
	unsigned char* bitmap = static_cast<unsigned char*>(rect.pBits);
	pDestBuffer->UnlockRect();

	int ret = enigma::image_save(filename, bitmap, desc.Width, desc.Height, desc.Width, desc.Height, false);

	pDestBuffer->Release();

  return ret;
}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{

}

int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{

}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig)
{

}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{
  return sprite_create_from_surface(id, x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth)
{

}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{

}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{

}

}
