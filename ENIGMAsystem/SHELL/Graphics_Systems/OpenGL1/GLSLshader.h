/** Copyright (C) 2008-2013 Robert B. Colton
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

#ifndef _GLSLSHADER__H
#define _GLSLSHADER__H

#include <vector>
#include <string>
using std::string;
using std::vector;

#include "../General/OpenGLHeaders.h"

extern GLenum shadertypes[];

struct Shader{
  string log;
  GLuint shader;
  Shader(int type) 
  {
    shader = glCreateShader(shadertypes[type]);
  }
 
  ~Shader()
  {
    glDeleteShader(shader);
  }
};

struct ShaderProgram{
  GLuint shaderprogram;

  ShaderProgram()
  {
    shaderprogram = glCreateProgram();
  }

  ~ShaderProgram()
  {
    glDeleteProgram(shaderprogram);
  }
};

extern vector<Shader*> shaders;
extern vector<ShaderProgram*> shaderprograms;

#endif
