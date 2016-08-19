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

#include "../General/OpenGLHeaders.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

#include "../General/GSvertex.h"

namespace enigma {

struct VertexFormat {
  map<int,int> flags;
  
  VertexFormat() {
  
  }
  
  ~VertexFormat() {
  
  }
  
  void AddAttribute(int type, int usage) {
    flags.insert(map<int,int>::value_type(type, usage));
  }
};

struct VertexBuffer {
  vector<gs_scalar> vertices;
  vector<gs_scalar> indices;
  
  GLuint vertexBuffer;
  GLuint indexBuffer;
};

vector<VertexFormat*> vertexFormats;
vector<VertexBuffer*> vertexBuffers;

VertexFormat* vertexFormat = 0;

}

namespace enigma_user {

int vertex_create_buffer() {
  return -1;
}

int vertex_create_buffer_ext(unsigned size) {
  (void) size;
  return -1;
}

void vertex_delete_buffer(int buffer) {

}

void vertex_begin(int buffer, int format) {

}

void vertex_end(int buffer) {

}

void vertex_freeze(int buffer) {

}

void vertex_submit(int buffer, int primitive) {

}

void vertex_submit(int buffer, int primitive, int texture) {

}

void vertex_delete(int buffer) {

}

void vertex_index(int buffer, unsigned id) {

}

void vertex_position(int buffer, gs_scalar x, gs_scalar y) {

}

void vertex_position_3d(int buffer, gs_scalar x, gs_scalar y, gs_scalar z) {

}

void vertex_normal(int buffer, gs_scalar nx, gs_scalar ny, gs_scalar nz) {

}

void vertex_texcoord(int buffer, gs_scalar u, gs_scalar v) {

}

void vertex_argb(int buffer, double alpha, unsigned char r, unsigned char g, unsigned char b) {

}

void vertex_colour(int buffer, int color, double alpha) {

}

void vertex_float1(int buffer, float f1) {

}

void vertex_float2(int buffer, float f1, float f2) {

}

void vertex_float3(int buffer, float f1, float f2, float f3) {

}

void vertex_float4(int buffer, float f1, float f2, float f3, float f4) {

}

void vertex_ubyte4(int buffer, unsigned char u1, unsigned char u2, unsigned char u3, unsigned char u4) {

}

void vertex_format_begin() {
  enigma::vertexFormat = new enigma::VertexFormat();
}

void vertex_format_add_colour() {
  enigma::vertexFormat->AddAttribute(vertex_type_colour, vertex_usage_colour);
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
  enigma::vertexFormats.push_back(enigma::vertexFormat);
  return enigma::vertexFormats.size() - 1;
}

}
