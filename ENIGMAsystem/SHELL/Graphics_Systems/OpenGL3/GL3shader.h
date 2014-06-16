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

#ifndef _GLSHADER__H
#define _GLSHADER__H

#include "../General/OpenGLHeaders.h"
#include <string>
using std::string;

namespace enigma
{
    string getVertexShaderPrefix();
    string getFragmentShaderPrefix();
    string getDefaultFragmentShader();
    string getDefaultVertexShader();
    void getDefaultUniforms(int prog_id);
    void getDefaultAttributes(int prog_id);
    void getUniforms(int prog_id);
    void getAttributes(int prog_id);
    int getGLTypeSize(GLuint type);
}

namespace enigma_user
{

enum {
  sh_vertex = 0,
  sh_fragment = 1,
  sh_tesscontrol = 2,
  sh_tessevaluation = 3,
  sh_geometry = 4
};

int glsl_shader_create(int type);
bool glsl_shader_load(int id, string fname);
bool glsl_shader_load_string(int id, string shaderSource);
bool glsl_shader_compile(int id);
bool glsl_shader_get_compiled(int id);
void glsl_shader_free(int id);
string glsl_shader_get_infolog(int id);

void glsl_program_print_infolog(int id);
void glsl_shader_print_infolog(int id);

int glsl_program_create();
bool glsl_program_link(int id);
bool glsl_program_validate(int id);
void glsl_program_attach(int id, int sid);
void glsl_program_detach(int id, int sid);
string glsl_program_get_infolog(int id);

void glsl_program_set(int id);
void glsl_program_reset();
void glsl_program_free(int id);
void glsl_program_default_set(int id); //Override default shader
void glsl_program_default_reset(); //Reset back the default shader

int glsl_get_uniform_location(int program, string name);
int glsl_get_attribute_location(int program, string name);

void glsl_uniformf(int location, float v0);
void glsl_uniformf(int location, float v0, float v1);
void glsl_uniformf(int location, float v0, float v1, float v2);
void glsl_uniformf(int location, float v0, float v1, float v2, float v3);
void glsl_uniformi(int location, int v0);
void glsl_uniformi(int location, int v0, int v1);
void glsl_uniformi(int location, int v0, int v1, int v2);
void glsl_uniformi(int location, int v0, int v1, int v2, int v3);
void glsl_uniformui(int location, unsigned v0);
void glsl_uniformui(int location, unsigned v0, unsigned v1);
void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2);
void glsl_uniformui(int location, unsigned v0, unsigned v1, unsigned v2, unsigned v3);

void glsl_uniform1fv(int location, int size, const float *value);
void glsl_uniform2fv(int location, int size, const float *value);
void glsl_uniform3fv(int location, int size, const float *value);
void glsl_uniform4fv(int location, int size, const float *value);

void glsl_uniform1iv(int location, int size, const float *value);
void glsl_uniform2iv(int location, int size, const float *value);
void glsl_uniform3iv(int location, int size, const float *value);
void glsl_uniform4iv(int location, int size, const float *value);

void glsl_uniform1uiv(int location, int size, const float *value);
void glsl_uniform2uiv(int location, int size, const float *value);
void glsl_uniform3uiv(int location, int size, const float *value);
void glsl_uniform4uiv(int location, int size, const float *value);

// Wrap our abstracted version to the GameMaker version
#define shader_set            glsl_program_set
#define shader_reset          glsl_program_reset
#define shader_get_uniform    glsl_get_uniform_location
#define shader_get_sampler_index glsl_get_uniform_location
#define shader_set_uniform_f  glsl_uniformf
#define shader_set_uniform_i  glsl_uniformi
#define shader_is_compiled    glsl_shader_get_compiled

}

#endif
