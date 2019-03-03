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

#include "DX11TextureStruct.h"
#include "Direct3D11Headers.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include "Universal_System/image_formats.h"

#include <stdio.h>
#include <string.h>

using std::string;

using namespace enigma::dx11;

vector<TextureStruct*> textureStructs(0);

inline unsigned int lgpp2(unsigned int x) {//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

namespace {

ID3D11ShaderResourceView *getDefaultWhiteTexture() {
    static int texid = -1;
    if (texid == -1) {
      unsigned data[1] = {0xFFFFFFFF};
      texid = enigma::graphics_create_texture(1, 1, 1, 1, (void*)data, false);
    }
    return textureStructs[texid]->view;
}

D3D11_SAMPLER_DESC samplerDesc = { };

void update_sampler_state() {
  static ID3D11SamplerState *pSamplerState = NULL;
  if (pSamplerState) { pSamplerState->Release(); pSamplerState = NULL; }
  m_device->CreateSamplerState(&samplerDesc, &pSamplerState);
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  m_deviceContext->PSSetSamplers(0, 1, &pSamplerState);
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

  TextureStruct* textureStruct = new TextureStruct(tex, view);
  textureStruct->width = width;
  textureStruct->height = height;
  textureStruct->fullwidth = fullwidth;
  textureStruct->fullheight = fullheight;
  textureStructs.push_back(textureStruct);
  return textureStructs.size()-1;
}

int graphics_duplicate_texture(int tex, bool mipmap)
{

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

}

unsigned char* graphics_get_texture_pixeldata(unsigned texture, unsigned* fullwidth, unsigned* fullheight)
{

}

void init_sampler_state() {
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
  update_sampler_state();
}

} // namespace enigma

namespace enigma_user {

int texture_add(string filename, bool mipmap) {
  unsigned int w, h, fullwidth, fullheight;
  int img_num;

  unsigned char *pxdata = enigma::image_load(filename,&w,&h,&fullwidth,&fullheight,&img_num,false);
  if (pxdata == NULL) { printf("ERROR - Failed to append sprite to index!\n"); return -1; }
  unsigned texture = enigma::graphics_create_texture(w, h, fullwidth, fullheight, pxdata, mipmap);
  delete[] pxdata;

  return texture;
}

void texture_save(int texid, string fname) {
	unsigned w, h;
	unsigned char* rgbdata = enigma::graphics_get_texture_pixeldata(texid, &w, &h);

  string ext = enigma::image_get_format(fname);

	enigma::image_save(fname, rgbdata, w, h, w, h, false);

	delete[] rgbdata;
}

void texture_delete(int texid) {
  delete textureStructs[texid];
}

bool texture_exists(int texid) {
  return textureStructs[texid] != NULL;
}

void texture_preload(int texid)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

void texture_set_priority(int texid, double prio)
{
  // Deprecated in ENIGMA and GM: Studio, all textures are automatically preloaded.
}

gs_scalar texture_get_width(int texid) {
	return textureStructs[texid]->width / textureStructs[texid]->fullwidth;
}

gs_scalar texture_get_height(int texid)
{
	return textureStructs[texid]->height / textureStructs[texid]->fullheight;
}

gs_scalar texture_get_texel_width(int texid)
{
	return 1.0/textureStructs[texid]->width;
}

gs_scalar texture_get_texel_height(int texid)
{
	return 1.0/textureStructs[texid]->height;
}

void texture_set_enabled(bool enable)
{

}

void texture_set_blending(bool enable)
{

}

void texture_set_stage(int stage, int texid) {
  draw_batch_flush(batch_flush_deferred);
  if (texid == -1) {
    ID3D11ShaderResourceView *nullView = getDefaultWhiteTexture();
    m_deviceContext->PSSetShaderResources(stage, 1, &nullView);
    return;
  }
  m_deviceContext->PSSetShaderResources(stage, 1, &textureStructs[texid]->view);
}

void texture_reset() {
  ID3D11ShaderResourceView *nullView = getDefaultWhiteTexture();
  m_deviceContext->PSSetShaderResources(0, 1, &nullView);
}

void texture_set_interpolation_ext(int sampler, bool enable)
{
  samplerDesc.Filter = enable ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
  update_sampler_state();
}

void texture_set_repeat_ext(int sampler, bool repeat)
{
  D3D11_TEXTURE_ADDRESS_MODE addressMode = repeat ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_CLAMP;
  samplerDesc.AddressU = addressMode;
  samplerDesc.AddressV = addressMode;
  samplerDesc.AddressW = addressMode;
  update_sampler_state();
}

void texture_set_wrap_ext(int sampler, bool wrapu, bool wrapv, bool wrapw)
{

}

void texture_set_border_ext(int sampler, int r, int g, int b, double a)
{

}

void texture_set_filter_ext(int sampler, int filter)
{

}

void texture_set_lod_ext(int sampler, double minlod, double maxlod, int maxlevel)
{

}

bool texture_mipmapping_supported()
{

}

bool texture_anisotropy_supported()
{

}

float texture_anisotropy_maxlevel()
{

}

void texture_anisotropy_filter(int sampler, gs_scalar levels)
{

}

} // namespace enigma_user
