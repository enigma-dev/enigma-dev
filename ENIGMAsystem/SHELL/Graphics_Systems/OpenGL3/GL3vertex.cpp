/** Copyright (C) 2014 Josh Ventura, Harijs Grinbergs
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

#include "GL3profiler.h"
#include "GL3shader.h"
#include "GLSLshader.h"

#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSvertex_impl.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include <map>

using std::map;

#define bind_array_buffer(vbo) if (enigma::bound_vbo != vbo) glBindBuffer( GL_ARRAY_BUFFER, enigma::bound_vbo = vbo );
#define bind_element_buffer(vboi) if (enigma::bound_vboi != vboi) glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, enigma::bound_vboi = vboi );

namespace {

GLenum primitive_types[] = { 0, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

map<int, GLuint> vertexBufferPeers;
map<int, GLuint> indexBufferPeers;

} // anonymous namespace

namespace enigma {

void graphics_delete_vertex_buffer_peer(int buffer) {
  glDeleteBuffers(1, &vertexBufferPeers[buffer]);
  vertexBufferPeers.erase(buffer);
}

void graphics_delete_index_buffer_peer(int buffer) {
  glDeleteBuffers(1, &indexBufferPeers[buffer]);
  indexBufferPeers.erase(buffer);
}

static inline int graphics_find_attribute_location(std::string name, int usageIndex) {
  int location = -1;
  if (usageIndex == 0) {
    location = enigma_user::glsl_get_attribute_location(enigma::bound_shader, name);
  }
  if (usageIndex > 0 || location == -1) {
    location = enigma_user::glsl_get_attribute_location(enigma::bound_shader, name + std::to_string(usageIndex));
  }
  return location;
}

void graphics_prepare_buffer(const int buffer, const bool isIndex) {
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

    if (isIndex) {
      bind_element_buffer(bufferPeer);
    } else {
      bind_array_buffer(bufferPeer);
    }

    const GLvoid *data = isIndex ? (const GLvoid *)&indexBuffers[buffer]->indices[0] : (const GLvoid *)&vertexBuffers[buffer]->vertices[0];
    GLenum usage = frozen ? (dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW) : GL_STREAM_DRAW;
    glBufferData(target, size, data, usage);

    if (isIndex) {
      indexBuffers[buffer]->clearData();
    } else {
      vertexBuffers[buffer]->clearData();
    }
  } else {
    if (isIndex) {
      bind_element_buffer(it->second);
    } else {
      bind_array_buffer(it->second);
    }
  }
}

void graphics_apply_vertex_format(int format, size_t offset) {
  using namespace enigma_user;

  const VertexFormat* vertexFormat = vertexFormats[format];

  //Bind texture
  glsl_uniformi_internal(shaderprograms[bound_shader]->uni_texSampler, 0);

  glsl_attribute_enable_all_internal(false); //Disable all attributes

  bool useTextCoords = false, useColors = false;
  map<int,int> useCount;
  const size_t stride = enigma_user::vertex_format_get_stride_size(format);
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

    if (flag.second == vertex_usage_color) useColors = true;
    if (flag.second == vertex_usage_textcoord) useTextCoords = true;

    // NOTE: This is not what GMS1.4 does, it uses glBindAttribLocation
    // so that in_Color0 is an alias of in_Color which glBindAttribLocation allows
    // GMS1.4 also apparently does this binding before the shaders are linked
    // according to a GMS tech blog:
    // https://www.yoyogames.com/blog/16/shaders-overview-part-2
    // https://www.yoyogames.com/blog/18/shaders-overview-part-4
    std::string name = "";
    int usageIndex = useCount[flag.second]++;
    switch (flag.second) {
      case vertex_usage_position: name = "in_Position"; break;
      case vertex_usage_color: name = "in_Color"; break;
      case vertex_usage_normal: name = "in_Normal"; break;
      case vertex_usage_textcoord: name = "in_TextureCoord"; break;
    }
    if (!name.empty()) {
      int location = graphics_find_attribute_location(name, usageIndex);
      if (location == -1 && flag.second == vertex_usage_color) {
        location = graphics_find_attribute_location("in_Colour", usageIndex);
      }

      if (location != -1) {
        glsl_attribute_enable_internal(location, true);
        glsl_attribute_set_internal(location, elements, type, (type == GL_UNSIGNED_BYTE), stride, offset);
      }
    }

    offset += size * sizeof(enigma::VertexElement);
  }

  glsl_uniformf_internal(shaderprograms[bound_shader]->uni_color,
                         (float)currentcolor[0]/255.0f,
                         (float)currentcolor[1]/255.0f,
                         (float)currentcolor[2]/255.0f,
                         (float)currentcolor[3]/255.0f);

  if (useTextCoords) {
    GLint boundTex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTex);
    glsl_uniformi_internal(shaderprograms[bound_shader]->uni_textureEnable, boundTex != 0);
  } else {
    glsl_uniformi_internal(shaderprograms[bound_shader]->uni_textureEnable, 0);
  }
  glsl_uniformi_internal(shaderprograms[bound_shader]->uni_colorEnable, useColors);
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
  draw_batch_flush(batch_flush_deferred);

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];

  #ifdef DEBUG_MODE
  enigma::GPUProfilerBatch& vbd = enigma::gpuprof.add_drawcall();
  ++vbd.drawcalls;
  #endif

  enigma::graphics_prepare_buffer(buffer, false);
  enigma::graphics_apply_vertex_format(vertexBuffer->format, offset);

	glDrawArrays(primitive_types[primitive], start, count);
}

void index_submit_range(int buffer, int vertex, int primitive, unsigned start, unsigned count) {
  draw_batch_flush(batch_flush_deferred);

  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[vertex];
  const enigma::IndexBuffer* indexBuffer = enigma::indexBuffers[buffer];

  #ifdef DEBUG_MODE
  enigma::GPUProfilerBatch& vbd = enigma::gpuprof.add_drawcall();
  ++vbd.drawcalls;
  #endif

  enigma::graphics_prepare_buffer(vertex, false);
  enigma::graphics_prepare_buffer(buffer, true);
  enigma::graphics_apply_vertex_format(vertexBuffer->format, 0);

  GLenum indexType = GL_UNSIGNED_SHORT;
  if (indexBuffer->type == index_type_uint) {
    indexType = GL_UNSIGNED_INT;
    start *= sizeof(unsigned int);
  } else {
    start *= sizeof(unsigned short);
  }

  glDrawElements(primitive_types[primitive], count, indexType, (GLvoid*)(intptr_t)start);
}

} // namespace enigma_user
