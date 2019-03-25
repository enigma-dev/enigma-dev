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

#include <iostream>
#include <string>
#include <stdlib.h>     /* malloc, free, rand */

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
using namespace std;
#include "OPENGLStd.h"
#include "GLshader.h"
#include "GLSLshader.h"
#include "Universal_System/shaderstruct.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h" // Room dimensions.
#include "Graphics_Systems/graphics_mandatory.h" // Room dimensions.
namespace enigma
{

  void graphics_init_vbo_method();

  void graphicssystem_initialize()
  {
    graphics_init_vbo_method();

    //enigma::pbo_isgo=GL_ARB_pixel_buffer_object;
    glMatrixMode(GL_PROJECTION);
    glClearColor(0,0,0,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    using enigma_user::room_width;
    using enigma_user::room_height;
    glViewport(0,0,(int)room_width,(int)room_height);
    glOrtho(-1,(int)room_width,-1,(int)room_height,0,1);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER,0);
    glDepthFunc(GL_LEQUAL); // to match GM8's D3D8 default

    glColor4f(0,0,0,1);
    glBindTexture(GL_TEXTURE_2D,0);

	  init_shaders();
	  // read shaders into graphics system structure and compile and link them if needed
	  for (size_t i = 0; i < shader_idmax; i++) {
	    ShaderStruct* shaderstruct = shaderdata[i];

      //if (string(shaderstruct->type) != string("GLSL")) { continue; }

      Shader* vshader = new Shader(enigma_user::sh_vertex);
      shaders.push_back(vshader);
      glShaderSource(vshader->shader, 1, (const GLchar**)&shaderstruct->vertex, NULL);

      Shader* fshader = new Shader(enigma_user::sh_fragment);
      shaders.push_back(fshader);
      glShaderSource(fshader->shader, 1, (const GLchar**)&shaderstruct->fragment, NULL);

      ShaderProgram* program = new ShaderProgram();
      shaderprograms.push_back(program);

      if (shaderstruct->precompile) {
        glCompileShader(vshader->shader);
        glCompileShader(fshader->shader);

        GLint blen = 0;
        GLsizei slen = 0;

        glGetShaderiv(vshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
          GLchar* compiler_log = (GLchar*)malloc(blen);

          glGetInfoLogARB(vshader->shader, blen, &slen, compiler_log);
          std::cout << compiler_log;
        } else {
          std::cout << "Vertex shader compile log empty";
        }

        glGetShaderiv(fshader->shader, GL_INFO_LOG_LENGTH , &blen);

        if (blen > 1)
        {
          GLchar* compiler_log = (GLchar*)malloc(blen);

          glGetInfoLogARB(fshader->shader, blen, &slen, compiler_log);
          std::cout << compiler_log;
        } else {
          std::cout << "Fragment shader compile log empty";
        }

      }

      glAttachShader(program->shaderprogram, vshader->shader);
      glAttachShader(program->shaderprogram, fshader->shader);

      glLinkProgram(program->shaderprogram);
      glValidateProgram(program->shaderprogram);
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
