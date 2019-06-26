#include "GLload.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <GL/glew.h>

#ifdef DEBUG_MODE
#include "Bridges/OpenGLDebug/OpenGLDebug.h"
#endif

namespace enigma {

void gl_load_exts() {
  GLenum err = glewInit();
  if (GLEW_OK != err)
    enigma_user::show_error(std::string("Failed to initialize glew for OpenGL. ") + (const char*)glewGetErrorString(err), true);
    
  #ifdef DEBUG_MODE
  register_opengl_debug_callback();
  #endif
}

} // namespace enigma
