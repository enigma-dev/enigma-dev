#include "OpenGLHeaders.h"

namespace enigma {

void openglCallbackFunction(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                     const GLchar* message, const void* userParam);
void register_opengl_debug_callback();

} // namespace enigma
