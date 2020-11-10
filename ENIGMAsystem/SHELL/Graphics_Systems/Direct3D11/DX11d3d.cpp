/** Copyright (C) 2013, 2018, 2019 Robert B. Colton
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
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/graphics_mandatory.h"

#include "Platforms/platforms_mandatory.h"

using namespace enigma::dx11;

namespace {

const D3D11_FILL_MODE fillmodes[3] = {
  D3D11_FILL_WIREFRAME, D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
};

const D3D11_BLEND blend_equivs[11] = {
  D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA,
  D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR,
  D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT
};

ID3D11ShaderResourceView *getDefaultWhiteTexture() {
    static int texid = -1;
    if (texid == -1) {
      unsigned data[1] = {0xFFFFFFFF};
      texid = enigma::graphics_create_texture(enigma::RawImage((unsigned char*)data, 1, 1) , false);
    }
    return static_cast<enigma::DX11Texture*>(enigma::textures[texid].get())->view;
}

} // namespace anonymous

namespace enigma {

void graphics_state_flush_samplers() {
  static ID3D11ShaderResourceView *nullTextureView = getDefaultWhiteTexture();
  static ID3D11SamplerState *pSamplerStates[8];

  for (int i = 0; i < 8; ++i) {
    const auto sampler = samplers[i];

    ID3D11ShaderResourceView *view = (sampler.texture == -1)?
      nullTextureView:static_cast<enigma::DX11Texture*>(enigma::textures[sampler.texture].get())->view;
    m_deviceContext->PSSetShaderResources(i, 1, &view);

    D3D11_SAMPLER_DESC samplerDesc = { };

    samplerDesc.Filter = sampler.interpolate?D3D11_FILTER_MIN_MAG_MIP_LINEAR:D3D11_FILTER_MIN_MAG_MIP_POINT;
    samplerDesc.AddressU = sampler.wrapu?D3D11_TEXTURE_ADDRESS_WRAP:D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressV = sampler.wrapv?D3D11_TEXTURE_ADDRESS_WRAP:D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.AddressW = sampler.wrapw?D3D11_TEXTURE_ADDRESS_WRAP:D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ID3D11SamplerState *pSamplerState = pSamplerStates[i];
    if (pSamplerState) { pSamplerState->Release(); pSamplerState = NULL; }
    m_device->CreateSamplerState(&samplerDesc, &pSamplerState);
    m_deviceContext->PSSetSamplers(i, 1, &pSamplerState);
    pSamplerStates[i] = pSamplerState; // copy the sampler state ptr back to our cache
  }
}

void graphics_state_flush() {
  // Setup the raster description which will determine how and what polygons will be drawn.
  D3D11_RASTERIZER_DESC rasterDesc = { };
  rasterDesc.AntialiasedLineEnable = false;
  rasterDesc.CullMode = D3D11_CULL_NONE;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.0f;
  rasterDesc.DepthClipEnable = false;
  rasterDesc.FillMode = fillmodes[drawFillMode];
  rasterDesc.FrontCounterClockwise = false;
  rasterDesc.MultisampleEnable = msaaEnabled;
  rasterDesc.ScissorEnable = false;
  rasterDesc.SlopeScaledDepthBias = 0.0f;

  static ID3D11RasterizerState* pRasterState = NULL;
  if (pRasterState) { pRasterState->Release(); pRasterState = NULL; }
  m_device->CreateRasterizerState(&rasterDesc, &pRasterState);
  m_deviceContext->RSSetState(pRasterState);

  D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };

  depthStencilDesc.DepthEnable = d3dHidden;
  depthStencilDesc.DepthWriteMask = d3dZWriteEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
  depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

  depthStencilDesc.StencilEnable = false;
  depthStencilDesc.StencilReadMask = 0xFF;
  depthStencilDesc.StencilWriteMask = 0xFF;

  // Stencil operations if pixel is front-facing.
  depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
  depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  // Stencil operations if pixel is back-facing.
  depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
  depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
  depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

  static ID3D11DepthStencilState* pDepthStencilState = NULL;
  if (pDepthStencilState) { pDepthStencilState->Release(); pDepthStencilState = NULL; }
  m_device->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
  m_deviceContext->OMSetDepthStencilState(pDepthStencilState, 1);

  D3D11_BLEND_DESC blendStateDesc = { };

  blendStateDesc.RenderTarget[0].BlendEnable = TRUE;

  blendStateDesc.RenderTarget[0].SrcBlendAlpha = blendStateDesc.RenderTarget[0].SrcBlend = blend_equivs[(blendMode[0]-1)%11];
  blendStateDesc.RenderTarget[0].DestBlendAlpha = blendStateDesc.RenderTarget[0].DestBlend = blend_equivs[(blendMode[1]-1)%11];
  blendStateDesc.RenderTarget[0].BlendOpAlpha = blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
  UINT8 colorWriteMask = 0;
  if (enigma::colorWriteEnable[0]) colorWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
  if (enigma::colorWriteEnable[1]) colorWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
  if (enigma::colorWriteEnable[2]) colorWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
  if (enigma::colorWriteEnable[3]) colorWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
  blendStateDesc.RenderTarget[0].RenderTargetWriteMask = colorWriteMask;

  static ID3D11BlendState* pBlendState = NULL;
  if (pBlendState) { pBlendState->Release(); pBlendState = NULL; }
  m_device->CreateBlendState(&blendStateDesc, &pBlendState);
  m_deviceContext->OMSetBlendState(pBlendState, NULL, 0xffffffff);

  graphics_state_flush_samplers();
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, value, 0);
}

void d3d_stencil_clear_value(int value) {
  draw_batch_flush(batch_flush_deferred);
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_STENCIL, 0, value);
}

void d3d_stencil_clear() {
  draw_batch_flush(batch_flush_deferred);
  m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_STENCIL, 0, 0);
}

} // namespace enigma_user
