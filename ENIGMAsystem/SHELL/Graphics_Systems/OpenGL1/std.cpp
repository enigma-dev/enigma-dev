/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include "std.h"
#include "shader.h"
#include "GLSLshader.h"

#include "Graphics_Systems/OpenGL-Common/version.h"
#include "Graphics_Systems/OpenGL-Common/screen.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/shaderstruct.h"
#include "Universal_System/var4.h"

#include <iostream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */

using namespace std;

namespace enigma {
  const gl_profile_type graphics_opengl_profile = gl_profile_compat;
  const unsigned int gl_major = 1, gl_minor = 1;

  void register_gl_debug_callback() {}
  void init_vao() {}

  void graphicssystem_initialize()
  {
    gl_screen_init();
    graphics_init_vbo_method();

    glEnable(GL_SCISSOR_TEST); // constrain clear to viewport like D3D9
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
    glDepthFunc(GL_LEQUAL); // to match GM8's D3D8 default

    init_shaders();
    // read shaders into graphics system structure and compile and link them if needed
    for (size_t i = 0; i < shader_idmax; i++) {
      ShaderStruct* shaderstruct = shaderdata[i];

      //if (string(shaderstruct.type) != string("GLSL")) { continue; }

      auto& vshader = shaders[shaders.add(Shader(enigma_user::sh_vertex))];
      glShaderSource(vshader.shader, 1, (const GLchar**)&shaderstruct->vertex, NULL);

      auto& fshader = shaders[shaders.add(Shader(enigma_user::sh_fragment))];
      glShaderSource(fshader.shader, 1, (const GLchar**)&shaderstruct->fragment, NULL);

      auto& program = shaderprograms[shaderprograms.add(ShaderProgram())];

      if (shaderstruct->precompile) {
        glCompileShader(vshader.shader);
        glCompileShader(fshader.shader);

        GLint blen = 0;
        GLsizei slen = 0;

        glGetShaderiv(vshader.shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
          GLchar* compiler_log = (GLchar*)malloc(blen);

          glGetInfoLogARB(vshader.shader, blen, &slen, compiler_log);
          DEBUG_MESSAGE(compiler_log, MESSAGE_TYPE::M_ERROR);
        } else {
          DEBUG_MESSAGE("Vertex shader compile log empty", MESSAGE_TYPE::M_ERROR);
        }

        glGetShaderiv(fshader.shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
          GLchar* compiler_log = (GLchar*)malloc(blen);

          glGetInfoLogARB(fshader.shader, blen, &slen, compiler_log);
          DEBUG_MESSAGE(compiler_log, MESSAGE_TYPE::M_ERROR);
        } else {
          DEBUG_MESSAGE("Fragment shader compile log empty", MESSAGE_TYPE::M_ERROR);
        }

      }

      glAttachShader(program.shaderprogram, vshader.shader);
      glAttachShader(program.shaderprogram, fshader.shader);

      glLinkProgram(program.shaderprogram);
      glValidateProgram(program.shaderprogram);
    }
  }
}

namespace enigma_user
{

// Stolen entirely from the documentation and thrown into a switch() structure.
string draw_get_graphics_error()
{
  GLenum err = glGetError();
  switch (err)
  {
    case GL_NO_ERROR:         return "";
    case GL_INVALID_ENUM:     return "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_VALUE:    return "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_INVALID_OPERATION:return "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_STACK_OVERFLOW:   return "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_STACK_UNDERFLOW:  return "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
    case GL_OUT_OF_MEMORY:    return "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
    //case GL_TABLE_TOO_LARGE:  return "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
    default: return "Unspecified error.";
  }
}

}
