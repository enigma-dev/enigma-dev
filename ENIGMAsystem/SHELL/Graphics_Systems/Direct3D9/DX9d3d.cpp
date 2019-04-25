/** Copyright (C) 2013, 2019 Robert B. Colton
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

#include "DX9textures_impl.h"
#include "Direct3D9Headers.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSblend.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GScolors.h"
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

const D3DCULL cullingstates[3] = {
  D3DCULL_NONE, D3DCULL_CCW, D3DCULL_CW
};

const D3DFILLMODE fillmodes[3] = {
  D3DFILL_POINT, D3DFILL_WIREFRAME, D3DFILL_SOLID
};

const D3DCMPFUNC depthoperators[8] = {
  D3DCMP_NEVER, D3DCMP_LESS, D3DCMP_EQUAL,
  D3DCMP_LESSEQUAL, D3DCMP_GREATER, D3DCMP_NOTEQUAL,
  D3DCMP_GREATEREQUAL, D3DCMP_ALWAYS
};

const D3DFOGMODE fogmodes[3] = {
  D3DFOG_EXP, D3DFOG_EXP2, D3DFOG_LINEAR
};

const D3DBLEND blendequivs[11] = {
  D3DBLEND_ZERO, D3DBLEND_ONE, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR, D3DBLEND_SRCALPHA,
  D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR,
  D3DBLEND_INVDESTCOLOR, D3DBLEND_SRCALPHASAT
};

} // anonymous namespace

namespace enigma {

void graphics_state_flush_samplers() {
  for (int i = 0; i < 8; ++i) {
    const Sampler& sampler = samplers[i];
    const auto tex = get_texture_peer(sampler.texture);
    d3ddev->SetTexture(i, tex);
    if (tex == NULL) continue; // texture doesn't exist skip updating the sampler
    d3ddev->SetTextureStageState(i, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    d3ddev->SetSamplerState(i, D3DSAMP_ADDRESSU, sampler.wrapu?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP);
    d3ddev->SetSamplerState(i, D3DSAMP_ADDRESSV, sampler.wrapv?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP);
    d3ddev->SetSamplerState(i, D3DSAMP_ADDRESSW, sampler.wrapw?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP);
    d3ddev->SetSamplerState(i, D3DSAMP_MINFILTER, sampler.interpolate?D3DTEXF_LINEAR:D3DTEXF_POINT);
    d3ddev->SetSamplerState(i, D3DSAMP_MAGFILTER, sampler.interpolate?D3DTEXF_LINEAR:D3DTEXF_POINT);
  }
}

void graphics_state_flush_fog() {
  d3ddev->SetRenderState(D3DRS_RANGEFOGENABLE, d3dFogEnabled);
  d3ddev->SetRenderState(D3DRS_FOGTABLEMODE, fogmodes[d3dFogMode]);
  d3ddev->SetRenderState(D3DRS_FOGVERTEXMODE, fogmodes[d3dFogMode]);
  d3ddev->SetRenderState(D3DRS_FOGCOLOR, *d3dFogColor);

  // NOTE: DWORD is 32 bits maximum meaning it can only hold single-precision
  // floats, so yes, we must cast double to float first too.
  // https://docs.microsoft.com/en-us/windows/desktop/direct3d9/d3drenderstatetype
  d3ddev->SetRenderState(D3DRS_FOGSTART, alias_cast<DWORD>((float)d3dFogStart));
  d3ddev->SetRenderState(D3DRS_FOGEND, alias_cast<DWORD>((float)d3dFogEnd));
  d3ddev->SetRenderState(D3DRS_FOGDENSITY, alias_cast<DWORD>((float)d3dFogDensity));
}

void graphics_state_flush_lighting() {
  // just in case the user does turn on lighting, we need to set a material
  // that works for lighting vertices that have no color data
  D3DMATERIAL9 mtrl = {};
  mtrl.Ambient.r = mtrl.Ambient.g = mtrl.Ambient.b = mtrl.Ambient.a = 1.0;
  mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0;
  d3ddev->SetMaterial(&mtrl);
  d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
  d3ddev->SetRenderState(D3DRS_SHADEMODE, d3dShading?D3DSHADE_GOURAUD:D3DSHADE_FLAT);
  d3ddev->SetRenderState(D3DRS_AMBIENT, *d3dLightingAmbient);

  for (int i = 0; i < 8; ++i) {
    const bool enabled = (i<d3dLightsActive);

    d3ddev->LightEnable(i, enabled);
    if (!enabled) continue; // don't bother updating disabled lights

    const Light& light = get_active_light(i);

    D3DLIGHT9 d3dLight = { };
    d3dLight.Type = light.directional ? D3DLIGHT_DIRECTIONAL : D3DLIGHT_POINT;
    d3dLight.Diffuse = {float(COL_GET_Rf(light.color)), float(COL_GET_Gf(light.color)), float(COL_GET_Bf(light.color)), 1.0f};
    d3dLight.Position = d3dLight.Direction = {(float)light.x, (float)light.y, (float)light.z};
    d3dLight.Range = light.range;
    d3dLight.Attenuation0 = 1.0f;    // no constant inverse attenuation
    d3dLight.Attenuation1 = 0.0f;    // only .125 inverse attenuation
    d3dLight.Attenuation2 = 0.0f;    // no square inverse attenuation
    //d3dLight.Phi = gs_angle_to_radians(360.0f);    // set the outer cone to 360 degrees
    //d3dLight.Theta = gs_angle_to_radians(360.0f);    // set the inner cone to 360 degrees
    d3dLight.Falloff = 1.0f;    // use the typical falloff

    d3ddev->SetLight(i, &d3dLight); // send the light off to d3d
  }
}

void graphics_state_flush() {
  d3ddev->SetRenderState(D3DRS_POINTSIZE, drawPointSize);
  d3ddev->SetRenderState(D3DRS_FILLMODE, fillmodes[drawFillMode]);

  d3ddev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, msaaEnabled);
  d3ddev->SetRenderState(D3DRS_ZENABLE, d3dHidden);
  d3ddev->SetRenderState(D3DRS_ZFUNC, depthoperators[d3dDepthOperator]);
  d3ddev->SetRenderState(D3DRS_ZWRITEENABLE, d3dZWriteEnable);
  d3ddev->SetRenderState(D3DRS_CULLMODE, cullingstates[d3dCulling]);

  DWORD colorWriteMask = 0;
  if (enigma::colorWriteEnable[0]) colorWriteMask |= D3DCOLORWRITEENABLE_RED;
  if (enigma::colorWriteEnable[1]) colorWriteMask |= D3DCOLORWRITEENABLE_GREEN;
  if (enigma::colorWriteEnable[2]) colorWriteMask |= D3DCOLORWRITEENABLE_BLUE;
  if (enigma::colorWriteEnable[3]) colorWriteMask |= D3DCOLORWRITEENABLE_ALPHA;
  d3ddev->SetRenderState(D3DRS_COLORWRITEENABLE, colorWriteMask);
  d3ddev->SetRenderState(D3DRS_SRCBLEND, blendequivs[(blendMode[0]-1)%11]);
  d3ddev->SetRenderState(D3DRS_DESTBLEND, blendequivs[(blendMode[1]-1)%11]);
  d3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE, alphaBlend);
  d3ddev->SetRenderState(D3DRS_ALPHATESTENABLE, alphaTest);
  d3ddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
  d3ddev->SetRenderState(D3DRS_ALPHAREF, alphaTestRef);

  graphics_state_flush_samplers();

  d3ddev->SetRenderState(D3DRS_FOGENABLE, d3dFogEnabled);
  if (d3dFogEnabled) graphics_state_flush_fog();

  d3ddev->SetRenderState(D3DRS_LIGHTING, d3dLighting);
  if (d3dLighting) graphics_state_flush_lighting();

  d3ddev->SetTransform(D3DTS_WORLD, (D3DMATRIX*)glm::value_ptr(world));
  d3ddev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)glm::value_ptr(view));
  d3ddev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)glm::value_ptr(projection));
}

} // namespace enigma

namespace enigma_user {

void d3d_clear_depth(double value) {
  draw_batch_flush(batch_flush_deferred);
  d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), value, 0);
}

void d3d_stencil_clear_value(int value) {
  draw_batch_flush(batch_flush_deferred);
  d3ddev->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 0, value);
}

void d3d_stencil_clear() {
  draw_batch_flush(batch_flush_deferred);
  d3ddev->Clear(0, NULL, D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 0, 0);
}

void d3d_set_software_vertex_processing(bool software) {
  d3ddev->SetSoftwareVertexProcessing(software);
}

} // namespace enigma_user
