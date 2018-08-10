/** Copyright (C) 2013, 2018 Robert B. Colton
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

// This file declares methods for creating generic vertex and index
// buffer representations for the backend equivalents (e.g, OpenGL).
// Systems which would like to offer support for ENIGMA's vertex
// functions and the rendering functions which utilize them need
// only to implement the following methods:
// void vertex_argb(int buffer, unsigned argb);
// void vertex_color(int buffer, int color, double alpha);
// void vertex_submit(int buffer, int primitive, unsigned start, unsigned count);
// void index_submit(int buffer, int vertex, int primitive, unsigned start, unsigned count);

#include "GSvertex_impl.h"
#include "GSprimitives.h"
#include "GStextures.h"

#include "Widget_Systems/widgets_mandatory.h"

#include <unordered_map>

#define RESOURCE_EXISTS(id, container) return (id >= 0 && (unsigned)id < enigma::container.size() && enigma::container[id] != nullptr);

namespace enigma {

vector<VertexFormat*> vertexFormats;
vector<VertexBuffer*> vertexBuffers;
vector<IndexBuffer*> indexBuffers;

std::unordered_map<size_t, int> vertexFormatCache;

VertexFormat* vertexFormat = 0;

}

namespace enigma_user {

void vertex_format_begin() {
  enigma::vertexFormat = new enigma::VertexFormat();
}

unsigned vertex_format_get_hash() {
  return enigma::vertexFormat->hash;
}

unsigned vertex_format_get_stride() {
  return enigma::vertexFormat->stride;
}

void vertex_format_add_color() {
  enigma::vertexFormat->AddAttribute(vertex_type_color, vertex_usage_color);
}

void vertex_format_add_position() {
  enigma::vertexFormat->AddAttribute(vertex_type_float2, vertex_usage_position);
}

void vertex_format_add_position_3d() {
  enigma::vertexFormat->AddAttribute(vertex_type_float3, vertex_usage_position);
}

void vertex_format_add_textcoord() {
  enigma::vertexFormat->AddAttribute(vertex_type_float2, vertex_usage_textcoord);
}

void vertex_format_add_normal() {
  enigma::vertexFormat->AddAttribute(vertex_type_float3, vertex_usage_normal);
}

void vertex_format_add_custom(int type, int usage) {
  enigma::vertexFormat->AddAttribute(type, usage);
}

int vertex_format_end() {
  int id = -1;
  auto search = enigma::vertexFormatCache.find(enigma::vertexFormat->hash);
  if (search != enigma::vertexFormatCache.end()) {
    id = search->second;
    delete enigma::vertexFormat;
  } else {
    id = enigma::vertexFormats.size();
    enigma::vertexFormats.push_back(enigma::vertexFormat);
    enigma::vertexFormatCache[enigma::vertexFormat->hash] = id;
  }
  enigma::vertexFormat = 0;
  return id;
}

bool vertex_format_exists() {
  return (enigma::vertexFormat != 0);
}

bool vertex_format_exists(int id) {
  RESOURCE_EXISTS(id, vertexFormats);
}

unsigned vertex_format_get_stride(int id) {
  return enigma::vertexFormats[id]->stride;
}

unsigned vertex_format_get_hash(int id) {
  return enigma::vertexFormats[id]->hash;
}

int vertex_create_buffer() {
  int id = enigma::vertexBuffers.size();
  enigma::vertexBuffers.push_back(new enigma::VertexBuffer());
  return id;
}

int vertex_create_buffer_ext(unsigned size) {
  int id = enigma::vertexBuffers.size();
  enigma::vertexBuffers.push_back(new enigma::VertexBuffer());
  return id;
}

void vertex_delete_buffer(int buffer) {
  enigma::graphics_delete_vertex_buffer_peer(buffer);
  delete enigma::vertexBuffers[buffer];
  enigma::vertexBuffers[buffer] = nullptr;
}

bool vertex_exists(int buffer) {
  RESOURCE_EXISTS(buffer, vertexBuffers);
}

void vertex_set_format(int buffer, int format) {
  enigma::vertexBuffers[buffer]->format = format;
}

unsigned vertex_get_size(int buffer) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  return vertexBuffer->getNumber() * sizeof(gs_scalar);
}

unsigned vertex_get_number(int buffer) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  if (vertex_format_exists(vertexBuffer->format)) {
    const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];

    return vertexBuffer->getNumber() / vertexFormat->stride;
  }

  return 0;
}

void vertex_freeze(int buffer) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  // we can freeze the vertex buffer only if it isn't already frozen
  // if it's not frozen, then we'll freeze it when we do a dirty update
  if (vertexBuffer->frozen) return;
  vertexBuffer->frozen = true;
  vertexBuffer->dirty = true;
}

void vertex_clear(int buffer) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  // clear it just for good measure, even though it's probably already empty
  // since we do that just after uploading it to the GPU "peer"
  vertexBuffer->vertices.clear();
  vertexBuffer->number = 0;

  // we can clear a.k.a. "unfreeze" the vertex buffer only if it is actually frozen
  if (!vertexBuffer->frozen) return;
  vertexBuffer->frozen = false;
  vertexBuffer->dirty = true;
}

void vertex_begin(int buffer, int format) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  vertexBuffer->vertices.clear();
  vertexBuffer->format = format;

  // we can only flag the vertex buffer contents as dirty and needing an update
  // if the vertex buffer hasn't been frozen, otherwise we just ignore it
  if (vertexBuffer->frozen) return;
  vertexBuffer->dirty = true;
}

void vertex_end(int buffer) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  if (vertexBuffer->frozen) return;
  vertexBuffer->number = vertexBuffer->vertices.size();
}

void vertex_data(int buffer, const enigma::varargs& data) {
  enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  #ifdef DEBUG_MODE
  if (!vertex_format_exists(vertexBuffer->format)) {
    show_error("Vertex format " + enigma_user::toString(vertexBuffer->format) +
               " does not exist and is required for vertex_data to decode varargs", false);
    return;
  }
  #endif
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];
  int attrIndex = 0;
  for (int i = 0; i < data.argc;) {
    attrIndex = attrIndex % vertexFormat->flags.size();
    std::pair<int,int> attr = vertexFormat->flags[attrIndex++];
    switch (attr.first) {
      case vertex_type_float1:
        vertex_float1(buffer, data.get(i++));
        continue;
      case vertex_type_float2:
        vertex_float2(buffer, data.get(i), data.get(i + 1));
        i += 2;
        continue;
      case vertex_type_float3:
        vertex_float3(buffer, data.get(i), data.get(i + 1), data.get(i + 2));
        i += 3;
        continue;
      case vertex_type_float4:
        vertex_float4(buffer, data.get(i), data.get(i + 1), data.get(i + 2), data.get(i + 3));
        i += 4;
        continue;
      case vertex_type_color:
        vertex_argb(buffer, data.get(i++));
        continue;
      case vertex_type_ubyte4:
        vertex_ubyte4(buffer, data.get(i), data.get(i + 1), data.get(i + 2), data.get(i + 3));
        i += 4;
        continue;
    }

    show_error("Vertex format " + enigma_user::toString(vertexBuffer->format) +
               " contains attribute with unknown type " + enigma_user::toString(attr.first), false);
    break;
  }
}

void vertex_position(int buffer, gs_scalar x, gs_scalar y) {
  enigma::vertexBuffers[buffer]->vertices.push_back(x);
  enigma::vertexBuffers[buffer]->vertices.push_back(y);
}

void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z) {
  enigma::vertexBuffers[buffer]->vertices.push_back(x);
  enigma::vertexBuffers[buffer]->vertices.push_back(y);
  enigma::vertexBuffers[buffer]->vertices.push_back(z);
}

void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz) {
  enigma::vertexBuffers[buffer]->vertices.push_back(nx);
  enigma::vertexBuffers[buffer]->vertices.push_back(ny);
  enigma::vertexBuffers[buffer]->vertices.push_back(nz);
}

void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v) {
  enigma::vertexBuffers[buffer]->vertices.push_back(u);
  enigma::vertexBuffers[buffer]->vertices.push_back(v);
}

void vertex_float1(int buffer, float f1) {
  enigma::vertexBuffers[buffer]->vertices.push_back(f1);
}

void vertex_float2(int buffer, float f1, float f2) {
  enigma::vertexBuffers[buffer]->vertices.push_back(f1);
  enigma::vertexBuffers[buffer]->vertices.push_back(f2);
}

void vertex_float3(int buffer, float f1, float f2, float f3) {
  enigma::vertexBuffers[buffer]->vertices.push_back(f1);
  enigma::vertexBuffers[buffer]->vertices.push_back(f2);
  enigma::vertexBuffers[buffer]->vertices.push_back(f3);
}

void vertex_float4(int buffer, float f1, float f2, float f3, float f4) {
  enigma::vertexBuffers[buffer]->vertices.push_back(f1);
  enigma::vertexBuffers[buffer]->vertices.push_back(f2);
  enigma::vertexBuffers[buffer]->vertices.push_back(f3);
  enigma::vertexBuffers[buffer]->vertices.push_back(f4);
}

void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4) {
  unsigned val = (u1 << 24) | (u2 << 16) | (u3 << 8) | u4;
  enigma::vertexBuffers[buffer]->vertices.push_back(val);
}

void vertex_submit(int buffer, int primitive) {
  vertex_submit(buffer, primitive, 0, vertex_get_number(buffer));
}

void vertex_submit(int buffer, int primitive, int texture) {
  texture_set(texture);
  vertex_submit(buffer, primitive);
}

void vertex_submit(int buffer, int primitive, int texture, unsigned start, unsigned count) {
  texture_set(texture);
  vertex_submit(buffer, primitive, start, count);
}

int index_create_buffer() {
  int id = enigma::indexBuffers.size();
  enigma::indexBuffers.push_back(new enigma::IndexBuffer());
  return id;
}

int index_create_buffer_ext(unsigned size) {
  int id = enigma::indexBuffers.size();
  enigma::indexBuffers.push_back(new enigma::IndexBuffer());
  return id;
}

void index_delete_buffer(int buffer) {
  enigma::graphics_delete_index_buffer_peer(buffer);
  delete enigma::indexBuffers[buffer];
  enigma::indexBuffers[buffer] = nullptr;
}

bool index_exists(int buffer) {
  RESOURCE_EXISTS(buffer, indexBuffers);
}

unsigned index_get_size(int buffer) {
  return enigma::indexBuffers[buffer]->getNumber() * sizeof(uint16_t);
}

unsigned index_get_number(int buffer) {
  return enigma::indexBuffers[buffer]->getNumber();
}

void index_freeze(int buffer) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  // we can freeze the index buffer only if it isn't already frozen
  // if it's not frozen, then we'll freeze it when we do a dirty update
  if (indexBuffer->frozen) return;
  indexBuffer->frozen = true;
  indexBuffer->dirty = true;
}

void index_clear(int buffer) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  // clear it just for good measure, even though it's probably already empty
  // since we do that just after uploading it to the GPU "peer"
  indexBuffer->indices.clear();
  indexBuffer->number = 0;

  // we can clear a.k.a. "unfreeze" the index buffer only if it is actually frozen
  if (!indexBuffer->frozen) return;
  indexBuffer->frozen = false;
  indexBuffer->dirty = true;
}

void index_begin(int buffer, int type) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  indexBuffer->indices.clear();
  indexBuffer->type = type;

  // we can only flag the index buffer contents as dirty and needing an update
  // if the index buffer hasn't been frozen, otherwise we just ignore it
  if (indexBuffer->frozen) return;
  indexBuffer->dirty = true;
}

void index_end(int buffer) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  if (indexBuffer->frozen) return;
  indexBuffer->number = indexBuffer->indices.size();
  if (indexBuffer->type == index_type_uint)
    indexBuffer->number /= 2;
}

void index_data(int buffer, const enigma::varargs& data) {
  enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];
  for (int i = 0; i < data.argc; i++) {
    if (indexBuffer->type == index_type_uint) {
      uint32_t ind = data.get(i);
      indexBuffer->indices.push_back(ind);
      indexBuffer->indices.push_back(ind << 16);
    } else {
      indexBuffer->indices.push_back((uint16_t)data.get(i));
    }
  }
}

void index_submit(int buffer, int vertex, int primitive) {
  index_submit(buffer, vertex, primitive, 0, index_get_number(buffer));
}

void index_submit(int buffer, int vertex, int primitive, int texture) {
  texture_set(texture);
  index_submit(buffer, vertex, primitive);
}

void index_submit(int buffer, int vertex, int primitive, int texture, unsigned start, unsigned count) {
  texture_set(texture);
  index_submit(buffer, vertex, primitive, start, count);
}

}
