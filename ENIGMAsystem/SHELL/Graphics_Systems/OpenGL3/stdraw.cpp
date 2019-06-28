#include "OpenGLHeaders.h"

namespace enigma_user {

int draw_get_msaa_maxlevel() {
  int maximumMSAA;
  glGetIntegerv(GL_MAX_SAMPLES, &maximumMSAA);
  return maximumMSAA;
}

bool draw_get_msaa_supported() { return GLEW_EXT_multisample; }

} // namespace enigma_user
