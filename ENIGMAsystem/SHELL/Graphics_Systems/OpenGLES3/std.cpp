#include "Graphics_Systems/OpenGL-Common/version.h"

namespace enigma {

const gl_profile_type graphics_opengl_profile = gl_profile_es;
const unsigned int gl_major = 3, gl_minor = 2; 
const std::string shader_header = "#version 320 es\nprecision mediump float;\n";

} // namespace enigma
