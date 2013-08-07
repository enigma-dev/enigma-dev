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

#ifndef _GLSHADER__H
#define _GLSHADER__H

#include <iostream>
#include <fstream>
using namespace std;

namespace enigma_user
{

enum {
  sh_vertex = 0,
  sh_tesscontrol = 1,
  sh_tessevaluation = 2,
  sh_geometry = 3,
  sh_fragment = 4
};

int glsl_shader_create(int type);
int glsl_shader_load(int id, string fname);
bool glsl_shader_compile(int id);
string glsl_shader_compile_output(int id);
void glsl_shader_free(int id);

int glsl_program_create();
bool glsl_program_link(int id);
bool glsl_program_validate(int id);
void glsl_program_attach(int id, int sid);
void glsl_program_detach(int id, int sid);
void glsl_program_bind_frag_data(int id, string name);
void glsl_program_use(int id);
void glsl_program_reset();
void glsl_program_free(int id);

}

#endif
