/** Copyright (C) 2015 Harijs Grinbergs
*** Copyright (C) 2018 Robert B. Colton
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

#ifndef ENIGMA_GSVERTEX_H
#define ENIGMA_GSVERTEX_H

#include "Universal_System/scalar.h"
#include "Universal_System/dynamic_args.h" // for vertex_data

namespace enigma_user {

enum {
  vertex_type_float1, // D3DDECLTYPE_FLOAT1
  vertex_type_float2, // D3DDECLTYPE_FLOAT2
  vertex_type_float3, // D3DDECLTYPE_FLOAT3
  vertex_type_float4, // D3DDECLTYPE_FLOAT4
  vertex_type_color,  // D3DDECLTYPE_D3DCOLOR
  vertex_type_ubyte4  // D3DDECLTYPE_UBYTE4
};

#define vertex_type_colour vertex_type_color

enum {
  vertex_usage_position,        // D3DDECLUSAGE_POSITION
  vertex_usage_color,           // D3DDECLUSAGE_COLOR
  vertex_usage_normal,          // D3DDECLUSAGE_NORMAL
  vertex_usage_textcoord,       // D3DDECLUSAGE_TEXCOORD
  vertex_usage_blendweight,     // D3DDECLUSAGE_BLENDWEIGHT
  vertex_usage_blendindices,    // D3DDECLUSAGE_BLENDINDICES
  vertex_usage_depth,           // D3DDECLUSAGE_DEPTH
  vertex_usage_tangent,         // D3DDECLUSAGE_TANGENT
  vertex_usage_binormal,        // D3DDECLUSAGE_BINORMAL
  vertex_usage_fog,             // D3DDECLUSAGE_FOG
  vertex_usage_sample           // D3DDECLUSAGE_SAMPLE
};

#define vertex_usage_colour vertex_usage_color

void vertex_format_begin();
int vertex_format_end();
bool vertex_format_exists(int id);
unsigned vertex_format_get_hash(int id);
unsigned vertex_format_get_stride(int id);
unsigned vertex_format_get_stride_size(int id);
unsigned vertex_format_get_hash();
unsigned vertex_format_get_stride();
unsigned vertex_format_get_stride_size();
void vertex_format_add_position();
void vertex_format_add_position_3d();
void vertex_format_add_color();
#define vertex_format_add_colour vertex_format_add_color
void vertex_format_add_textcoord();
void vertex_format_add_normal();
void vertex_format_add_custom(int type, int usage);

int vertex_create_buffer();
int vertex_create_buffer_ext(unsigned size);
void vertex_delete_buffer(int buffer);
bool vertex_exists(int buffer);
void vertex_set_format(int buffer, int format);
unsigned vertex_get_buffer_size(int buffer);
unsigned vertex_get_number(int buffer);
void vertex_freeze(int buffer, bool dynamic = false);
void vertex_clear(int buffer);
void vertex_begin(int buffer, int format = -1);
void vertex_end(int buffer);
void vertex_data(int buffer, const enigma::varargs& data);
void vertex_position(int buffer, gs_scalar x, gs_scalar y);
void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z);
void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz);
void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v);
void vertex_argb(int buffer, unsigned argb);
void vertex_color(int buffer, int color, double alpha);
#define vertex_colour vertex_color
void vertex_float1(int buffer, float f1);
void vertex_float2(int buffer, float f1, float f2);
void vertex_float3(int buffer, float f1, float f2, float f3);
void vertex_float4(int buffer, float f1, float f2, float f3, float f4);
void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);
void vertex_submit(int buffer, int primitive);
void vertex_submit(int buffer, int primitive, int texture);
void vertex_submit_range(int buffer, int primitive, unsigned start, unsigned count);
void vertex_submit_range(int buffer, int primitive, int texture, unsigned start, unsigned count);
void vertex_submit_offset(int buffer, int primitive, unsigned offset, unsigned start, unsigned count);
void vertex_submit_offset(int buffer, int primitive, int texture, unsigned offset, unsigned start, unsigned count);

enum {
  index_type_ushort,
  index_type_uint
};

int index_create_buffer();
int index_create_buffer_ext(unsigned size);
void index_delete_buffer(int buffer);
bool index_exists(int buffer);
unsigned index_get_buffer_size(int buffer);
unsigned index_get_number(int buffer);
void index_freeze(int buffer, bool dynamic = false);
void index_clear(int buffer);
void index_begin(int buffer, int type);
void index_end(int buffer);
void index_data(int buffer, const enigma::varargs& data);
void index_submit(int buffer, int vertex, int primitive);
void index_submit(int buffer, int vertex, int primitive, int texture);
void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count);
void index_submit_range(int buffer, int vertex, int primitive, int texture, unsigned start, unsigned count);

}

#endif //ENIGMA_GSVERTEX_H
