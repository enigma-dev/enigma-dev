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
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Platforms/platforms_mandatory.h"

using namespace enigma::dx11;

namespace {

D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { };

void update_depth_stencil_state() {
  static ID3D11DepthStencilState* pDepthStencilState = NULL;
  if (pDepthStencilState) { pDepthStencilState->Release(); pDepthStencilState = NULL; }
  m_device->CreateDepthStencilState(&depthStencilDesc, &pDepthStencilState);
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  m_deviceContext->OMSetDepthStencilState(pDepthStencilState, 1);
}

} // namespace anonymous

namespace enigma {

bool d3dMode = false;
bool d3dHidden = false;
bool d3dZWriteEnable = true;
int d3dCulling = 0;

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
}

void init_depth_stencil_state() {
  depthStencilDesc.DepthEnable = false;
  depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

  update_depth_stencil_state();
}

} // namespace enigma

namespace enigma_user
{

void d3d_clear_depth(double value) {
    draw_batch_flush(batch_flush_deferred);
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, value, 0);
}

void d3d_start()
{
    draw_batch_flush(batch_flush_deferred);
	enigma::d3dMode = true;
    enigma::d3dPerspective = true;
	enigma::d3dCulling =  rs_none;
	d3d_set_hidden(false);
}

void d3d_end()
{
    draw_batch_flush(batch_flush_deferred);
	enigma::d3dMode = false;
    enigma::d3dPerspective = false;
	enigma::d3dCulling = rs_none;
	d3d_set_hidden(false);
}

void d3d_set_hidden(bool enable)
{
    draw_batch_flush(batch_flush_deferred);
    enigma::d3dHidden = enable;
    depthStencilDesc.DepthEnable = enable;
    update_depth_stencil_state();
}

void d3d_set_zwriteenable(bool enable)
{
    draw_batch_flush(batch_flush_deferred);
	enigma::d3dZWriteEnable = enable;
}

void d3d_set_lighting(bool enable)
{

}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}

void d3d_set_fog_enabled(bool enable)
{

}

void d3d_set_fog_mode(int mode)
{

}

void d3d_set_fog_hint(int mode) {

}

void d3d_set_fog_color(int color)
{

}

void d3d_set_fog_start(double start)
{

}

void d3d_set_fog_end(double end)
{

}

void d3d_set_fog_density(double density)
{

}

void d3d_set_culling(int mode)
{
	enigma::d3dCulling = mode;

}

bool d3d_get_mode()
{
    return enigma::d3dMode;
}

bool d3d_get_hidden()
{
    return enigma::d3dHidden;
}

int d3d_get_culling() {
	return enigma::d3dCulling;
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

void d3d_set_shading(bool smooth)
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

    }

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
    {

    }

    bool light_define_specularity(int id, int r, int g, int b, double a)
    {

    }

    bool light_enable(int id)
    {

    }

    bool light_disable(int id)
    {

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
  float specular[4] = {r, g, b, a};

}

void d3d_light_shininess(int facemode, int shine)
{

}

void d3d_light_define_ambient(int col)
{

}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

}

// ***** LIGHTS END *****
