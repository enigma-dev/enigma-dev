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

#include "../General/OpenGLHeaders.h"
#include "../General/GSvertex.h"

// GL1 cannot have this stuff!!! Use GL3! Maybe an error here?
namespace enigma_user {
  unsigned vertex_format_create() {
    return (unsigned) -1;
  }

  void vertex_format_destroy(int id) {}

  bool vertex_format_exists(int id) {
    return false;
  }

  unsigned vertex_create_buffer() {
    return (unsigned) -1;
  }

  unsigned vertex_create_buffer_ext(unsigned size) {
    (void) size;
    return (unsigned) -1;
  }

  void vertex_delete_buffer(int buffer) {}

  void vertex_begin(int buffer, int format) {}

  void vertex_end(int buffer) {}

  void vertex_freeze(int buffer) {}

  void vertex_submit(int buffer, int primitive) {}

  void vertex_submit(int buffer, int primitive, int texture) {}

  void vertex_delete(int buffer) {}

  void vertex_index(int buffer, unsigned id) {}

  void vertex_position(int buffer, gs_scalar x, gs_scalar y) {}

  void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z) {}

  void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz) {}

  void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v) {}

  void vertex_argb(int buffer, double alpha, unsigned char r, unsigned char g, unsigned char b) {}

  void vertex_colour(int buffer, int color, double alpha) {}

  void vertex_float1(int buffer, float f1) {}

  void vertex_float2(int buffer, float f1, float f2) {}

  void vertex_float3(int buffer, float f1, float f2, float f3) {}

  void vertex_float4(int buffer, float f1, float f2, float f3, float f4) {}

  void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4) {}

  void vertex_format_begin() {}

  void vertex_format_add(int id, int type, int attribute) {}

  void vertex_format_add_colour() {}

  void vertex_format_add_position() {}

  void vertex_format_add_position_3d() {} 

  void vertex_format_add_textcoord() {}

  void vertex_format_add_normal() {}

  void vertex_format_add_custom(int type, int usage) {}

  unsigned vertex_format_end() {
    return (unsigned) -1;
  }

}
