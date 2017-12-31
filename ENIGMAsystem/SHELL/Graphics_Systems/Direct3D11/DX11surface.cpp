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

#include "Bridges/General/DX11Context.h"
#include "Direct3D11Headers.h"
using namespace std;
#include <cstddef>
#include <iostream>
#include <math.h>


#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/sprites_internal.h"
#include "Universal_System/background_internal.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
extern int room_width, room_height/*, sprite_idmax*/;
}

#include "../General/GSprimitives.h"
#include "../General/GSsurface.h"
#include "DX11SurfaceStruct.h"
#include "DX11TextureStruct.h"

namespace enigma
{
  vector<Surface*> Surfaces(0);

}


namespace enigma_user
{

bool surface_is_supported()
{
  //TODO: Implement with IDirect3D9::CheckDeviceFormat
  return true;
}

int surface_create(int width, int height, bool depthbuffer)
{
  ID3D11Texture2D *renderTargetTexture;
  ID3D11RenderTargetView* renderTargetView;
  ID3D11ShaderResourceView* shaderResourceView;

  D3D11_TEXTURE2D_DESC textureDesc;
  HRESULT result;
  D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
  D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


  // Initialize the render target texture description.
  ZeroMemory(&textureDesc, sizeof(textureDesc));

  // Setup the render target texture description.
  textureDesc.Width = width;
  textureDesc.Height = height;
  textureDesc.MipLevels = 1;
  textureDesc.ArraySize = 1;
  textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
  textureDesc.SampleDesc.Count = 1;
  textureDesc.Usage = D3D11_USAGE_DEFAULT;
  textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
  textureDesc.CPUAccessFlags = 0;
  textureDesc.MiscFlags = 0;

  // Create the render target texture.
  result = m_device->CreateTexture2D(&textureDesc, NULL, &renderTargetTexture);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the render target view.
  renderTargetViewDesc.Format = textureDesc.Format;
  renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
  renderTargetViewDesc.Texture2D.MipSlice = 0;

  // Create the render target view.
  result = m_device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
  if(FAILED(result))
  {
    return false;
  }

  // Setup the description of the shader resource view.
  shaderResourceViewDesc.Format = textureDesc.Format;
  shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
  shaderResourceViewDesc.Texture2D.MipLevels = 1;

  // Create the shader resource view.
  result = m_device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
  if(FAILED(result))
  {
    return false;
  }

  enigma::Surface* surface = new enigma::Surface();
  TextureStruct* gmTexture = new TextureStruct(renderTargetTexture);
  textureStructs.push_back(gmTexture);
  surface->renderTargetView = renderTargetView;
  surface->tex = textureStructs.size() - 1;
  surface->width = width; surface->height = height;
  enigma::Surfaces.push_back(surface);
  return enigma::Surfaces.size() - 1;
}

int surface_create_msaa(int width, int height, int levels)
{

}

void surface_set_target(int id)
{
  get_surface(surface,id);
  m_deviceContext->OMSetRenderTargets(1, &surface->renderTargetView, NULL);
}

void surface_reset_target()
{
  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
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

}

int surface_getpixel_ext(int id, int x, int y)
{

}

int surface_getpixel_alpha(int id, int x, int y)
{

}

}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef DX_BGR
  #define DX_BGR 0x80E0
#endif

#include "Universal_System/estring.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{

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

