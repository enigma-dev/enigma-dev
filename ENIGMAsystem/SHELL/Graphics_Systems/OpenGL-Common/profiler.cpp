/** Copyright (C) 2014 Harijs Grinbergs
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

#include "profiler.h"

namespace enigma {

GPUProfiler gpuprof;

} // namespace enigma

namespace enigma_user {

int profiler_get_vertex_count() { return enigma::gpuprof.drawn_vertex_number; }
int profiler_get_drawcall_count() { return enigma::gpuprof.drawn_drawcall_number; }
int profiler_get_vbo_count() { return enigma::gpuprof.drawn_vbo_number; }

} // namespace enigma_user
