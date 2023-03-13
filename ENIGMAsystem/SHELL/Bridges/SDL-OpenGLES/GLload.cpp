#include "Bridges/OpenGLES/GLload.h"
#include "OpenGLHeaders.h"


#include <SDL2/SDL.h>

namespace enigma {

// this isn't needed by epoxy used for GLES
void gl_load_exts() {
  gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress);
}

}
