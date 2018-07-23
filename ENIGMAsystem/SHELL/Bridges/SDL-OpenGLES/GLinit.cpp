#include "Bridges/General/GLinit.h"

#include <SDL2/SDL.h>

GLenum GL_BGRA = GL_BGRA_EXT;
bool graphics_is_gles = true;

void gl_init() {
  gladLoadGLES2Loader(SDL_GL_GetProcAddress);
}

void glClearDepth(GLfloat depth) {
  glClearDepthf(depth);
}
