/** Copyright (C) 2008-2014 Josh Ventura, DatZach, Polygone, Harijs Grinbergs
*** Copyright (C) 2013-2014, 2019 Robert Colton
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

#include "GSd3d.h"
#include "GSstdraw.h"
#include "GSmatrix.h"
#include "GSprimitives.h"
#include "GScolors.h"
#include "GScolor_macros.h"

#include "Universal_System/roomsystem.h" // for view variables

namespace enigma {

bool d3dMode=false, d3dHidden=false, d3dClipPlane=false, d3dZWriteEnable=true,
     d3dPerspective=true, d3dLighting=false, d3dShading=true;
int d3dCulling=0, d3dDepthOperator=enigma_user::rs_lequal;
float d3dLightingAmbient[4]={0.0f,0.0f,0.0f,1.0f};

bool d3dFogEnabled=false;
int d3dFogMode=enigma_user::rs_linear, d3dFogHint=enigma_user::rs_nicest;
float d3dFogStart=0.0f, d3dFogEnd=0.0f, d3dFogDensity=0.0f;
float d3dFogColor[3]={0.0f,0.0f,0.0f};

Light d3dLights[8];
bool d3dLightEnabled[8]={false};

} // namespace enigma

namespace enigma_user {

void d3d_start() {
  enigma::draw_set_state_dirty();
  enigma::d3dMode = true;
  enigma::d3dPerspective = true;
  enigma::d3dHidden = true;
  enigma::d3dZWriteEnable = true;
  enigma::d3dCulling = rs_none;
  enigma::alphaTest = true;

  // Set up projection matrix
  d3d_set_projection_perspective(0, 0, view_wview[view_current], view_hview[view_current], 0);

  // Set up modelview matrix
  d3d_transform_set_identity();

  draw_clear(enigma_user::c_black);
  d3d_clear_depth(1.0f);
}

void d3d_end() {
  enigma::draw_set_state_dirty();
  enigma::d3dMode = false;
  enigma::d3dPerspective = false;
  enigma::d3dHidden = false;
  enigma::d3dZWriteEnable = false;
  enigma::d3dCulling = rs_none;
  enigma::alphaTest = false;

  d3d_set_projection_ortho(0, 0, view_wview[view_current], view_hview[view_current], 0); //This should probably be changed not to use views
}

void d3d_set_perspective(bool enable) {
  enigma::draw_set_state_dirty();
  // in GM8.1 and GMS v1.4 this does not take effect
  // until the next frame in screen_redraw
  enigma::d3dPerspective = enable;
}

void d3d_set_hidden(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dHidden = enable;
}

void d3d_set_zwriteenable(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dZWriteEnable = enable;
}

void d3d_set_culling(int mode) {
  enigma::draw_set_state_dirty();
  enigma::d3dCulling = mode;
}

void d3d_set_lighting(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dLighting = enable;
}

void d3d_set_shading(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dShading = enable;
}

void d3d_set_depth_operator(int mode) {
  enigma::draw_set_state_dirty();
  enigma::d3dDepthOperator = mode;
}

void d3d_set_depth(double dep) {
  enigma::draw_set_state_dirty();
  //TODO: lmao we still haven't figured out what this is
}

void d3d_set_clip_plane(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dClipPlane = enable;
}

void d3d_set_fog(bool enable, int color, double start, double end) {
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
}

void d3d_set_fog_enabled(bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogEnabled = enable;
}

void d3d_set_fog_mode(int mode) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogMode = mode;
}

void d3d_set_fog_hint(int mode) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogHint = mode;
}

void d3d_set_fog_color(int color) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogColor[0] = COL_GET_Rf(color);
  enigma::d3dFogColor[1] = COL_GET_Gf(color);
  enigma::d3dFogColor[2] = COL_GET_Bf(color);
}

void d3d_set_fog_start(double start) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogStart = start;
}

void d3d_set_fog_end(double end) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogEnd = end;
}

void d3d_set_fog_density(double density) {
  enigma::draw_set_state_dirty();
  enigma::d3dFogDensity = density;
}

void d3d_light_define_ambient(int col) {
  enigma::draw_set_state_dirty();
  enigma::d3dLightingAmbient[0] = float(COL_GET_Rf(col));
  enigma::d3dLightingAmbient[1] = float(COL_GET_Gf(col));
  enigma::d3dLightingAmbient[2] = float(COL_GET_Bf(col));
}

void d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col) {
  enigma::draw_set_state_dirty();
  enigma::d3dLights[id].x = dx;
  enigma::d3dLights[id].y = dy;
  enigma::d3dLights[id].z = dz;
  enigma::d3dLights[id].color = col;
  enigma::d3dLights[id].directional = true;
}

void d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col) {
  enigma::draw_set_state_dirty();
  enigma::d3dLights[id].x = x;
  enigma::d3dLights[id].y = y;
  enigma::d3dLights[id].z = z;
  enigma::d3dLights[id].range = range;
  enigma::d3dLights[id].color = col;
  enigma::d3dLights[id].directional = false;
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a) {
  enigma::draw_set_state_dirty();
}

void d3d_light_set_ambient(int id, int r, int g, int b, double a) {
  enigma::draw_set_state_dirty();
}

void d3d_light_set_specularity(int id, int r, int g, int b, double a) {
  enigma::draw_set_state_dirty();
}

void d3d_light_shininess(int facemode, int shine) {
  enigma::draw_set_state_dirty();
}

void d3d_light_enable(int id, bool enable) {
  enigma::draw_set_state_dirty();
  enigma::d3dLightEnabled[id] = enable;
}

bool d3d_get_mode() { return enigma::d3dMode; }
bool d3d_get_perspective() { return enigma::d3dPerspective; }
bool d3d_get_hidden() { return enigma::d3dHidden; }
bool d3d_get_zwriteenable() { return enigma::d3dZWriteEnable; }
int d3d_get_culling() { return enigma::d3dCulling; }
bool d3d_get_lighting() { return enigma::d3dLighting; }
bool d3d_get_shading() { return enigma::d3dShading; }

} // namespace enigma_user
