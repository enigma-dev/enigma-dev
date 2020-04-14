#ifndef ENIGMA_OPENGL_COMMON_VERSION_H
#define ENIGMA_OPENGL_COMMON_VERSION_H

#include "OpenGLHeaders.h"

#include <string>

namespace enigma {

enum gl_profile_type {
  gl_profile_core = 0,
  gl_profile_compat,
  gl_profile_es
};

extern const gl_profile_type graphics_opengl_profile;
extern const unsigned int gl_major, gl_minor; 
extern const std::string shader_header;

void graphics_flush_ext();
void register_gl_debug_callback();
void init_vao();
void gl_screen_init();
void graphics_init_vbo_method();

namespace OpenGL {
int getGLTypeSize(GLuint type);
} // namespace OpenGL

} // namespace enigma

#endif
