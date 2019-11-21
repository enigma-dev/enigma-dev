#include "Graphics_Systems/OpenGL-Common/version.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace enigma {
namespace OpenGL {

int getGLTypeSize(GLuint type){
  switch (type){
    case GL_FLOAT: return 1;
    case GL_FLOAT_VEC2: return 2;
    case GL_FLOAT_VEC3: return 3;
    case GL_FLOAT_VEC4: return 4;
    case GL_INT: return 1;
    case GL_INT_VEC2: return 2;
    case GL_INT_VEC3: return 3;
    case GL_INT_VEC4: return 4;
    case GL_UNSIGNED_INT: return 1;
    case GL_BOOL: return 1;
    case GL_BOOL_VEC2: return 2;
    case GL_BOOL_VEC3: return 3;
    case GL_BOOL_VEC4: return 4;
    case GL_FLOAT_MAT2: return 4;
    case GL_FLOAT_MAT3: return 9;
    case GL_FLOAT_MAT4: return 16;
    default: { DEBUG_MESSAGE("getGLTypeSize Asking size for unknown type - " + std::to_string(type), MESSAGE_TYPE::M_ERROR); return 1; }
  }
}

}
} //namespace enigma
