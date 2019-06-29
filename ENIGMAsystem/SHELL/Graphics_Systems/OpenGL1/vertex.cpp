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

//NOTE: If this source is changed to utilize additional features of the GL_ARB_vertex_buffer_object
//extension, then please be sure to update the graphics_init_vbo_method() helper below to ensure any
//functions are properly aliased and will continue working on affected graphics cards.

#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSvertex_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSstdraw.h"

#include <map>
using std::map;

namespace {

GLenum primitive_types[] = { 0, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

// for OpenGL1.1
map<int, std::vector<enigma::VertexElement> > vertexBufferArrays;
map<int, std::vector<short unsigned int> > indexBufferArrays;
// for OpenGL1.5 or ARB_vertex_buffer_object support
map<int, GLuint> vertexBufferPeers;
map<int, GLuint> indexBufferPeers;

void* graphics_prepare_buffer_array(const int buffer, const bool isIndex) {
  using namespace enigma;

  const bool dirty = isIndex ? indexBuffers[buffer]->dirty : vertexBuffers[buffer]->dirty;
  if (dirty) {
    if (isIndex) {
      IndexBuffer* indexBuffer = indexBuffers[buffer];
      indexBufferArrays[buffer] = indexBuffer->indices;
      indexBuffer->clearData();
    } else {
      VertexBuffer* vertexBuffer = vertexBuffers[buffer];
      vertexBufferArrays[buffer] = vertexBuffer->vertices;
      vertexBuffer->clearData();
    }
  }
  if (isIndex) {
    return (void*)indexBufferArrays[buffer].data();
  } else {
    return (void*)vertexBufferArrays[buffer].data();
  }
}

void graphics_prepare_buffer_peer(const int buffer, const bool isIndex) {
  using namespace enigma;

  const bool dirty = isIndex ? indexBuffers[buffer]->dirty : vertexBuffers[buffer]->dirty;
  const bool frozen = isIndex ? indexBuffers[buffer]->frozen : vertexBuffers[buffer]->frozen;
  const bool dynamic = isIndex ? indexBuffers[buffer]->dynamic : vertexBuffers[buffer]->dynamic;
  GLuint bufferPeer;
  auto it = isIndex ? indexBufferPeers.find(buffer) : vertexBufferPeers.find(buffer);
  const GLenum target = isIndex ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;

  // if the contents of the buffer are dirty then we need to update
  // our native buffer object "peer"
  if (dirty) {
    size_t size = isIndex ? enigma_user::index_get_buffer_size(buffer) : enigma_user::vertex_get_buffer_size(buffer);

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

    const GLvoid *data = isIndex ? (const GLvoid *)&indexBuffers[buffer]->indices[0] : (const GLvoid *)&vertexBuffers[buffer]->vertices[0];
    GLenum usage = frozen ? (dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) : GL_STREAM_DRAW;
    glBufferData(target, size, data, usage);

    if (isIndex) {
      indexBuffers[buffer]->clearData();
    } else {
      vertexBuffers[buffer]->clearData();
    }
  } else {
    glBindBuffer(target, it->second);
  }
}

} // anonymous namespace

namespace enigma {

bool vbo_is_supported = false;

void graphics_init_vbo_method() {
  // we don't check for extensions until GLEW has been initialized
  vbo_is_supported = GLEW_ARB_vertex_buffer_object;
  // if the vbo extension is supported, but the GL version is old,
  // we need to alias the buffer functions to the extension ones
  if (GLEW_ARB_vertex_buffer_object == true && GLEW_VERSION_1_5 == false) {
    glGenBuffers = glGenBuffersARB;
    glBindBuffer = glBindBufferARB;
    glBufferData = glBufferDataARB;
    glDeleteBuffers = glDeleteBuffersARB;
  }
}

void graphics_delete_vertex_buffer_peer(int buffer) {
  if (vbo_is_supported) {
    glDeleteBuffers(1, &vertexBufferPeers[buffer]);
    vertexBufferPeers.erase(buffer);
  } else {
    vertexBufferArrays.erase(buffer);
  }
}

void graphics_delete_index_buffer_peer(int buffer) {
  if (vbo_is_supported) {
    glDeleteBuffers(1, &indexBufferPeers[buffer]);
    indexBufferPeers.erase(buffer);
  } else {
    indexBufferArrays.erase(buffer);
  }
}

GLvoid* graphics_prepare_buffer(const int buffer, const bool isIndex) {
  if (vbo_is_supported) {
    graphics_prepare_buffer_peer(buffer, isIndex);
    return NULL;
  } else {
    return graphics_prepare_buffer_array(buffer, isIndex);
  }
}

struct ClientState {
  bool useVertices = false, useNormals = false, useColors = false, useFogCoords = false;
  size_t texture = 0;
};

ClientState graphics_apply_vertex_format(int format, const GLvoid* base_pointer) {
  using namespace enigma_user;

  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[format];

  ClientState state;
  size_t offset = 0;
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

    #define GL_ATTRIB_OFFSET(P) ((const GLvoid *) ((intptr_t)base_pointer + (sizeof(gs_scalar) * P)))

    // this is an "emulation" of vertex format declarations for the OpenGL fixed-function pipeline
    switch (flag.second) {
      case vertex_usage_position:
        if (state.useVertices) break;
        state.useVertices = true;
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_color:
        if (state.useColors) break;
        state.useColors = true;
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_normal:
        if (state.useNormals) break;
        state.useNormals = true;
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_textcoord:
        if (state.texture >= GL_MAX_TEXTURE_UNITS) break;
        glClientActiveTexture(GL_TEXTURE0 + state.texture++);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(elements, type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_blendweight: break;
      case vertex_usage_blendindices: break;
      case vertex_usage_depth: break;
      case vertex_usage_tangent: break;
      case vertex_usage_binormal: break;
      case vertex_usage_fog:
        if (state.useFogCoords) break;
        state.useFogCoords = true;
        glEnableClientState(GL_FOG_COORD_ARRAY);
        glFogCoordPointer(type, stride, GL_ATTRIB_OFFSET(offset));
        break;
      case vertex_usage_sample: break;
    }

    offset += size;
  }

  return state;
}

void graphics_reset_client_state(const ClientState &state) {
  if (state.useVertices) glDisableClientState(GL_VERTEX_ARRAY);
  if (state.useNormals) glDisableClientState(GL_NORMAL_ARRAY);
  if (state.useColors) glDisableClientState(GL_COLOR_ARRAY);
  if (state.useFogCoords) glDisableClientState(GL_FOG_COORD_ARRAY);
  if (state.texture > 0) {
    for (size_t i = 0; i < state.texture; ++i) {
      glClientActiveTexture(GL_TEXTURE0 + i);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }
  if (vbo_is_supported) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
}

} // namespace enigma

namespace enigma_user {

void vertex_argb(int buffer, unsigned argb) {
  enigma::color_t finalcol = (COL_GET_A(argb) << 24) | (COL_GET_R(argb) << 16) | (COL_GET_G(argb) << 8) | COL_GET_B(argb);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_color(int buffer, int color, double alpha) {
  enigma::color_t finalcol = color + (CLAMP_ALPHA(alpha) << 24);
  enigma::vertexBuffers[buffer]->vertices.push_back(finalcol);
}

void vertex_submit_offset(int buffer, int primitive, unsigned offset, unsigned start, unsigned count) {
  draw_state_flush();

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  void* base_pointer = enigma::graphics_prepare_buffer(buffer, false);
  enigma::ClientState state = enigma::graphics_apply_vertex_format(vertexBuffer->format, (GLvoid*)((intptr_t)base_pointer + offset));

  glDrawArrays(primitive_types[primitive], start, count);

  enigma::graphics_reset_client_state(state);
}

void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  draw_state_flush();

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];
  const enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  void* base_vertex_pointer = enigma::graphics_prepare_buffer(vertex, false);
  void* base_index_pointer = enigma::graphics_prepare_buffer(buffer, true);
  enigma::ClientState state = enigma::graphics_apply_vertex_format(vertexBuffer->format, base_vertex_pointer);

  GLenum indexType = GL_UNSIGNED_SHORT;
  if (indexBuffer->type == index_type_uint) {
    indexType = GL_UNSIGNED_INT;
    start *= sizeof(unsigned int);
  } else {
    start *= sizeof(unsigned short);
  }

  glDrawElements(primitive_types[primitive], count, indexType, (GLvoid*)((intptr_t)base_index_pointer + start));

  enigma::graphics_reset_client_state(state);
  if (enigma::vbo_is_supported)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

} // namespace enigma_user
