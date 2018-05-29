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

#include "GL3profiler.h"
#include "GL3shader.h"
#include "GLSLshader.h"

#include "Graphics_Systems/General/GSvertex.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmath.h"

#include "Graphics_Systems/General/OpenGLHeaders.h"

#include "Bridges/General/GL3Context.h" //Needed to get if bound texture == -1

#define bind_array_buffer(vbo) if (enigma::bound_vbo != vbo) glBindBuffer( GL_ARRAY_BUFFER, enigma::bound_vbo = vbo );

namespace enigma {

extern unsigned char currentcolor[4];
extern unsigned bound_vbo;
extern unsigned bound_vboi;
extern unsigned bound_shader;
extern vector<enigma::ShaderProgram*> shaderprograms;

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

  bind_array_buffer(peers[buffer]);
  glBufferData(GL_ARRAY_BUFFER, size, &vertexBuffer->vertices[0], GL_STATIC_DRAW);
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

  #ifdef DEBUG_MODE
  enigma::GPUProfilerBatch& vbd = oglmgr->gpuprof.add_drawcall();
  #endif

  if (enigma::transform_needs_update == true){
    enigma::transformation_update();
  }

  //Send transposed (done by GL because of "true" in the function below) matrices to shader
  enigma::glsl_uniform_matrix4fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_viewMatrix,  1, enigma::view_matrix);
  enigma::glsl_uniform_matrix4fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_projectionMatrix,  1, enigma::projection_matrix);
  enigma::glsl_uniform_matrix4fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_modelMatrix,  1, enigma::model_matrix);
  enigma::glsl_uniform_matrix4fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_mvMatrix,  1, enigma::mv_matrix);
  enigma::glsl_uniform_matrix4fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_mvpMatrix,  1, enigma::mvp_matrix);
  enigma::glsl_uniform_matrix3fv_internal(enigma::shaderprograms[enigma::bound_shader]->uni_normalMatrix,  1, enigma::normal_matrix);

  //Bind texture
  enigma::glsl_uniformi_internal(enigma::shaderprograms[enigma::bound_shader]->uni_texSampler, 0);

  bind_array_buffer(enigma::peers[buffer]);

  enigma::glsl_attribute_enable_all_internal(false); //Disable all attributes

  bool useTextCoords, useColors;
  size_t offset = 0;
  const size_t stride = vertexFormat->stride * sizeof(float);
  for (size_t i = 0; i < vertexFormat->flags.size(); ++i) {
    const pair<int, int> flag = vertexFormat->flags[i];

    enigma::glsl_attribute_enable_internal(flag.second,true);

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

    enigma::glsl_attribute_set_internal(flag.second, elements, type, (type == GL_FLOAT), stride, offset);

    offset += size;
  }

  enigma::glsl_uniformf_internal(enigma::shaderprograms[enigma::bound_shader]->uni_color, (float)enigma::currentcolor[0]/255.0f, (float)enigma::currentcolor[1]/255.0f, (float)enigma::currentcolor[2]/255.0f, (float)enigma::currentcolor[3]/255.0f);

  if (useTextCoords) {
    if (oglmgr->GetBoundTexture() != 0){
      enigma::glsl_uniformi_internal(enigma::shaderprograms[enigma::bound_shader]->uni_textureEnable, 1);
    } else {
      enigma::glsl_uniformi_internal(enigma::shaderprograms[enigma::bound_shader]->uni_textureEnable, 0);
    }
  } else {
    enigma::glsl_uniformi_internal(enigma::shaderprograms[enigma::bound_shader]->uni_textureEnable, 0);
  }
  enigma::glsl_uniformi_internal(enigma::shaderprograms[enigma::bound_shader]->uni_colorEnable, useColors);

  vertex_start *= (stride / 4);
	glDrawArrays(enigma::primitive_types[primitive], vertex_start, vertex_count);
}

}
