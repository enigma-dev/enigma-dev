/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#ifndef ENIGMA_USER_GSMODEL
#define ENIGMA_USER_GSMODEL

#include "Universal_System/scalar.h"

#include <string>

namespace enigma_user {
  enum {
      model_static   = 0, // GL_STATIC_DRAW     D3DUSAGE_WRITEONLY
      model_dynamic  = 1, // GL_DYNAMIC_DRAW    D3DUSAGE_DYNAMIC
      model_stream   = 2  // GL_STREAM_DRAW
  };

  unsigned d3d_model_create(int type = model_static);
  void d3d_model_destroy(int id);
  bool d3d_model_exists(int id);
  void d3d_model_clear(int id);
  unsigned d3d_model_get_stride(int id);
  void d3d_model_save(int id, std::string fname);
  bool d3d_model_load(int id, std::string fname);
  void d3d_model_draw(int id);
  void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z);
  void d3d_model_draw(int id, int texId);
  void d3d_model_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId);
  void d3d_model_part_draw(int id, int vertex_count);
  void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int vertex_count);
  void d3d_model_part_draw(int id, int texId, int vertex_count);
  void d3d_model_part_draw(int id, gs_scalar x, gs_scalar y, gs_scalar z, int texId, int vertex_count);
  void d3d_model_primitive_begin(int id, int kind);
  void d3d_model_primitive_end(int id);
  void d3d_model_vertex(int id, gs_scalar x, gs_scalar y);
  void d3d_model_vertex(int id, gs_scalar x, gs_scalar y, gs_scalar z);
  void d3d_model_index(int id, unsigned ind);
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
  void d3d_model_wall(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep);
  void d3d_model_floor(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep);
  void d3d_model_block(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed = true);
  void d3d_model_cylinder(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
  void d3d_model_cone(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps);
  void d3d_model_ellipsoid(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps);
  void d3d_model_icosahedron(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps);
  void d3d_model_torus(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius);

  bool d3d_model_has_color(int id);
  bool d3d_model_has_texture(int id);
  bool d3d_model_has_normals(int id);
}

#endif
