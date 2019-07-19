#include "GLload.h"

#ifdef __ANDROID__
#include <epoxy/gl.h>
#include <SDL2/SDL.h>
#endif

namespace enigma {

// this isn't needed by epoxy used for GLES
void gl_load_exts() {
  #ifdef __ANDROID__
  gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
  #endif
}

}
