#include "GLload.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <GL/glew.h>

namespace enigma {

void gl_load_exts() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    DEBUG_MESSAGE(std::string("Failed to initialize glew for OpenGL. ") + (const char*)glewGetErrorString(err), MESSAGE_TYPE::M_FATAL_ERROR);
}

}
