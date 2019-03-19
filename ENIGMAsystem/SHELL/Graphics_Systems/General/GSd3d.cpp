#include "GSd3d.h"
#include "GSstdraw.h"
#include "GSmatrix.h"
#include "GSprimitives.h"
#include "GScolors.h"
#include "GScolor_macros.h"

#include "Universal_System/roomsystem.h" // for view variables

namespace enigma {

bool d3dMode = false, d3dHidden = false, d3dZWriteEnable = true, d3dPerspective = true, d3dLighting = false, d3dShading = true;
int d3dCulling = 0;
float d3dLightingAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

} // namespace enigma

namespace enigma_user {

void d3d_start()
{
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
  d3d_clear_depth(0.0f);
}

void d3d_end()
{
  enigma::d3dMode = false;
  enigma::d3dPerspective = false;
  enigma::d3dHidden = false;
  enigma::d3dZWriteEnable = false;
  enigma::d3dCulling = rs_none;
  enigma::alphaTest = false;

  d3d_set_projection_ortho(0, 0, view_wview[view_current], view_hview[view_current], 0); //This should probably be changed not to use views
}

void d3d_set_perspective(bool enable) {
  // in GM8.1 and GMS v1.4 this does not take effect
  // until the next frame in screen_redraw
  enigma::d3dPerspective = enable;
}

void d3d_set_hidden(bool enable) {
  enigma::d3dHidden = enable;
}

void d3d_set_zwriteenable(bool enable) {
  enigma::d3dZWriteEnable = enable;
}

void d3d_set_culling(int mode) {
  enigma::d3dCulling = mode;
}

void d3d_set_lighting(bool enable) {
  enigma::d3dLighting = enable;
}

void d3d_set_shading(bool enable) {
  enigma::d3dShading = enable;
}

bool d3d_get_perspective() {
  return enigma::d3dHidden;
}

bool d3d_get_mode() {
  return enigma::d3dMode;
}

bool d3d_get_hidden() {
  return enigma::d3dHidden;
}

bool d3d_get_zwriteenable() {
  return enigma::d3dZWriteEnable;
}

int d3d_get_culling() {
  return enigma::d3dCulling;
}

bool d3d_get_lighting() {
  return enigma::d3dLighting;
}

bool d3d_get_shading() {
  return enigma::d3dShading;
}

void d3d_light_define_ambient(int col)
{
  enigma::d3dLightingAmbient[0] = float(COL_GET_Rf(col));
  enigma::d3dLightingAmbient[1] = float(COL_GET_Gf(col));
  enigma::d3dLightingAmbient[2] = float(COL_GET_Bf(col));
}

} // namespace enigma_user
