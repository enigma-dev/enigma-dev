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

#ifndef ENIGMA_DX11SHADER_H
#define ENIGMA_DX11SHADER_H

#include <string>

namespace enigma_user {

enum {
  sh_unknown = 0,
  sh_vertex = 1,
  sh_pixel = 2
};

int hlsl_shader_create(int type);
int hlsl_shader_load(int id, std::string fname);
void hlsl_shader_set_constantf(int id, unsigned start, const float* data, unsigned count);
void hlsl_shader_set_vector(int id, std::string name, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar w);
void hlsl_shader_use(int id);
void hlsl_shader_reset();
void hlsl_shader_free(int id);

} // namespace enigma_user

#endif
