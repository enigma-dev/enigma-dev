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

#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <stdint.h>

using std::vector;
using std::pair;

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer);
void graphics_delete_index_buffer_peer(int buffer);

template <class T>
inline void hash_combine(std::size_t& seed, const T& v) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct VertexFormat {
  vector<pair<int,int> > flags; // order of elements for each vertex in insertion order
  std::size_t stride; // number of elements each vertex is comprised of, not in bytes
  std::size_t stride_size; // size of the stride (aka vertex) in bytes
  std::size_t hash; // hash that uniquely identifies this vertex format

  // NOTE: flags should only be mutated using AddAttribute so the hash is correct!
  // NOTE: stride is not in number of bytes because each backend uses the native size of the type
  // NOTE: hash is cached for performance reasons

  VertexFormat(): stride(0), stride_size(0), hash(0) {}

  void Clear() {
    hash = stride = stride_size = 0;
    flags.clear();
  }

  void AddAttribute(int type, int attribute) {
    using namespace enigma_user;

    hash_combine(hash, type);
    hash_combine(hash, attribute);

    switch (type) {
      case vertex_type_float1: stride += 1; stride_size += 1 * sizeof(float); break;
      case vertex_type_float2: stride += 2; stride_size += 2 * sizeof(float); break;
      case vertex_type_float3: stride += 3; stride_size += 3 * sizeof(float); break;
      case vertex_type_float4: stride += 4; stride_size += 4 * sizeof(float); break;
      case vertex_type_color: stride += 1; stride_size += 4 * sizeof(unsigned char); break;
      case vertex_type_ubyte4: stride += 1; stride_size += 4 * sizeof(unsigned char); break;
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
  vector<VertexElement> vertices; // interleaved vertex elements
  bool frozen; // whether vertex_freeze has been called
  bool dynamic; // if the user wants to update the buffer infrequently
  bool dirty; // whether the user has begun specifying new vertex data
  int format; // index of the vertex format describing this buffer
  std::size_t number; // cached size of vertices

  // NOTE: dynamic does not mean updating the buffer every frame!
  // NOTE: format may not exist when this buffer is first created
  // NOTE: number is only intended to be accessed with getNumber()!

  VertexBuffer(): frozen(false), dynamic(false), dirty(false), format(-1), number(0) {}

  // returns the number of vertex elements in the buffer
  int getNumber() const {
    return dirty ? vertices.size() : number;
  }

  // intuitively clears the vertex data on the CPU side
  // intended to be called by the backend so that static
  // buffers shrink all CPU resources and stream buffers
  // only clear them leaving the reserved capacity
  // for future primitives to be specified
  void clearData() {
    if (frozen) {
      // this will give us 0 size and 0 capacity
      std::vector<enigma::VertexElement>().swap(vertices);
    } else {
      // this will give us 0 size but keep capacity
      vertices.clear();
    }
    dirty = false; // we aren't dirty anymore
  }
};

struct IndexBuffer {
  vector<uint16_t> indices; // index data of this buffer
  bool frozen; // whether index_freeze has been called
  bool dynamic; // if the user wants to update the buffer infrequently
  bool dirty; // whether the user has begun specifying new index data
  int type; // how the indices in this buffer are to be interpreted
  std::size_t number; // cached size of indices

  // NOTE: dynamic does not mean updating the buffer every frame!
  // NOTE: some types are not available on certain backends
  // NOTE: number is only intended to be accessed with getNumber()!

  IndexBuffer(): frozen(false), dynamic(false), dirty(false), type(-1), number(0) {}

  // returns the number of index elements in the buffer
  int getNumber() const {
    return dirty ? indices.size() : number;
  }

  // intuitively clears the index data on the CPU side
  // intended to be called by the backend so that static
  // buffers shrink all CPU resources and stream buffers
  // only clear them leaving the reserved capacity
  // for future primitives to be specified
  void clearData() {
    if (frozen) {
      // this will give us 0 size and 0 capacity
      std::vector<uint16_t>().swap(indices);
    } else {
      // this will give us 0 size but keep capacity
      indices.clear();
    }
    dirty = false; // we aren't dirty anymore
  }
};

extern vector<std::unique_ptr<VertexFormat>> vertexFormats;
extern vector<std::unique_ptr<VertexBuffer>> vertexBuffers;
extern vector<std::unique_ptr<IndexBuffer>> indexBuffers;

}

#endif
