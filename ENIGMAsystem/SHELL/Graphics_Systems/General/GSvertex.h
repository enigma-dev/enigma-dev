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

#ifndef __GSVERTEXH_
#define __GSVERTEXH_

#include "Universal_System/scalar.h"

namespace enigma_user {
	enum {
		vertex_type_float1,   // D3DDECLTYPE_FLOAT1
		vertex_type_float2,   // D3DDECLTYPE_FLOAT2
		vertex_type_float3,   // D3DDECLTYPE_FLOAT3
		vertex_type_float4,   // D3DDECLTYPE_FLOAT4
		vertex_type_colour,   // D3DDECLTYPE_D3DCOLOR
		vertex_type_ubyte4    // D3DDECLTYPE_UBYTE4
	};
	
	enum {
		vertex_usage_position,              // D3DDECLUSAGE_POSITION
		vertex_usage_colour,                // D3DDECLUSAGE_COLOR
		vertex_usage_normal,                // D3DDECLUSAGE_NORMAL
		vertex_usage_textcoord,             // D3DDECLUSAGE_TEXCOORD
		vertex_usage_blendweight,           // D3DDECLUSAGE_BLENDWEIGHT
		vertex_usage_blendindices,          // D3DDECLUSAGE_BLENDINDICES
		vertex_usage_depth,                 // D3DDECLUSAGE_DEPTH 
		vertex_usage_tangent,               // D3DDECLUSAGE_TANGENT
		vertex_usage_binormal,              // D3DDECLUSAGE_BINORMAL
		vertex_usage_fog,                   // D3DDECLUSAGE_FOG
		vertex_usage_sample                 // D3DDECLUSAGE_SAMPLE
	};

    int vertex_create_buffer();
    int vertex_create_buffer_ext(unsigned size);
    void vertex_delete_buffer(int buffer);

    void vertex_begin(int buffer, int format);
	void vertex_end(int buffer);
    void vertex_freeze(int buffer);
	void vertex_submit(int buffer, int primitive);
    void vertex_submit(int buffer, int primitive, int texture);
    void vertex_delete(int buffer);

	void vertex_index(int buffer, unsigned id);
    void vertex_position(int buffer, gs_scalar x, gs_scalar y);
    void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z);
	void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz);
    void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v);
	void vertex_argb(int buffer, double alpha, unsigned char r, unsigned char g, unsigned char b);
	void vertex_colour(int buffer, int color, double alpha);
    void vertex_float1(int buffer, float f1);
    void vertex_float2(int buffer, float f1, float f2);
    void vertex_float3(int buffer, float f1, float f2, float f3);
    void vertex_float4(int buffer, float f1, float f2, float f3, float f4);
    void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4);

    void vertex_format_begin();
    void vertex_format_add_colour();
    void vertex_format_add_position();
    void vertex_format_add_position_3d();
    void vertex_format_add_textcoord();
    void vertex_format_add_normal();
    void vertex_format_add_custom(int type, int usage);
    int vertex_format_end();
}

#endif
