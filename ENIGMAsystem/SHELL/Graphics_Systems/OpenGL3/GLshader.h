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

#ifndef _GLVERTEXBUFFER__H
#define _GLVERTEXBUFFER__H

#include <vector>
using std::vector;

struct Shader {

};

extern vector<Shader*> shaders;

int shader_glsl_create(int type); // creates a shader and returns the index
void shader_glsl_load(const char* fname); // loads the shader code from an external file
void shader_glsl_compile(int id); // compiles the shader code so that it can then be used for rendering
void shader_glsl_begin(int id); // uses the shader on all further draw calls
void shader_glsl_end(int id); // stops the shader on all further draw calls

#endif
