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

#include "Graphics_Systems/General/GSprimitives.h"
#include "GLSLshader.h"
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
      case GL_UNSIGNED_INT_VEC2: return 2;
      case GL_UNSIGNED_INT_VEC3: return 3;
      case GL_UNSIGNED_INT_VEC4: return 4;
      case GL_BOOL: return 1;
      case GL_BOOL_VEC2: return 2;
      case GL_BOOL_VEC3: return 3;
      case GL_BOOL_VEC4: return 4;
      case GL_FLOAT_MAT2: return 4;
      case GL_FLOAT_MAT3: return 9;
      case GL_FLOAT_MAT4: return 16;
      case GL_FLOAT_MAT2x3: return 6;
      case GL_FLOAT_MAT2x4: return 8;
      case GL_FLOAT_MAT3x2: return 6;
      case GL_FLOAT_MAT3x4: return 12;
      case GL_FLOAT_MAT4x2: return 8;
      case GL_FLOAT_MAT4x3: return 12;
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

namespace enigma_user
{

void glsl_uniformui(int location, unsigned v0) {
  get_uniform(it,location,1);
  if (it->second.data[0].ui != v0){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1ui(location, v0);
    it->second.data[0].ui = v0;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1) {
  get_uniform(it,location,2);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2ui(location, v0, v1);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2) {
  get_uniform(it,location,3);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1 || it->second.data[2].ui != v2){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3ui(location, v0, v1, v2);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1, it->second.data[2].ui = v2;
  }
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3) {
  get_uniform(it,location,4);
  if (it->second.data[0].ui != v0 || it->second.data[1].ui != v1 || it->second.data[2].ui != v2 || it->second.data[3].ui != v3){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4ui(location, v0, v1, v2, v3);
    it->second.data[0].ui = v0, it->second.data[1].ui = v1, it->second.data[2].ui = v2, it->second.data[3].ui = v3;
  }
}

////////////////////////VECTOR FUNCTIONS FOR UNSIGNED INT UNIFORMS/////////////////
void glsl_uniform1uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,1);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform1uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform2uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,2);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform2uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform3uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,3);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform3uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

void glsl_uniform4uiv(int location, int size, const unsigned int *value){
  get_uniform(it,location,4);
  if (std::equal(it->second.data.begin(), it->second.data.end(), value, enigma::UATypeUIComp) == false){
    enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
    glUniform4uiv(location, size, value);
    for (size_t i=0; i<it->second.data.size(); ++i){
      it->second.data[i].ui = value[i];
    }
  }
}

}
