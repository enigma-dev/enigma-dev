#include "Graphics_Systems/OpenGL-Common/version.h"

namespace enigma {

const gl_profile_type graphics_opengl_profile = gl_profile_es;
const unsigned int gl_major = 3, gl_minor = 0; 
const std::string shader_header = "#version 300 es\nprecision mediump float;\n";

GLenum shadertypes[5] = {
  GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER
};

} // namespace enigma
