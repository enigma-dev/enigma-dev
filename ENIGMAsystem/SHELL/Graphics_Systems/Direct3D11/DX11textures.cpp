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
#include "Universal_System/image_formats.h"
#include "Universal_System/nlpo2.h"

using namespace enigma::dx11;

namespace enigma {

int graphics_create_texture(const RawImage& img, bool mipmap, unsigned* fullwidth, unsigned* fullheight) {
  ID3D11Texture2D *tex;
  D3D11_TEXTURE2D_DESC tdesc;
  D3D11_SUBRESOURCE_DATA tbsd;
  
  unsigned fw = img.w, fh = img.h;
  if (fullwidth == nullptr) fullwidth = &fw; 
  if (fullheight == nullptr) fullheight = &fh;
  
  if (img.pxdata != nullptr) {
    *fullwidth  = nlpo2dc(img.w) + 1;
    *fullheight = nlpo2dc(img.h) + 1;
  }

  tbsd.SysMemPitch = (*fullwidth) * 4;
  // not needed since this is a 2d texture,
  // but we can pass size info for debugging
  tbsd.SysMemSlicePitch = (*fullwidth) * (*fullheight) * 4;

  tdesc.Width = *fullwidth;
  tdesc.Height = *fullheight;
  tdesc.MipLevels = 1;
  tdesc.ArraySize = 1;

  tdesc.SampleDesc.Count = 1;
  tdesc.SampleDesc.Quality = 0;
  tdesc.Usage = D3D11_USAGE_DEFAULT;
  tdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  tdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

  tdesc.CPUAccessFlags = 0;
  tdesc.MiscFlags = 0;

  if (img.pxdata != nullptr && (img.w != *fullwidth || img.h != *fullheight)) {
    RawImage padded = image_pad(img, *fullwidth, *fullheight);
    tbsd.pSysMem = padded.pxdata;
    if (FAILED(m_device->CreateTexture2D(&tdesc,&tbsd,&tex)))
      return 0;
  } else {
    // surfaces will fall through here to create render target textures
    tbsd.pSysMem = img.pxdata;
    if (FAILED(m_device->CreateTexture2D(&tdesc,&tbsd,&tex)))
      return 0;
  }

  D3D11_SHADER_RESOURCE_VIEW_DESC vdesc;
  vdesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  vdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
  vdesc.Texture2D.MostDetailedMip = 0;
  vdesc.Texture2D.MipLevels = 1;

  ID3D11ShaderResourceView *view;
  m_device->CreateShaderResourceView(tex, &vdesc, &view);

  const int id = textures.size();
  textures.push_back(std::make_unique<DX11Texture>(tex, view));
  auto& textureStruct = textures.back();
  textureStruct->width = img.w;
  textureStruct->height = img.h;
  textureStruct->fullwidth = *fullwidth;
  textureStruct->fullheight = *fullheight;
  return id;
}

void graphics_delete_texture(int tex) {
  if (tex >= 0) {
    DX11Texture* texture = static_cast<DX11Texture*>(textures[tex].get());
    texture->peer->Release(), texture->peer = NULL;
    texture->view->Release(), texture->view = NULL;
  }
}

unsigned char* graphics_copy_texture_pixels(int texture, unsigned* fullwidth, unsigned* fullheight) {
  *fullwidth = textures[texture]->fullwidth;
  *fullheight = textures[texture]->fullheight;

  unsigned char* ret = new unsigned char[((*fullwidth)*(*fullheight)*4)];

  return ret;
}

unsigned char* graphics_copy_texture_pixels(int texture, int x, int y, int width, int height) {
  unsigned fw, fh;
  unsigned char* pxdata = graphics_copy_texture_pixels(texture, &fw, &fh);
  return pxdata;
}

void graphics_push_texture_pixels(int texture, int x, int y, int width, int height, unsigned char* pxdata) {}

void graphics_push_texture_pixels(int texture, int width, int height, unsigned char* pxdata) {}

} // namespace enigma

namespace enigma_user {

void texture_set_priority(int texid, double prio)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
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
