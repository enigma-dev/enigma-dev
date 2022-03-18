/** Copyright (C) 2013 Robert B. Colton
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

#include "shader.h"
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Universal_System/Resources/AssetArray.h"

#include <iostream>
#include <fstream>
#include <vector>

#include <math.h>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */

using namespace std;
using std::vector;

namespace enigma {

GLenum shadertypes[5] = {
  GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER
};

AssetArray<Shader> shaders;
AssetArray<ShaderProgram> shaderprograms;

void cleanup_shaders() {
  for(size_t i = 0; i < shaderprograms.size(); ++i) shaderprograms.destroy(i);
  for(size_t i = 0; i < shaders.size(); ++i) shaders.destroy(i);
}

unsigned long getFileLength(ifstream& file)
{
    if(!file.good()) return 0;

    //unsigned long pos=file.tellg();  UNUSED
    file.seekg(0,ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);

    return len;
}

}

namespace enigma_user {

using enigma::shaders;
using enigma::shaderprograms;
using enigma::ShaderProgram;
using enigma::Shader;

int glsl_shader_create(int type)
{
  return shaders.add(Shader(type));
}

int glsl_shader_load(int id, string fname)
{
  ifstream file;
  file.open(fname.c_str(), ios::in); // opens as ASCII
  if (!file.is_open()) return 1; // Error: File could not be oppenned

  unsigned long len = enigma::getFileLength(file);
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

  glShaderSource(shaders[id].shader, 1, (const GLchar**)&ShaderSource, NULL);

  file.close();
  return 3; // No Error
}

bool glsl_shader_compile(int id)
{
  glCompileShader(shaders[id].shader);

  GLint blen = 0;
  GLsizei slen = 0;

  glGetShaderiv(shaders[id].shader, GL_INFO_LOG_LENGTH , &blen);

  if (blen > 1)
  {
    GLchar* compiler_log = (GLchar*)malloc(blen);

    glGetInfoLogARB(shaders[id].shader, blen, &slen, compiler_log);
    shaders[id].log = (string)compiler_log;
  } else {
    shaders[id].log = "Shader compile log empty";
  }

  GLint compiled;
  glGetProgramiv(shaders[id].shader, GL_COMPILE_STATUS, &compiled);
  if (compiled)
  {
    return true;
  } else {
    return false;
  }
}

bool glsl_shader_get_compiled(int id) {
  GLint compiled;
  glGetProgramiv(shaders[id].shader, GL_COMPILE_STATUS, &compiled);
  if (compiled)
  {
    return true;
  } else {
    return false;
  }
}

string glsl_shader_get_infolog(int id)
{
  return shaders[id].log;
}

void glsl_shader_free(int id)
{
  shaders.destroy(id);
}

int glsl_program_create()
{
  return shaderprograms.add(ShaderProgram());
}

bool glsl_program_link(int id)
{
//glBindFragDataLocation(shaderprograms[id].shaderprogram, 0, "fragColor");

  glLinkProgram(shaderprograms[id].shaderprogram);
  GLint linked;
  glGetProgramiv(shaderprograms[id].shaderprogram, GL_LINK_STATUS, &linked);
  if (linked)
  {
    return true;
  } else {
    return false;
  }
}

bool glsl_program_validate(int id)
{
  glValidateProgram(shaderprograms[id].shaderprogram);
  GLint validated;
  glGetProgramiv(shaderprograms[id].shaderprogram, GL_VALIDATE_STATUS, &validated);
  if (validated)
  {
    return true;
  } else {
    return false;
  }
}

void glsl_program_attach(int id, int sid)
{
  glAttachShader(shaderprograms[id].shaderprogram, shaders[sid].shader);
}

void glsl_program_detach(int id, int sid)
{
  glDetachShader(shaderprograms[id].shaderprogram, shaders[sid].shader);
}

void glsl_program_set(int id)
{
  draw_batch_flush(batch_flush_deferred);
  glUseProgram(shaderprograms[id].shaderprogram);
}

void glsl_program_reset()
{
  draw_batch_flush(batch_flush_deferred);
  glUseProgram(0);
}

void glsl_program_free(int id)
{
  shaderprograms.destroy(id);
}

int glsl_get_uniform_location(int program, string name) {
	return glGetUniformLocation(shaderprograms[program].shaderprogram, name.c_str());
}

void glsl_uniformf(int location, float v0) {
  draw_batch_flush(batch_flush_deferred);
	glUniform1f(location, v0);
}

void glsl_uniformf(int location, float v0, float v1) {
  draw_batch_flush(batch_flush_deferred);
	glUniform2f(location, v0, v1);
}

void glsl_uniformf(int location, float v0, float v1, float v2) {
  draw_batch_flush(batch_flush_deferred);
	glUniform3f(location, v0, v1, v2);
}

void glsl_uniformf(int location, float v0, float v1, float v2, float v3) {
  draw_batch_flush(batch_flush_deferred);
	glUniform4f(location, v0, v1, v2, v3);
}

void glsl_uniformi(int location, int v0) {
  draw_batch_flush(batch_flush_deferred);
	glUniform1i(location, v0);
}

void glsl_uniformi(int location, int v0, int v1) {
  draw_batch_flush(batch_flush_deferred);
	glUniform2i(location, v0, v1);
}

void glsl_uniformi(int location, int v0, int v1, int v2) {
  draw_batch_flush(batch_flush_deferred);
	glUniform3i(location, v0, v1, v2);
}

void glsl_uniformi(int location, int v0, int v1, int v2, int v3) {
  draw_batch_flush(batch_flush_deferred);
	glUniform4i(location, v0, v1, v2, v3);
}

void glsl_uniformui(int location, unsigned v0) {
  draw_batch_flush(batch_flush_deferred);
	glUniform1ui(location, v0);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1) {
  draw_batch_flush(batch_flush_deferred);
	glUniform2ui(location, v0, v1);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2) {
  draw_batch_flush(batch_flush_deferred);
	glUniform3ui(location, v0, v1, v2);
}

void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3) {
  draw_batch_flush(batch_flush_deferred);
	glUniform4ui(location, v0, v1, v2, v3);
}

}
