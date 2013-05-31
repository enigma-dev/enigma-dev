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

#ifndef _DX10SHADER__H
#define _DX10SHADER__H

namespace enigma_user
{

enum {
  sh_vertex = 0,
  sh_tesscontrol = 1,
  sh_tessevaluation = 2,
  sh_geometry = 3,
  sh_fragment = 4
};

int shader_create(int type);
int shader_load(int id, const char* fname);
bool shader_compile(int id);
const char* shader_compile_output(int id);
void shader_free(int id);

int shader_program_create();
bool shader_program_link(int id);
bool shader_program_validate(int id);
void shader_program_attach(int id, int sid);
void shader_program_detach(int id, int sid);
void shader_program_bind_frag_data(int id, const char* name);
void shader_program_use(int id);
void shader_program_reset();
void shader_program_free(int id);

}

#endif
