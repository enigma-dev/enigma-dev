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

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Platforms/platforms_mandatory.h"

using namespace enigma::dx11;

namespace {

const D3D11_BLEND blend_equivs[11] = {
  D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR, D3D11_BLEND_SRC_ALPHA,
  D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_DEST_COLOR,
  D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_SRC_ALPHA_SAT
};

} // namespace anonymous

namespace enigma {

void graphics_state_flush() {
  // Setup the raster description which will determine how and what polygons will be drawn.
  D3D11_RASTERIZER_DESC rasterDesc = { };
  rasterDesc.AntialiasedLineEnable = false;
  rasterDesc.CullMode = D3D11_CULL_NONE;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.0f;
  rasterDesc.DepthClipEnable = false;
  rasterDesc.FillMode = D3D11_FILL_SOLID;
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
  blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

  static ID3D11BlendState* pBlendState = NULL;
  if (pBlendState) { pBlendState->Release(); pBlendState = NULL; }
  m_device->CreateBlendState(&blendStateDesc, &pBlendState);
  m_deviceContext->OMSetBlendState(pBlendState, NULL, 0xffffffff);
}

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
    draw_batch_flush(batch_flush_deferred);
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, value, 0);
}

void d3d_set_fill_mode(int fill)
{

}

void d3d_set_line_width(float value) {

}

void d3d_set_point_size(float value) {

}

void d3d_set_depth_operator(int mode) {

}

void d3d_set_depth(double dep)
{

}

void d3d_set_clip_plane(bool enable)
{
   ///TODO: Code this
}

}

// ***** LIGHTS BEGIN *****
#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    bool light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
    {
      return false; //TODO: implement
    }

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
    {
      return false; //TODO: implement
    }

    bool light_define_specularity(int id, int r, int g, int b, double a)
    {
      return false; //TODO: implement
    }

    bool light_enable(int id)
    {
      return false; //TODO: implement
    }

    bool light_disable(int id)
    {
      return false; //TODO: implement
    }
} d3d_lighting;

namespace enigma_user
{

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{

}

void d3d_light_shininess(int facemode, int shine)
{

}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

}

// ***** LIGHTS END *****
