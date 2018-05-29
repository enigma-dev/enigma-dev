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

#include "Graphics_Systems/General/GSvertex.h"

#include "Graphics_Systems/General/OpenGLHeaders.h"

#include <map>
using std::map;

namespace enigma {

GLenum primitive_types[] = { 0, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN };

map<int, GLuint> peers;

void graphics_create_vertex_buffer_peer(int buffer) {
  GLuint vertexBufferPeer;
  glGenBuffers(1, &vertexBufferPeer);
  peers[buffer] = vertexBufferPeer;
}

void graphics_upload_vertex_buffer_peer(int buffer) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  size_t size = enigma_user::vertex_get_size(buffer);

  glBindBuffer(GL_ARRAY_BUFFER, peers[buffer]);
  glBufferData(GL_ARRAY_BUFFER, size, &vertexBuffer->vertices[0], GL_DYNAMIC_DRAW);
}

void graphics_delete_vertex_buffer_peer(int buffer) {
  glDeleteBuffers(1, &peers[buffer]);
  peers.erase(buffer);
}

}

namespace enigma_user {

void vertex_submit(int buffer, int primitive, unsigned vertex_start, unsigned vertex_count) {
  const enigma::VertexBuffer* vertexBuffer = enigma::vertexBuffers[buffer];
  const enigma::VertexFormat* vertexFormat = enigma::vertexFormats[vertexBuffer->format];

  glBindBuffer(GL_ARRAY_BUFFER, enigma::peers[buffer]);

  bool useVertices, useNormals, useColors, useTextCoords, useFogCoords;
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

    #define OFFSET( P )  ( ( const GLvoid * ) ( sizeof( gs_scalar ) * ( P         ) ) )

    switch (flag.second) {
      case vertex_usage_position:
        useVertices = true;
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(elements, type, stride, OFFSET(offset));
        break;
      case vertex_usage_color:
        useColors = true;
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(elements, type, stride, OFFSET(offset));
        break;
      case vertex_usage_normal:
        useNormals = true;
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(type, stride, OFFSET(offset));
        break;
      case vertex_usage_textcoord:
        useTextCoords = true;
        glClientActiveTexture(GL_TEXTURE0 + texture++);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(elements, type, stride, OFFSET(offset));
        break;
      case vertex_usage_blendweight: break;
      case vertex_usage_blendindices: break;
      case vertex_usage_depth: break;
      case vertex_usage_tangent: break;
      case vertex_usage_binormal: break;
      case vertex_usage_fog:
        useFogCoords = true;
        glEnableClientState(GL_FOG_COORD_ARRAY);
        glFogCoordPointer(type, stride, OFFSET(offset));
        break;
      case vertex_usage_sample: break;
    }

    offset += size;
  }


  vertex_start *= (stride / 4);
	glDrawArrays(enigma::primitive_types[primitive], vertex_start, vertex_count);

  if (useVertices) glDisableClientState(GL_VERTEX_ARRAY);
  if (useNormals) glDisableClientState(GL_NORMAL_ARRAY);
  if (useColors) glDisableClientState(GL_COLOR_ARRAY);
  if (useFogCoords) glDisableClientState(GL_FOG_COORD_ARRAY);
  if (useTextCoords) {
    for (size_t i = 0; i < texture; ++i) {
      glClientActiveTexture(GL_TEXTURE0 + i);
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
  }

/*
glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3, GL_FLOAT, 16, &vertexBuffer->vertices[0]);
glEnableClientState(GL_COLOR_ARRAY);
glColorPointer(4, GL_UNSIGNED_BYTE, 16, &vertexBuffer->vertices[3]);
glDrawArrays(GL_TRIANGLES, 0, 3);
glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);*/
}

}
