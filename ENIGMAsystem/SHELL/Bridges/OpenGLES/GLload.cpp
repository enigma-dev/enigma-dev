#include "GLload.h"

#ifdef DEBUG_MODE
#include "Bridges/OpenGLDebug/OpenGLDebug.h"
#endif

namespace enigma {

// this isn't needed by epoxy used for GLES
void gl_load_exts() {
  #ifdef DEBUG_MODE
  register_opengl_debug_callback();
  #endif
}

}
