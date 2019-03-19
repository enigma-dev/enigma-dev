#include "GSd3d.h"
#include "GSprimitives.h"

namespace enigma {

bool d3dHidden = false, d3dZWriteEnable = true, d3dPerspective = true, d3dLighting = false, d3dShading = true;
int d3dCulling = 0;

} // namespace enigma

namespace enigma_user {

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

} // namespace enigma_user
