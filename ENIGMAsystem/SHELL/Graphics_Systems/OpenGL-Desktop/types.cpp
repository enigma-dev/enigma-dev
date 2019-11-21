#include "Graphics_Systems/OpenGL-Common/version.h"

namespace enigma {

int getGLTypeSize(GLuint type) {
  switch (type) {
    case GL_FLOAT_MAT2x3: return 6;
    case GL_FLOAT_MAT2x4: return 8;
    case GL_FLOAT_MAT3x2: return 6;
    case GL_FLOAT_MAT3x4: return 12;
    case GL_FLOAT_MAT4x2: return 8;
    case GL_FLOAT_MAT4x3: return 12;
    case GL_SAMPLER_1D: return 1;
    case GL_SAMPLER_2D: return 1;
    case GL_SAMPLER_3D: return 1;
    case GL_UNSIGNED_INT_VEC2: return 2;
    case GL_UNSIGNED_INT_VEC3: return 3;
    case GL_UNSIGNED_INT_VEC4: return 4;
    default: return OpenGL::getGLTypeSize(type);
  }
}

} //namespace enigma
