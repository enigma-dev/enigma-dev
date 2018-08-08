/** Copyright (C) 2013 Robert Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2017, 2018 Robert Colton
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

#include "GSmodel_impl.h"
#include "GSmodel.h"
#include "GSvertex.h"
#include "GSmatrix.h"
#include "GStextures.h"
#include "GSprimitives.h"

#include <math.h>

using namespace std;

namespace enigma {

vector<Model*> models;

}

namespace enigma_user {

int d3d_model_create(int type) {
  int id = enigma::models.size();
  enigma::Model* model = new enigma::Model(type);
  model->vertex_buffer = vertex_create_buffer();
  enigma::models.push_back(model);
  return id;
}

void d3d_model_destroy(int id) {
  const enigma::Model* model = enigma::models[id];
  vertex_delete_buffer(model->vertex_buffer);

  delete enigma::models[id];
  enigma::models[id] = nullptr;
}

bool d3d_model_exists(int id) {
  return (id >= 0 && (unsigned)id < enigma::models.size() && enigma::models[id] != nullptr);
}

void d3d_model_clear(int id) {
  enigma::Model* model = enigma::models[id];
  vertex_clear(model->vertex_buffer);
  model->primitives.clear();
}

void d3d_model_draw(int id) {
  const enigma::Model* model = enigma::models[id];
  if (model->vertex_started) {
    vertex_end(model->vertex_buffer);
  }
  if (model->type == model_static) {
    vertex_freeze(model->vertex_buffer);
  }
  for (auto primitive : model->primitives) {
    vertex_set_format(model->vertex_buffer, primitive.format);
    vertex_submit(
      model->vertex_buffer,
      primitive.type,
      primitive.vertex_start,
      primitive.vertex_count
    );
  }
}

void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z) {
  d3d_transform_add_translation(x, y, z);
  d3d_model_draw(id);
  d3d_transform_add_translation(-x, -y, -z);
}

void d3d_model_draw(int id, int texId) {
  texture_set(texId);
  d3d_model_draw(id);
}

void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId) {
  d3d_transform_add_translation(x, y, z);
  d3d_model_draw(id, texId);
  d3d_transform_add_translation(-x, -y, -z);
}

void d3d_model_primitive_begin(int id, int kind, int format) {
  enigma::Model* model = enigma::models[id];
  if (!model->vertex_started) {
    model->vertex_started = true;
    vertex_begin(model->vertex_buffer);
  }
  size_t start = 0;
  if (model->current_primitive > 0) {
    const enigma::Primitive& last_primitive = model->primitives[model->current_primitive - 1];
    start = last_primitive.vertex_start + last_primitive.vertex_count;
  }
  enigma::Primitive primitive(kind, format, start);
  model->primitives.push_back(primitive);
}

void d3d_model_primitive_end(int id) {
  enigma::Model* model = enigma::models[id];
  enigma::Primitive *primitive = &model->primitives[model->current_primitive++];
  vertex_set_format(model->vertex_buffer, primitive->format);
  primitive->vertex_count = vertex_get_number(model->vertex_buffer) - primitive->vertex_start;
}

void d3d_model_float1(int id, float f1) {
  const enigma::Model* model = enigma::models[id];
  vertex_float1(model->vertex_buffer, f1);
}

void d3d_model_float2(int id, float f1, float f2) {
  const enigma::Model* model = enigma::models[id];
  vertex_float2(model->vertex_buffer, f1, f2);
}

void d3d_model_float3(int id, float f1, float f2, float f3) {
  const enigma::Model* model = enigma::models[id];
  vertex_float3(model->vertex_buffer, f1, f2, f3);
}

void d3d_model_float4(int id, float f1, float f2, float f3, float f4) {
  const enigma::Model* model = enigma::models[id];
  vertex_float4(model->vertex_buffer, f1, f2, f3, f4);
}

void d3d_model_ubyte4(int id, uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4) {
  const enigma::Model* model = enigma::models[id];
  vertex_ubyte4(model->vertex_buffer, u1, u2, u3, u4);
}

void d3d_model_vertex(int id, gs_scalar x, gs_scalar y) {
  enigma::Model* model = enigma::models[id];
  enigma::Primitive &primitive = model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive.format)) {
    if (vertex_format_exists()) {
      primitive.format = vertex_format_end();
    } else {
      vertex_format_begin();
      vertex_format_add_position();
    }
  }
  vertex_position(model->vertex_buffer, x, y);
}

void d3d_model_vertex(int id, gs_scalar x, gs_scalar y, gs_scalar z) {
  enigma::Model* model = enigma::models[id];
  enigma::Primitive &primitive = model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive.format)) {
    if (vertex_format_exists()) {
      primitive.format = vertex_format_end();
    } else {
      vertex_format_begin();
      vertex_format_add_position_3d();
    }
  }
  vertex_position_3d(model->vertex_buffer, x, y, z);
}

void d3d_model_color(int id, int col, double alpha) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_color();
  }
  vertex_color(model->vertex_buffer, col, alpha);
}

void d3d_model_argb(int id, unsigned argb) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_color();
  }
  vertex_argb(model->vertex_buffer, argb);
}

void d3d_model_texcoord(int id, gs_scalar tx, gs_scalar ty) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_textcoord();
  }
  vertex_texcoord(model->vertex_buffer, tx, ty);
}

void d3d_model_normal(int id, gs_scalar nx, gs_scalar ny, gs_scalar nz) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_normal();
  }
  vertex_normal(model->vertex_buffer, nx, ny, nz);
}

void d3d_model_float1(int id, int usage, float f1) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_custom(vertex_type_float1, usage);
  }
  vertex_float1(model->vertex_buffer, f1);
}

void d3d_model_float2(int id, int usage, float f1, float f2) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_custom(vertex_type_float2, usage);
  }
  vertex_float2(model->vertex_buffer, f1, f2);
}

void d3d_model_float3(int id, int usage, float f1, float f2, float f3) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_custom(vertex_type_float3, usage);
  }
  vertex_float3(model->vertex_buffer, f1, f2, f3);
}

void d3d_model_float4(int id, int usage, float f1, float f2, float f3, float f4) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_custom(vertex_type_float4, usage);
  }
  vertex_float4(model->vertex_buffer, f1, f2, f3, f4);
}

void d3d_model_ubyte4(int id, int usage, uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4) {
  const enigma::Model* model = enigma::models[id];
  const enigma::Primitive *primitive = &model->primitives[model->current_primitive];
  if (!vertex_format_exists(primitive->format)) {
    vertex_format_add_custom(vertex_type_ubyte4, usage);
  }
  vertex_ubyte4(model->vertex_buffer, u1, u2, u3, u4);
}

void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, int col, double alpha) {
  d3d_model_vertex(id, x, y);
  d3d_model_color(id, col, alpha);
}

void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, int col, double alpha) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_color(id, col, alpha);
}

void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty) {
  d3d_model_vertex(id, x, y);
  d3d_model_texcoord(id, tx, ty);
}

void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_texcoord(id, tx, ty);
}

void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, double alpha) {
  d3d_model_vertex(id, x, y);
  d3d_model_texcoord(id, tx, ty);
  d3d_model_color(id, col, alpha);
}

void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int col, double alpha) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_texcoord(id, tx, ty);
  d3d_model_color(id, col, alpha);
}

void d3d_model_vertex_normal(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_normal(id, nx, ny, nz);
}

void d3d_model_vertex_normal_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int col, double alpha) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_normal(id, nx, ny, nz);
  d3d_model_color(id, col, alpha);
}

void d3d_model_vertex_normal_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_normal(id, nx, ny, nz);
  d3d_model_texcoord(id, tx, ty);
}

void d3d_model_vertex_normal_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int col, double alpha) {
  d3d_model_vertex(id, x, y, z);
  d3d_model_normal(id, nx, ny, nz);
  d3d_model_texcoord(id, tx, ty);
  d3d_model_color(id, col, alpha);
}

void d3d_model_save(int id, string fname) {
  //TODO: Write save code that has never been done before yet
}

bool d3d_model_load(int id, string fname) {
  d3d_model_clear(id);
}

void d3d_model_floor(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep) {
  // Setup U and V vectors
  gs_scalar vX = x2 - x1;
  gs_scalar vY = y2 - y1;
  gs_scalar vZ = z2 - z1;
  gs_scalar wX = x2 - x1;
  gs_scalar wZ = z2 - z1;

  // Cross-Product of vectors to create normal vector
  gs_scalar nX = (vY * wZ);
  gs_scalar nY = (vZ * wX) - (vX * wZ);
  gs_scalar nZ = -(vY * wX);

  // Normalize result
  gs_scalar mag = sqrt((nX*nX) + (nY*nY) + (nZ*nZ));
  nX /= mag, nY /= mag, nZ /= mag;

  // Create floor
  d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture(id, x1, y1, z1, nX, nY, nZ, 0,    0);
    d3d_model_vertex_normal_texture(id, x1, y2, z1, nX, nY, nZ, 0,    vrep);
    d3d_model_vertex_normal_texture(id, x2, y1, z2, nX, nY, nZ, hrep, 0);
    d3d_model_vertex_normal_texture(id, x2, y2, z2, nX, nY, nZ, hrep, vrep);
  d3d_model_primitive_end(id);
}

void d3d_model_wall(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep) {
  // Setup U and V vectors
  gs_scalar vX = x2 - x1;
  gs_scalar vY = y2 - y1;
  gs_scalar wZ = z2 - z1;

  // Cross-Product of vectors to create normal vector
  gs_scalar nX = (vY * wZ);
  gs_scalar nY = -(vX * wZ);

  // Normalize result
  gs_scalar mag = sqrt((nX*nX) + (nY*nY));
  nX /= mag, nY /= mag;

  // Create wall
  d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture(id, x1, y1, z1, nX, nY, 0, 0, 0);
    d3d_model_vertex_normal_texture(id, x2, y2, z1, nX, nY, 0, hrep, 0);
    d3d_model_vertex_normal_texture(id, x1, y1, z2, nX, nY, 0, 0, vrep);
    d3d_model_vertex_normal_texture(id, x2, y2, z2, nX, nY, 0, hrep, vrep);
  d3d_model_primitive_end(id);
}

void d3d_model_block(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  // GM's block model function requires uninterpolated "flat" normals if we want to be visually authentic
  // This is slightly slower because it requires more vertices to make the normals flat but we have to...

  // Negative X
  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture( id, x1,y1,z1, -1,0,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x1,y1,z2, -1,0,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x1,y2,z2, -1,0,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x1,y2,z1, -1,0,0, 1,1 );
  d3d_model_primitive_end(id);

  // Positive X
  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture( id, x2,y1,z1, 1,0,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x2,y2,z1, 1,0,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x2,y2,z2, 1,0,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x2,y1,z2, 1,0,0, 1,0 );
  d3d_model_primitive_end(id);

  // Negative Y
  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture( id, x1,y1,z1, 0,-1,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x2,y1,z1, 0,-1,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x2,y1,z2, 0,-1,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x1,y1,z2, 0,-1,0, 0,0 );
  d3d_model_primitive_end(id);

  // Positive Y
  d3d_model_primitive_begin(id, pr_trianglestrip);
  d3d_model_vertex_normal_texture( id, x1,y2,z1, 0,1,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x1,y2,z2, 0,1,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x2,y2,z2, 0,1,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x2,y2,z1, 0,1,0, 0,1 );
  d3d_model_primitive_end(id);

  if (closed) {
    // Negative Z
    d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture( id, x1,y1,z1, 0,0,-1, 0,0 );
    d3d_model_vertex_normal_texture( id, x1,y2,z1, 0,0,-1, 0,1 );
    d3d_model_vertex_normal_texture( id, x2,y2,z1, 0,0,-1, 1,1 );
    d3d_model_vertex_normal_texture( id, x2,y1,z1, 0,0,-1, 1,0 );
    d3d_model_primitive_end(id);

    // Positive Z
    d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture( id, x1,y1,z2, 0,0,1, 0,0 );
    d3d_model_vertex_normal_texture( id, x2,y1,z2, 0,0,1, 1,0 );
    d3d_model_vertex_normal_texture( id, x2,y2,z2, 0,0,1, 1,1 );
    d3d_model_vertex_normal_texture( id, x1,y2,z2, 0,0,1, 0,1 );
    d3d_model_primitive_end(id);
  }
}

void d3d_model_cylinder(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  float v[100][3];
  float t[100][3];
  steps = min(max(steps, 3), 48); // i think 48 should be circle_presicion
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k;
  a = 0; px = cx+rx; py = cy; tp = 0; k = 0;

  d3d_model_primitive_begin(id, pr_trianglestrip);
  for (int i = 0; i <= steps; i++) {
    v[k][0] = px; v[k][1] = py; v[k][2] = z2;
    t[k][0] = tp; t[k][1] = 0;
    d3d_model_vertex_normal_texture(id, px, py, z2, cos(a), sin(a), 0, tp, 0);
    k++;
    v[k][0] = px; v[k][1] = py; v[k][2] = z1;
    t[k][0] = tp; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, px, py, z1, cos(a), sin(a), 0, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  d3d_model_primitive_end(id);

  if (closed) {
    // BOTTOM
    d3d_model_primitive_begin(id, pr_trianglefan);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
    t[k][0] = 0; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, cx, cy, z1, 0, 0, -1, 0, vrep);
    k++;
    for (int i = steps*2; i >= 0; i-=2) {
      d3d_model_vertex_normal_texture(id, v[i+1][0], v[i+1][1], v[i+1][2], 0, 0, -1, t[i][0], t[i][1]);
    }
    d3d_model_primitive_end(id);

    // TOP
    d3d_model_primitive_begin(id, pr_trianglefan);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
    t[k][0] = 0; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, cx, cy, z2, 0, 0, 1, 0, vrep);
    k++;
    for (int i = 0; i <= steps*2; i+=2) {
      d3d_model_vertex_normal_texture(id, v[i][0], v[i][1], v[i][2], 0, 0, 1, t[i][0], t[i][1]);
    }
    d3d_model_primitive_end(id);
  }
}

void d3d_model_cone(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k = 0;
  d3d_model_primitive_begin(id, pr_trianglefan);
  d3d_model_vertex_normal_texture(id, cx, cy, z2, 0, 0, 1, 0, 0);
  k++;
  a = 0; px = cx+rx; py = cy; tp = 0;
  for (int i = 0; i <= steps; i++) {
    d3d_model_vertex_normal_texture(id, px, py, z1, cos(a), sin(a), 0, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  d3d_model_primitive_end(id);
  if (closed) {
    d3d_model_primitive_begin(id, pr_trianglefan);
    d3d_model_vertex_normal_texture(id, cx, cy, z1, 0, 0, -1, 0, vrep);
    k++;
    tp = 0;
    for (int i = 0; i <= steps + 1; i++) {
      d3d_model_vertex_normal_texture(id, px, py, z1, 0, 0, -1, tp, 0);
      k++; a -= pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    d3d_model_primitive_end(id);
  }
}

void d3d_model_ellipsoid(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{
  float v[277][3];
  float n[277][3];
  float t[277][3];
  steps = min(max(steps, 3), 24);
  const int zsteps = ceil(steps/2);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
  double a, b, px, py, pz, tp, tzp, cosb;
  double cosx[25], siny[25], txp[25];
  a = pr; tp = 0;
  for (int i = 0; i <= steps; i++) {
    cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
    txp[i] = tp;
    a += pr; tp += invstep;
  }
  int k = 0, kk;
  // BOTTOM
  d3d_model_primitive_begin(id, pr_trianglefan);
  v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
  n[k][0] = 0; n[k][1] = 0; n[k][2] = -1;
  t[k][0] = 0; t[k][1] = vrep;
  d3d_model_vertex_normal_texture(id, cx, cy, cz - rz, 0, 0, -1, 0, vrep);
  k++;
  b = qr-M_PI/2;
  cosb = cos(b);
  pz = rz*sin(b);
  tzp = vrep-invstep2;
  px = cx+rx*cosb; py = cy;
  for (int i = 0; i <= steps; i++) {
    v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
    n[k][0] = cosx[i]; n[k][1] = -siny[i]; n[k][2] = cosb;
    t[k][0] = txp[i]; t[k][1] = tzp;
    d3d_model_vertex_normal_texture(id, px, py, cz + pz, cosx[i], -siny[i], sin(b), txp[i], tzp);
    k++; px = cx+cosx[i]*cosb; py = cy-siny[i]*cosb;
  }
  d3d_model_primitive_end(id);
  // SIDES
  for (int ii = 0; ii < zsteps - 2; ii++) {
    b += qr;
    cosb = cos(b);
    pz = rz*sin(b);
    tzp -= invstep2;
    d3d_model_primitive_begin(id, pr_trianglestrip);
    px = cx+rx*cosb; py = cy;
    for (int i = 0; i <= steps; i++) {
      kk = k - steps - 1;
      d3d_model_vertex_normal_texture(id, v[kk][0], v[kk][1], v[kk][2], n[kk][0], n[kk][1], n[kk][2], t[kk][0], t[kk][1]);
      v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
      n[k][0] = cosx[i]; n[k][1] = -siny[i]; n[k][2] = sin(b);
      t[k][0] = txp[i]; t[k][1] = tzp;
      d3d_model_vertex_normal_texture(id, px, py, cz + pz, cosx[i], -siny[i], sin(b), txp[i], tzp);
      k++; px = cx+cosx[i]*cosb; py = cy-siny[i]*cosb;
    }
    d3d_model_primitive_end(id);
  }
  // TOP
  d3d_model_primitive_begin(id, pr_trianglefan);
  v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
  n[k][0] = 0; n[k][1] = 0; n[k][2] = 1;
  t[k][0] = 0; t[k][1] = 0;
  d3d_model_vertex_normal_texture(id, cx, cy, cz + rz, 0, 0, 1, 0, 0);
  k++;
  for (int i = k - 2; i >= k - steps - 2; i--) {
    d3d_model_vertex_normal_texture(id, v[i][0], v[i][1], v[i][2], n[i][0], n[i][1], n[i][2], t[i][0], t[i][1]);
  }
  d3d_model_primitive_end(id);
}

void d3d_model_icosahedron(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{
  gs_scalar width = (x2-x1), length = (y2-y1), height = (z2-z1);
  static gs_scalar vdata[12][3] = {
    {0, 0.5, 1}, {1, 0.5, 1}, {0, 0.5, 0}, {1, 0.5, 0},
    {0.5, 1, 1}, {0.5, 1, 0}, {0.5, 0, 1}, {0.5, 0, 0},
    {1, 1, 0.5}, {0, 1, 0.5}, {1, 0, 0.5}, {0, 0, 0.5}
  };

  static int tindices[20][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
  };

  d3d_model_primitive_begin(id, pr_trianglelist);
  for (unsigned i = 0; i < 20; i++) {
    d3d_model_vertex(id, x1 + vdata[tindices[i][0]][0] * width, y1 + vdata[tindices[i][0]][1] * length, z1 + vdata[tindices[i][0]][2] * height);
    d3d_model_vertex(id, x1 + vdata[tindices[i][1]][0] * width, y1 + vdata[tindices[i][1]][1] * length, z1 + vdata[tindices[i][1]][2] * height);
    d3d_model_vertex(id, x1 + vdata[tindices[i][2]][0] * width, y1 + vdata[tindices[i][2]][1] * length, z1 + vdata[tindices[i][2]][2] * height);
    // TODO: Add normals, uv's, and subdivide by the number of steps
  }
  d3d_model_primitive_end(id);
}

void d3d_model_torus(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius)
{
  double TWOPI = 2 * (double)M_PI;
  for (int i = 0; i < csteps; i++) {
    d3d_model_primitive_begin(id, pr_trianglestrip);
    for (int j = 0; j <= tsteps; j++) {
      for (int k = 1; k >= 0; k--) {

        double s = (i + k) % csteps + 0.5;
        double t = j % tsteps;

        double x = (radius + tradius * cos(s * TWOPI / csteps)) * cos(t * TWOPI / tsteps);
        double y = (radius + tradius * cos(s * TWOPI / csteps)) * sin(t * TWOPI / tsteps);
        double z = tradius * sin(s * TWOPI / csteps);
        double u = ((i + k) / (float)csteps) * hrep;
        double v = (j / (float)tsteps) * vrep;

        gs_scalar nX = cos(s * TWOPI / csteps) * cos(t * TWOPI / tsteps);
        gs_scalar nY = cos(s * TWOPI / csteps) * sin(t * TWOPI / tsteps);
        gs_scalar nZ = sin(s * TWOPI / csteps);

        d3d_model_vertex_normal_texture(id, x1 + x, y1 + y, z1 + z, nX, nY, nZ, u, v);
      }
    }
    d3d_model_primitive_end(id);
  }
}

}
