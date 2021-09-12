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

#ifndef ENIGMA_GLSLSHADER_H
#define ENIGMA_GLSLSHADER_H

#include "OpenGLHeaders.h"
#include "Universal_System/Resources/AssetArray.h"

#include <string>

using std::string;

namespace enigma {

extern GLenum shadertypes[];

struct Shader{
  string log;
  GLuint shader;
  Shader(int type): log(""), shader(0), destroyed(false)
  {
    shader = glCreateShader(shadertypes[type]);
  }

  void destroy()
  {
    glDeleteShader(shader);
    destroyed = true;
  }
  bool isDestroyed() const { return destroyed; }

  static const char* getAssetTypeName() { return "shader"; }

private:
  bool destroyed;
};

struct ShaderProgram{
  GLuint shaderprogram;

  ShaderProgram(): shaderprogram(0), destroyed(false)
  {
    shaderprogram = glCreateProgram();
  }

  void destroy()
  {
    glDeleteProgram(shaderprogram);
    destroyed = true;
  }
  bool isDestroyed() const { return destroyed; }

  static const char* getAssetTypeName() { return "shader program"; }

private:
  bool destroyed;
};

extern AssetArray<Shader> shaders;
extern AssetArray<ShaderProgram> shaderprograms;

}

#endif
