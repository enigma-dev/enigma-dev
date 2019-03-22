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

#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmatrix_impl.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Widget_Systems/widgets_mandatory.h"

#include "Universal_System/scalar.h"

#include <glm/gtc/type_ptr.hpp>

using namespace enigma::dx9;

namespace {

template<typename T, typename F>
T alias_cast(F raw_data) {
  T value;
  memcpy(&value, &raw_data, sizeof(value));
  return value;
}

D3DCULL cullingstates[3] = {
  D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW
};

D3DFILLMODE fillmodes[3] = {
  D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID
};

D3DCMPFUNC depthoperators[8] = {
  D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL,
  D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL,
  D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS
};

D3DFOGMODE fogmodes[3] = {
    D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR
};

} // anonymous namespace

namespace enigma {

void graphics_set_matrix(int type) {
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  D3DTRANSFORMSTATETYPE state;
  glm::mat4 matrix;
  switch(type) {
    case enigma_user::matrix_world:
      state = D3DTS_WORLD;
      matrix = enigma::world;
      break;
    case enigma_user::matrix_view:
      state = D3DTS_VIEW;
      matrix = enigma::view;
      break;
    case enigma_user::matrix_projection:
      state = D3DTS_PROJECTION;
      matrix = enigma::projection;
      break;
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type " + std::to_string(type), false);
      #endif
      return;
  }
  d3dmgr->SetTransform(state, (D3DMATRIX*)glm::value_ptr(matrix));
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), value, 0);
}

void d3d_start()
{
  draw_batch_flush(batch_flush_deferred);
  enigma::d3dMode = true;
  enigma::d3dPerspective = true;
  enigma::d3dCulling = rs_none;
  d3dmgr->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  d3dmgr->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
  d3dmgr->SetRenderState(D3DRS_ZENABLE, enigma::d3dHidden = true);
  d3dmgr->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

  // Enable texture repetition by default
  d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
  d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
  d3dmgr->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
}

void d3d_end()
{
  draw_batch_flush(batch_flush_deferred);
  enigma::d3dMode = false;
  enigma::d3dPerspective = false;
  enigma::d3dCulling = rs_none;
  d3dmgr->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
  d3d_set_hidden(false);
}

void d3d_set_software_vertex_processing(bool software) {
	d3dmgr->device->SetSoftwareVertexProcessing(software);
}

void d3d_set_hidden(bool enable)
{
    draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ZENABLE, enable); // enable/disable the z-buffer
    enigma::d3dHidden = enable;
}

void d3d_set_zwriteenable(bool enable)
{
    draw_batch_flush(batch_flush_deferred);
	enigma::d3dZWriteEnable = enable;
	d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, enable);    // enable/disable z-writing
}

void d3d_set_lighting(bool enable)
{
    draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_LIGHTING, enable);    // enable/disable the 3D lighting
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
    draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_FOGENABLE, enable);
	d3dmgr->SetRenderState(D3DRS_RANGEFOGENABLE, enable);
}

void d3d_set_fog_mode(int mode)
{
    draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_FOGTABLEMODE, fogmodes[mode]);
	d3dmgr->SetRenderState(D3DRS_FOGVERTEXMODE, fogmodes[mode]);
}

void d3d_set_fog_hint(int mode) {

}

void d3d_set_fog_color(int color)
{
    draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_FOGCOLOR,
                    D3DCOLOR_RGBA(COL_GET_R(color), COL_GET_G(color), COL_GET_B(color), 255)); // Highest 8 bits are not used.
}

// NOTE: DWORD is 32 bits maximum meaning it can only hold single-precision
// floats, so yes, we must cast double to float first too.
// https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3drenderstatetype
void d3d_set_fog_start(double start)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_FOGSTART, alias_cast<DWORD>((float)start));
}

void d3d_set_fog_end(double end)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_FOGEND, alias_cast<DWORD>((float)end));
}

void d3d_set_fog_density(double density)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_FOGDENSITY, alias_cast<DWORD>((float)density));
}

void d3d_set_culling(int mode)
{
	draw_batch_flush(batch_flush_deferred);
	enigma::d3dCulling = mode;
	d3dmgr->SetRenderState(D3DRS_CULLMODE, cullingstates[mode]);
}

void d3d_set_fill_mode(int fill)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_FILLMODE, fillmodes[fill]);
}

void d3d_set_line_width(float value) {

}

void d3d_set_point_size(float value) {
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_POINTSIZE, value);
}

void d3d_set_depth_operator(int mode) {
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_ZFUNC, depthoperators[mode]);
}

void d3d_set_depth(double dep)
{

}

void d3d_set_shading(bool smooth)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->SetRenderState(D3DRS_SHADEMODE, smooth?D3DSHADE_GOURAUD:D3DSHADE_FLAT);
}

void d3d_set_clip_plane(bool enable)
{
   ///TODO: Code this
}

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
  draw_batch_flush(batch_flush_deferred);

  D3DLIGHT9 light = { };
  light.Type = D3DLIGHT_DIRECTIONAL;
  light.Diffuse = { COL_GET_Rf(col), COL_GET_Gf(col), COL_GET_Bf(col), 1.0f };
  light.Direction = { dx, dy, dz };

  return d3dmgr->device->SetLight(id, &light) == D3D_OK;
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
{
  draw_batch_flush(batch_flush_deferred);

  D3DLIGHT9 light = { };
  light.Type = D3DLIGHT_POINT;
  light.Diffuse = { COL_GET_Rf(col), COL_GET_Gf(col), COL_GET_Bf(col), 1.0f };
  light.Position = { x, y, z };
  light.Range = range;
  light.Attenuation0 = 1.0f;    // no constant inverse attenuation
  light.Attenuation1 = 0.0f;    // only .125 inverse attenuation
  light.Attenuation2 = 0.0f;    // no square inverse attenuation
  //light.Phi = gs_angle_to_radians(360.0f);    // set the outer cone to 360 degrees
  //light.Theta = gs_angle_to_radians(360.0f);    // set the inner cone to 360 degrees
  light.Falloff = 1.0f;    // use the typical falloff

  return d3dmgr->SetLight(id, &light) == D3D_OK;
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
  return false;
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  draw_batch_flush(batch_flush_deferred);
}

void d3d_light_shininess(int facemode, int shine)
{
  draw_batch_flush(batch_flush_deferred);
}

void d3d_light_define_ambient(int col)
{
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(COL_GET_R(col), COL_GET_G(col), COL_GET_B(col), 255));
}

bool d3d_light_enable(int id, bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  return d3dmgr->LightEnable(id, enable) == D3D_OK;
}

//TODO(harijs) - This seems to be broken like this. Almost works, but stencilmask needs some different value
void d3d_stencil_start_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_STENCILENABLE, true);
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, false);
  d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, false);

  d3dmgr->SetRenderState(D3DRS_STENCILREF, 0x1);
  d3dmgr->SetRenderState(D3DRS_STENCILMASK, 0x1);

  d3dmgr->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

  d3dmgr->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
  d3dmgr->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
  d3dmgr->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
}

void d3d_stencil_use_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_ZWRITEENABLE, true);
  d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, true);

  d3dmgr->SetRenderState(D3DRS_STENCILMASK, 0x0);
  d3dmgr->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
}

void d3d_stencil_end_mask(){
  draw_batch_flush(batch_flush_deferred);
  d3dmgr->SetRenderState(D3DRS_STENCILENABLE, false);
}

} // namespace enigma_user
