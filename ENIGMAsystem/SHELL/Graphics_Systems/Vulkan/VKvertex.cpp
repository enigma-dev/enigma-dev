/** Copyright (C) 2021 Robert Colton
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

#include "VulkanHeaders.h"

#include "Graphics_Systems/General/GSvertex_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSstdraw.h"

#include "Widget_Systems/widgets_mandatory.h" // for show_error

#include <d3dcompiler.h>

#include <map>
using std::map;

using namespace enigma::vlkn;

namespace {

} // namespace anonymous

namespace enigma {


void graphics_delete_vertex_buffer_peer(int buffer) {

}

void graphics_delete_index_buffer_peer(int buffer) {

}

} // namespace enigma

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {

}

void vertex_color(int buffer, int color, double alpha) {

}

void vertex_submit_offset(int buffer, int primitive, unsigned offset, unsigned start, unsigned count) {

}

void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count) {

}

} // namespace enigma_user
