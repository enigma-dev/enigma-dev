/** Copyright (C) 2018 Robert B. Colton
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

#include "Graphics_Systems/General/GSvertex_impl.h"

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer) {}

}

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {}

void vertex_color(int buffer, int color, double alpha) {}

void vertex_submit(int buffer, int primitive, unsigned vertex_start, unsigned vertex_count) {}

}
