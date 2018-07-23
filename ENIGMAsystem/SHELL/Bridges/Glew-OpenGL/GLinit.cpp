#include "Bridges/General/GLinit.h"

bool graphics_is_gles = false;

void gl_init() {
#ifdef DEBUG_MODE
  GLenum err = glewInit();
  if (GLEW_OK != err)
    std::cerr << "GLEW ERROR!" << std::endl;
#else
  glewInit();
#endif
}
