#include "GLinit.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <GL/glew.h>

namespace enigma {

void gl_init() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    show_error(std::string("Failed to initialize glew for OpenGL. ") + (const char*)glewGetErrorString(err), true);
}

}
