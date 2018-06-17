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
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Graphics_Systems/General/OpenGLHeaders.h"

#include <map>
using std::map;

namespace {

GLenum primitive_types[] = { 0, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

map<int, GLuint> vertexBufferPeers;
map<int, GLuint> indexBufferPeers;

}

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer) {
  glDeleteBuffers(1, &vertexBufferPeers[buffer]);
  vertexBufferPeers.erase(buffer);
}

void graphics_delete_index_buffer_peer(int buffer) {
  glDeleteBuffers(1, &indexBufferPeers[buffer]);
  indexBufferPeers.erase(buffer);
}

void graphics_prepare_buffer(const int buffer, const bool isIndex) {
  const bool dirty = isIndex ? indexBuffers[buffer]->dirty : vertexBuffers[buffer]->dirty;
  const bool frozen = isIndex ? indexBuffers[buffer]->frozen : vertexBuffers[buffer]->frozen;
  GLuint bufferPeer;
  auto it = isIndex ? indexBufferPeers.find(buffer) : vertexBufferPeers.find(buffer);
  const GLenum target = isIndex ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;

  // if the contents of the buffer are dirty then we need to update
  // our native buffer object "peer"
  if (dirty) {
    size_t size = isIndex ? enigma_user::index_get_size(buffer) : enigma_user::vertex_get_size(buffer);

    // if we haven't created a native "peer" for this buffer yet,
    // then we need to do so now
    if (it == (isIndex ? indexBufferPeers.end() : vertexBufferPeers.end())) {
      glGenBuffers(1, &bufferPeer);
      if (isIndex) {
        indexBufferPeers[buffer] = bufferPeer;
      } else {
        vertexBufferPeers[buffer] = bufferPeer;
      }
    } else {
      bufferPeer = it->second;
    }

    glBindBuffer(target, bufferPeer);
    GLint pSize;
    glGetBufferParameteriv(target, GL_BUFFER_SIZE, &pSize);

    // if the size of the peer isn't big enough to hold the new contents
    // or freeze was called, then we need to make a call to glBufferData
    // to allocate a bigger peer or remove the GL_DYNAMIC_DRAW usage
    const GLvoid *data = isIndex ? (const GLvoid *)&indexBuffers[buffer]->indices[0] : (const GLvoid *)&vertexBuffers[buffer]->vertices[0];
    if (size > (size_t)pSize || frozen) {
      GLenum usage = frozen ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;
      glBufferData(target, size, data, usage);
    } else {
      glBufferSubData(target, 0, size, data);
    }

    if (isIndex) {
      indexBuffers[buffer]->indices.clear();
      indexBuffers[buffer]->dirty = false;
    } else {
      vertexBuffers[buffer]->vertices.clear();
      vertexBuffers[buffer]->dirty = false;
    }
  } else {
    glBindBuffer(target, it->second);
  }
}

void graphics_apply_vertex_format(int format) {
  using namespace enigma_user;

  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[format];

  bool useVertices = false, useNormals = false, useColors = false, useFogCoords = false;
  size_t offset = 0, texture = 0;
  const size_t stride = vertexFormat->stride * sizeof(float);
  for (size_t i = 0; i < vertexFormat->flags.size(); ++i) {
    const pair<int, int> flag = vertexFormat->flags[i];

    size_t elements = 0, size = 0;
    GLenum type = GL_FLOAT;
    switch (flag.first) {
      case vertex_type_float1: elements = 1; size = 1; break;
      case vertex_type_float2: elements = 2; size = 2; break;
      case vertex_type_float3: elements = 3; size = 3; break;
      case vertex_type_float4: elements = 4; size = 4; break;
      case vertex_type_color: elements = 4; size = 1; type = GL_UNSIGNED_BYTE; break;
      case vertex_type_ubyte4: elements = 4; size = 1; type = GL_UNSIGNED_BYTE; break;
    }

    #define GL_ATTRIB_OFFSET(P) ((const GLvoid *) (sizeof(gs_scalar) * P))

    // this is an "emulation" of vertex format declarations for the OpenGL fixed-function pipeline
    switch (flag.second) {
      case vertex_usage_position:
        if (useVertices) break;
        useVertices = true;
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_color:
        if (useColors) break;
        useColors = true;
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_normal:
        if (useNormals) break;
        useNormals = true;
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_textcoord:
        if (texture >= GL_MAX_TEXTURE_UNITS) break;
        glClientActiveTexture(GL_TEXTURE0 + texture++);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_blendweight: break;
      case vertex_usage_blendindices: break;
      case vertex_usage_depth: break;
      case vertex_usage_tangent: break;
      case vertex_usage_binormal: break;
      case vertex_usage_fog:
        if (useFogCoords) break;
        useFogCoords = true;
        glEnableClientState(GL_FOG_COORD_ARRAY);
        glFogCoordPointer(type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_sample: break;
    }

    offset += size;
  }

  if (!useVertices) glDisableClientState(GL_VERTEX_ARRAY);
  if (!useNormals) glDisableClientState(GL_NORMAL_ARRAY);
  if (!useColors) glDisableClientState(GL_COLOR_ARRAY);
  if (!useFogCoords) glDisableClientState(GL_FOG_COORD_ARRAY);
  if (texture < GL_MAX_TEXTURE_UNITS) {
    for (size_t i = texture; i < GL_MAX_TEXTURE_UNITS; ++i) {
      glClientActiveTexture(GL_TEXTURE0 + i);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }
}

}

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {
  enigma::color_t finalcol = (COL_GET_A(argb) << 24) | (COL_GET_R(argb) << 16) | (COL_GET_G(argb) << 8) | COL_GET_B(argb);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_color(int buffer, int color, double alpha) {
  enigma::color_t finalcol = color + (CLAMP_ALPHA(alpha) << 24);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_submit(int buffer, int primitive, unsigned start, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  enigma::graphics_prepare_buffer(buffer, false);
  enigma::graphics_apply_vertex_format(vertexBuffer->format);

	glDrawArrays(primitive_types[primitive], start, count);
}

void index_submit(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];
  const enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  enigma::graphics_prepare_buffer(vertex, false);
  enigma::graphics_prepare_buffer(buffer, true);
  enigma::graphics_apply_vertex_format(vertexBuffer->format);

  GLenum indexType = GL_UNSIGNED_SHORT;
  if (indexBuffer->type == index_type_uint) {
    indexType = GL_UNSIGNED_INT;
    start *= sizeof(unsigned int);
  } else {
    start *= sizeof(unsigned short);
  }

  glDrawElements(primitive_types[primitive], count, indexType, (GLvoid*)(intptr_t)start);
}

}
