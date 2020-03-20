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

#include "DX9surface_impl.h"
#include "DX9textures_impl.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"

using namespace std;
using namespace enigma::dx9;

namespace enigma_user {

bool surface_is_supported()
{
  //TODO: Implement with IDirect3D9::CheckDeviceFormat
  return true;
}

int surface_create(int width, int height, bool depthbuffer, bool, bool)
{
  LPDIRECT3DTEXTURE9 texture = NULL;
  d3ddev->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
  enigma::Surface* surface = new enigma::Surface();
  const int texid = enigma::textures.size();
  enigma::textures.push_back(std::make_unique<enigma::DX9Texture>(texture));
  auto& gmTexture = enigma::textures.back();
  gmTexture->width = gmTexture->fullwidth = width;
  gmTexture->height = gmTexture->fullheight = height;
  //d3ddev->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
  texture->GetSurfaceLevel(0,&surface->surf);
  surface->texture = texid; surface->width = width; surface->height = height;
  enigma::surfaces.push_back(surface);
  return enigma::surfaces.size() - 1;
}

int surface_create_msaa(int width, int height, int levels)
{
  LPDIRECT3DTEXTURE9 texture = NULL;
  d3ddev->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);
  enigma::Surface* surface = new enigma::Surface();
  const int texid = enigma::textures.size();
  enigma::textures.push_back(std::make_unique<enigma::DX9Texture>(texture));
  auto& gmTexture = enigma::textures.back();
  gmTexture->width = gmTexture->fullwidth = width;
  gmTexture->height = gmTexture->fullheight = height;
  d3ddev->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
  surface->texture = texid; surface->width = width; surface->height = height;
  enigma::surfaces.push_back(surface);
  return enigma::surfaces.size() - 1;
}

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surface,id);
  d3ddev->SetRenderTarget(0, surface.surf);

  d3d_set_projection_ortho(0, 0, surface.width, surface.height, 0);
}

void surface_reset_target()
{
  draw_batch_flush(batch_flush_deferred);

  LPDIRECT3DSURFACE9 pBackBuffer;
  d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
  d3ddev->SetRenderTarget(0, pBackBuffer);
  pBackBuffer->Release();
}

int surface_get_target()
{
  return 0; //TODO: implement
}

void surface_free(int id)
{
  delete enigma::surfaces[id];
}

}
