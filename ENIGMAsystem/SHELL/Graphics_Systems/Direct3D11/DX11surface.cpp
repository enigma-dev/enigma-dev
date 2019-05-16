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

#include "DX11surface_impl.h"
#include "DX11textures_impl.h"
#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSsurface.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"

using namespace std;
using namespace enigma::dx11;

namespace enigma_user {

bool surface_is_supported()
{
  //TODO: Implement with IDirect3D9::CheckDeviceFormat
  return true;
}

int surface_create(int width, int height, bool depthbuffer, bool, bool)
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
  enigma::DX11Texture* gmTexture = new enigma::DX11Texture(renderTargetTexture, shaderResourceView);
  const int texid = enigma::textures.size();
  enigma::textures.push_back(gmTexture);
  surface->renderTargetView = renderTargetView;
  surface->texture = texid;
  surface->width = width; surface->height = height;
  enigma::surfaces.push_back(surface);
  return enigma::surfaces.size() - 1;
}

int surface_create_msaa(int width, int height, int levels)
{
  return -1; //TODO: implement
}

void surface_set_target(int id)
{
  draw_batch_flush(batch_flush_deferred);

  get_surface(surface,id);
  m_deviceContext->OMSetRenderTargets(1, &surface.renderTargetView, NULL);
}

void surface_reset_target()
{
  draw_batch_flush(batch_flush_deferred);

  m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
}

int surface_get_target()
{
  return -1; //TODO: implement
}

void surface_free(int id)
{
  delete enigma::surfaces[id];
}

}
