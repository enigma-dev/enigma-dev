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

#include "DX11textures_impl.h"
#include "Direct3D11Headers.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GStextures_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"

using namespace enigma::dx11;

namespace {

inline unsigned int lgpp2(unsigned int x) {//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

} // namespace anonymous

namespace enigma {

int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool mipmap)
{
  ID3D11Texture2D *tex;
  D3D11_TEXTURE2D_DESC tdesc;
  D3D11_SUBRESOURCE_DATA tbsd;

  tbsd.pSysMem = pxdata;
  tbsd.SysMemPitch = fullwidth*4;
  // not needed since this is a 2d texture,
  // but we can pass size info for debugging
  tbsd.SysMemSlicePitch = fullwidth*fullheight*4;

  tdesc.Width = fullwidth;
  tdesc.Height = fullheight;
  tdesc.MipLevels = 1;
  tdesc.ArraySize = 1;

  tdesc.SampleDesc.Count = 1;
  tdesc.SampleDesc.Quality = 0;
  tdesc.Usage = D3D11_USAGE_DEFAULT;
  tdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  tdesc.CPUAccessFlags = 0;
  tdesc.MiscFlags = 0;

  if (FAILED(m_device->CreateTexture2D(&tdesc,&tbsd,&tex)))
    return 0;

  D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
  vdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  vdesc.Texture2D.MostDetailedMip = 0;
  vdesc.Texture2D.MipLevels = 1;

  ID3D11ShaderResourceView *view;
  m_device->CreateShaderResourceView(tex, &vdesc, &view);

  DX11Texture* textureStruct = new DX11Texture(tex, view);
  textureStruct->width = width;
  textureStruct->height = height;
  textureStruct->fullwidth = fullwidth;
  textureStruct->fullheight = fullheight;
  const int id = textures.size();
  textures.push_back(textureStruct);
  return id;
}

int graphics_duplicate_texture(int tex, bool mipmap)
{
  return -1; //TODO: implement
}

void graphics_copy_texture(int source, int destination, int x, int y)
{

}

void graphics_copy_texture_part(int source, int destination, int xoff, int yoff, int w, int h, int x, int y)
{

}

void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
{

}

void graphics_delete_texture(int tex)
{
  auto texture = (DX11Texture*)textures[tex];
  texture->peer->Release(), texture->peer = NULL;
  texture->view->Release(), texture->view = NULL;
}

unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
{
  return NULL; //TODO: implement
}

} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

void texture_set_enabled(bool enable)
{

}

void texture_set_blending(bool enable)
{

}

bool texture_mipmapping_supported()
{
  return false; //TODO: implement
}

bool texture_anisotropy_supported()
{
  return false; //TODO: implement
}

float texture_anisotropy_maxlevel()
{
  return 0.0f; //TODO: implement
}

} // namespace enigma_user
