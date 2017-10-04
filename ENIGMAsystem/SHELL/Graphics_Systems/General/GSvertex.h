/** Copyright (C) 2015 Harijs Grinbergs
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

#include "../General/OpenGLHeaders.h"
#include <vector>
#include <utility>
using std::vector;
using std::pair;

#include "Universal_System/scalar.h"

namespace enigma {
  struct VertexFormat {
    vector<pair<int,int> > flags;

    void AddAttribute(int type, int attribute) {
      flags.push_back(std::make_pair(type, attribute));
    }
  };
}

namespace enigma_user {
  enum {
    vertex_type_float1, // D3DDECLTYPE_FLOAT1
    vertex_type_float2, // D3DDECLTYPE_FLOAT2
    vertex_type_float3, // D3DDECLTYPE_FLOAT3
    vertex_type_float4, // D3DDECLTYPE_FLOAT4
    vertex_type_colour, // D3DDECLTYPE_D3DCOLOR
    vertex_type_ubyte4  // D3DDECLTYPE_UBYTE4
  };

  enum {
    vertex_usage_position,        // D3DDECLUSAGE_POSITION
    vertex_usage_colour,          // D3DDECLUSAGE_COLOR
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

  unsigned vertex_format_create();
  void vertex_format_destroy(int id);
  bool vertex_format_exists(int id);
  void vertex_format_add(int id, int type, int attribute);
}

#endif //ENIGMA_GSVERTEX_H
