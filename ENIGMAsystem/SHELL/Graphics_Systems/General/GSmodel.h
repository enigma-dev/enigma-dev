/** Copyright (C) 2008-2013 Josh Ventura, Robert Colton
*** Copyright (C) 2018 Robert Colton
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

#ifndef ENIGMA_GSMODEL_H
#define ENIGMA_GSMODEL_H

#include "Universal_System/scalar.h"
#include "Universal_System/dynamic_args.h" // for d3d_model_vertex_data

#include <string>
#include <stdint.h>

namespace enigma_user {
  enum {
    // write to buffer peer once
    // aka GL_STATIC_DRAW or D3DUSAGE_WRITEONLY
    // (e.g, single time in the create event)
    model_static   = 0,
    // write to buffer peer infrequently
    // aka GL_DYNAMIC_DRAW or D3DUSAGE_WRITEONLY
    // (e.g, every other step or in an alarm event)
    model_dynamic  = 1,
    // write to buffer peer every frame
    // aka GL_STREAM_DRAW or D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC
    // (e.g, specifying primitives in the draw event)
    model_stream   = 2,
  };

  int d3d_model_create(int type = model_static, bool use_draw_color = false);
  void d3d_model_destroy(int id);
  bool d3d_model_exists(int id);
  void d3d_model_clear(int id);
  void d3d_model_save(int id, std::string fname);
  bool d3d_model_load(int id, std::string fname);
  void d3d_model_draw(int id);
  void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z);
  void d3d_model_draw(int id, int texId);
  void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId);
  void d3d_model_primitive_begin(int id, int kind, int format = -1);
  void d3d_model_primitive_end(int id);

  // Custom granular model specification functions
  // These assume that a valid vertex format was supplied when d3d_model_primitive_begin was called
  // and may not work correctly if that precondition is false.
  void d3d_model_float1(int id, float f1);
  void d3d_model_float2(int id, float f1, float f2);
  void d3d_model_float3(int id, float f1, float f2, float f3);
  void d3d_model_float4(int id, float f1, float f2, float f3, float f4);
  void d3d_model_ubyte4(int id, uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4);

  // Granular model specification functions
  // If a vertex format is not specified during d3d_model_primitive_begin, it will be calculated
  // between calls to d3d_model_vertex based on the order that the following functions were called.
  void d3d_model_vertex(int id, gs_scalar x, gs_scalar y);
  void d3d_model_vertex(int id, gs_scalar x, gs_scalar y, gs_scalar z);
  void d3d_model_color(int id, int col, double alpha);
  void d3d_model_argb(int id, unsigned argb);
  void d3d_model_texture(int id, gs_scalar tx, gs_scalar ty);
  void d3d_model_normal(int id, gs_scalar nx, gs_scalar ny, gs_scalar nz);

  // Custom granular model specification functions
  // These are part of the granular model specification functions but accept a vertex format usage
  // to assist in determining the vertex format since one was not supplied during d3d_model_primitive_begin.
  void d3d_model_float1(int id, int usage, float f1);
  void d3d_model_float2(int id, int usage, float f1, float f2);
  void d3d_model_float3(int id, int usage, float f1, float f2, float f3);
  void d3d_model_float4(int id, int usage, float f1, float f2, float f3, float f4);
  void d3d_model_ubyte4(int id, int usage, uint8_t u1, uint8_t u2, uint8_t u3, uint8_t u4);

  // Custom composite specification functions
  // These assume that a valid vertex format was supplied when d3d_model_primitive_begin was called
  // and may not work correctly if that precondition is false. They allow you to specify the model
  // data in a more expressive way.
  void d3d_model_vertex_data(int id, const enigma::varargs& data);

  // Composite model specification functions
  // These call the granular specification functions so that the vertex format can be correctly guessed.
  void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, int col, double alpha);
  void d3d_model_vertex_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, int col, double alpha);
  void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty);
  void d3d_model_vertex_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty);
  void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, double alpha);
  void d3d_model_vertex_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int col, double alpha);
  void d3d_model_vertex_normal(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz);
  void d3d_model_vertex_normal_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int col, double alpha);
  void d3d_model_vertex_normal_texture(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty);
  void d3d_model_vertex_normal_texture_color(int id, gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int col, double alpha);

  // Shape building functions
  // These may use either the granular or composite model specification functions to define generic shapes in the model.
  void d3d_model_wall(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep);
  void d3d_model_floor(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep);
  void d3d_model_block(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed = true);
  void d3d_model_cylinder(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
  void d3d_model_cone(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
  void d3d_model_ellipsoid(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps);
  void d3d_model_icosahedron(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps);
  void d3d_model_torus(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius);
}

#endif
