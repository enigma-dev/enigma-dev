#include "GSd3d.h"

namespace enigma {

bool d3dMode=false, d3dPerspective=false, d3dHidden=false, d3dZWriteEnable=true;
int d3dCulling = enigma_user::rs_none;

} // namespace enigma

namespace enigma_user {

void d3d_set_perspective(bool enable) {
  // in GM8.1 and GMS v1.4 this does not take effect
  // until the next frame in screen_redraw
  enigma::d3dPerspective = enable;
}

bool d3d_get_perspective() {
  return enigma::d3dPerspective;
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

} // namespace enigma_user
