/** Copyright (C) 2013-2014 Robert B. Colton, Harijs Grinbergs
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "OpenGLHeaders.h"

#include <stdio.h>      /* printf, scanf, NULL */

namespace enigma
{

  int getGLTypeSize(GLuint type) {
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
#ifdef GL_SAMPLER_1D
      case GL_SAMPLER_1D: return 1;
#endif
#ifdef GL_SAMPLER_2D
      case GL_SAMPLER_2D: return 1;
#endif
#ifdef GL_SAMPLER_3D
      case GL_SAMPLER_3D: return 1;
#endif

      default: { printf("getGLTypeSize Asking size for unknown type - %i!\n", type); return 1; }
    }
  }

}
