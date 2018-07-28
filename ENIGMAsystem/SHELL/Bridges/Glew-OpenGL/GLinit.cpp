#include "Bridges/General/GLinit.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>

bool graphics_is_gles = false;

void gl_init() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    show_error(std::string("Failed to initialize glew for OpenGL. ") + glewGetErrorString(err), true);
}
