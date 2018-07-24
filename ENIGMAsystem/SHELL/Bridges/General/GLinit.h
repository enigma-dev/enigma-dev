void gl_init();

using GLenum = unsigned;
extern GLenum GL_BGRA;
extern unsigned int gl_major, gl_minor;
extern bool graphics_is_gles;
using GLfloat = float;
void glClearDepth(GLfloat depth);

#include "OpenGLHeaders.h"
