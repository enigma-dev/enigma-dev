/** Copyright (C) 2014 Josh Ventura
*** Copyright (C) 2015 Harijs Grinbergs
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

#ifdef INCLUDED_FROM_SHELLMAIN
#  error This file includes non-ENIGMA STL headers and should not be included from SHELLmain.
#endif

#ifndef ENIGMA_GSVERTEX_IMPL_H
#define ENIGMA_GSVERTEX_IMPL_H

#include "GSvertex.h"

#include <vector>
#include <utility>
#include <stdint.h>
using std::vector;
using std::pair;

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer);
void graphics_delete_index_buffer_peer(int buffer);

struct VertexFormat {
  vector<pair<int,int> > flags;
  std::size_t stride;

  VertexFormat(): stride(0) {}

  void AddAttribute(int type, int attribute) {
    using namespace enigma_user;

    switch (type) {
      case vertex_type_float1: stride += 1; break;
      case vertex_type_float2: stride += 2; break;
      case vertex_type_float3: stride += 3; break;
      case vertex_type_float4: stride += 4; break;
      case vertex_type_color: stride += 1; break;
      case vertex_type_ubyte4: stride += 1; break;
    }
    flags.push_back(std::make_pair(type, attribute));
  }
};

template<int x> struct intmatch { };
template<int x> struct uintmatch { };
template<> struct intmatch<1>   { typedef int8_t type;  };
template<> struct intmatch<2>   { typedef int16_t type; };
template<> struct intmatch<4>   { typedef int32_t type; };
template<> struct intmatch<8>   { typedef int64_t type; };
template<> struct uintmatch<1>  { typedef uint8_t type;  };
template<> struct uintmatch<2>  { typedef uint16_t type; };
template<> struct uintmatch<4>  { typedef uint32_t type; };
template<> struct uintmatch<8>  { typedef uint64_t type; };
typedef uintmatch<sizeof(gs_scalar)>::type color_t;
union VertexElement {
  color_t d;
  gs_scalar f;

  VertexElement(gs_scalar v): f(v) {}
  VertexElement(color_t v): d(v) {}
};

struct VertexBuffer {
  vector<VertexElement> vertices;
  bool frozen, dirty;
  int format;
  std::size_t number;

  VertexBuffer(): frozen(false), dirty(false), format(-1), number(0) {}
};

struct IndexBuffer {
  vector<uint16_t> indices;
  bool frozen, dirty;
  int type;
  std::size_t number;

  IndexBuffer(): frozen(false), dirty(false), type(-1), number(0) {}
};

extern vector<VertexFormat*> vertexFormats;
extern vector<VertexBuffer*> vertexBuffers;
extern vector<IndexBuffer*> indexBuffers;

}

#endif
