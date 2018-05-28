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

#include "GSvertex.h"
#include "GStextures.h"
#include "GScolor_macros.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)

namespace enigma {

vector<VertexFormat*> vertexFormats;
vector<VertexBuffer*> vertexBuffers;

VertexFormat* vertexFormat = 0;

}

namespace enigma_user {

void vertex_format_begin() {
  enigma::vertexFormat = new enigma::VertexFormat();
}

void vertex_format_add_color() {
  enigma::vertexFormat->AddAttribute(vertex_type_color, vertex_usage_color);
}

void vertex_format_add_position() {
  enigma::vertexFormat->AddAttribute(vertex_type_float3, vertex_usage_position);
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
  int id = enigma::vertexFormats.size();
  enigma::vertexFormats.push_back(enigma::vertexFormat);
  return id;
}

bool vertex_format_exists(int id) {
  return (id >= 0 && (unsigned)id < enigma::vertexFormats.size() && enigma::vertexFormats[id] != nullptr);
}

void vertex_format_delete(int id) {
  delete enigma::vertexFormats[id];
  enigma::vertexFormats[id] = nullptr;
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
  delete enigma::vertexBuffers[buffer];
  enigma::vertexBuffers[buffer] = nullptr;
}

void vertex_begin(int buffer, int format) {
  enigma::vertexBuffers[buffer]->vertices.clear();
  enigma::vertexBuffers[buffer]->format = format;
}

void vertex_end(int buffer) {

}

unsigned vertex_get_size(int buffer) {
  return enigma::vertexBuffers[buffer]->vertices.size() * sizeof(gs_scalar);
}

unsigned vertex_get_number(int buffer) {
  return enigma::vertexBuffers[buffer]->vertices.size();
}

void vertex_freeze(int buffer) {
  if (enigma::vertexBuffers[buffer]->frozen) return;
  enigma::vertexBuffers[buffer]->frozen = true;
  enigma::graphics_create_vertex_buffer_peer(buffer);
}

void vertex_submit(int buffer, int primitive) {
  vertex_submit(buffer, primitive, 0, enigma::vertexBuffers[buffer]->vertices.size());
}

void vertex_submit(int buffer, int primitive, int texture) {
  texture_set(texture);
  vertex_submit(buffer, primitive);
}

void vertex_submit(int buffer, int primitive, int texture, unsigned offset, unsigned count) {
  texture_set(texture);
  vertex_submit(buffer, primitive, offset, count);
}

void vertex_position(int buffer, gs_scalar x, gs_scalar y) {
  enigma::vertexBuffers[buffer]->vertices.push_back(x);
  enigma::vertexBuffers[buffer]->vertices.push_back(y);
  enigma::vertexBuffers[buffer]->vertices.push_back(0);
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

void vertex_argb(int buffer, unsigned argb) {
  enigma::vertexBuffers[buffer]->vertices.push_back(argb);
}

void vertex_color(int buffer, int color, double alpha) {
  unsigned char a = alpha * 255;
  unsigned argb = (__GETR(color) << 24) | (__GETB(color) << 16) | (__GETG(color) << 8) | a;
  enigma::vertexBuffers[buffer]->vertices.push_back(argb);
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

}
