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

#include "../General/OpenGLHeaders.h"
#include "GL3shader.h"
#include <math.h>

#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

#include <vector>
using std::vector;

GLenum shadertypes[5] = {   
  GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
}; 

struct Shader{
  const char* log;
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

vector<Shader*> shaders(0);
vector<ShaderProgram*> shaderprograms(0);

namespace enigma_user
{

int shader_create(int type)
{
  unsigned int id = shaders.size();
  shaders.push_back(new Shader(type));
  return id;
}

}

unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;
    
    unsigned long pos=file.tellg();
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);
    
    return len;
}

namespace enigma_user
{

int shader_load(int id, string fname)
{
  ifstream file;
  file.open(fname.c_str(), ios::in); // opens as ASCII
  if (!file.is_open()) return 1; // Error: File could not be oppenned

  unsigned long len = getFileLength(file);
  if (len == 0) return 2;   // Error: Empty File 
  
  GLchar* ShaderSource;
  ShaderSource = (GLchar*) new char[len+1];

  ShaderSource[len] = 0;

  unsigned int i=0;
  while (file.good())
  {
    ShaderSource[i] = file.get();       // get character from file.
    if (!file.eof())
    i++;
  }
    
  ShaderSource[i] = 0;  // 0-terminate it at the correct position

  glShaderSource(shaders[id]->shader, 1, (const GLchar**)&ShaderSource, NULL);

  file.close();
  return 3; // No Error
}

bool shader_compile(int id)
{
  glCompileShader(shaders[id]->shader);

  GLint blen = 0;	
  GLsizei slen = 0;

  glGetShaderiv(shaders[id]->shader, GL_INFO_LOG_LENGTH , &blen);       

  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);

    glGetInfoLogARB(shaders[id]->shader, blen, &slen, compiler_log);
    shaders[id]->log = compiler_log;
  } else {
    shaders[id]->log = "Shader compile log empty";
  }

  GLint compiled;
  glGetProgramiv(shaders[id]->shader, GL_COMPILE_STATUS, &compiled);
  if (compiled)
  {
    return true;
  } else {
    return false;
  }
}

const char* shader_compile_output(int id)
{
  return shaders[id]->log;
}

void shader_free(int id)
{
  delete shaders[id];
}

int shader_program_create()
{
  unsigned int id = shaderprograms.size();
  shaderprograms.push_back(new ShaderProgram());
  return id;
}

bool shader_program_link(int id)
{
//glBindFragDataLocation(shaderprograms[id]->shaderprogram, 0, "fragColor");

  glLinkProgram(shaderprograms[id]->shaderprogram);
  GLint linked;
  glGetProgramiv(shaderprograms[id]->shaderprogram, GL_LINK_STATUS, &linked);
  if (linked)
  {
    return true;
  } else {
    return false;
  }
}

bool shader_program_validate(int id)
{
  glValidateProgram(shaderprograms[id]->shaderprogram);
  GLint validated;
  glGetProgramiv(shaderprograms[id]->shaderprogram, GL_VALIDATE_STATUS, &validated);
  if (validated)
  {
    return true;
  } else {
    return false;
  }
}

void shader_program_attach(int id, int sid)
{
  glAttachShader(shaderprograms[id]->shaderprogram, shaders[sid]->shader);
}

void shader_program_detach(int id, int sid)
{
  glDetachShader(shaderprograms[id]->shaderprogram, shaders[sid]->shader);
}

void shader_program_use(int id)
{
  glUseProgram(shaderprograms[id]->shaderprogram);
}

void shader_program_reset()
{
  glUseProgram(0);
}

void shader_program_free(int id)
{
  delete shaderprograms[id];
}

}

