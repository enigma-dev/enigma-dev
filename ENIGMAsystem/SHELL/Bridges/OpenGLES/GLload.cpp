#include "GLload.h"

#include <glad/glad.h>

#ifdef __ANDROID__
#include <SDL2/SDL.h>
#endif

namespace enigma {

// this isn't needed by epoxy used for GLES
void gl_load_exts() {
  gladLoadGLES2Loader((GLADloadproc)SDL_GL_GetProcAddress);
}

}
