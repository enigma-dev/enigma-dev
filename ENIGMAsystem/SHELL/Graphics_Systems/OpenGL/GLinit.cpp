#include "glad.h"

void opengl_initialize() {
  gladLoadGLLoader(SDL_GL_GetProcAddress);
}
