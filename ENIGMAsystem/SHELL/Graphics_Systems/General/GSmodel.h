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

#include <string>
#include "OpenGLHeaders.h"

extern GLenum vbotypes[];

namespace enigma_user {

enum {
  vbo_static,
  vbo_dynamic,
  vbo_stream
};

  unsigned int d3d_model_create(int vbot = vbo_static);
  void d3d_model_destroy(const unsigned int id);
  void d3d_model_copy(const unsigned int id, const unsigned int source);
  unsigned int d3d_model_duplicate(const unsigned int source);
  void d3d_model_merge(const unsigned int id, const unsigned int source);
  bool d3d_model_exists(const unsigned int id);
  void d3d_model_clear(const unsigned int id);
  void d3d_model_save(const unsigned int id, std::string fname);
  bool d3d_model_load(const unsigned int id, std::string fname);
  void d3d_model_draw(const unsigned int id);
  void d3d_model_draw(const unsigned int id, float x, float y, float z);
  void d3d_model_draw(const unsigned int id, int texId);
  void d3d_model_draw(const unsigned int id, float x, float y, float z, int texId);
  void d3d_model_primitive_begin(const unsigned int id, int kind);
  void d3d_model_primitive_end(const unsigned int id);
  void d3d_model_vertex(const unsigned int id, float x, float y, float z);
  void d3d_model_normal(const unsigned int id, float nx, float ny, float nz);
  void d3d_model_texture(const unsigned int id, float tx, float ty);
  void d3d_model_color(const unsigned int id, int col, double alpha);
  void d3d_model_index(const unsigned int id, GLuint in);
  void d3d_model_vertex_color(const unsigned int id, float x, float y, float z, int col, double alpha);
  void d3d_model_vertex_texture(const unsigned int id, float x, float y, float z, float tx, float ty);
  void d3d_model_vertex_texture_color(const unsigned int id, float x, float y, float z, float tx, float ty, int col, double alpha);
  void d3d_model_vertex_normal(const unsigned int id, float x, float y, float z, float nx, float ny, float nz);
  void d3d_model_vertex_normal_color(const unsigned int id, float x, float y, float z, float nx, float ny, float nz, int col, double alpha);
  void d3d_model_vertex_normal_texture(const unsigned int id, float x, float y, float z, float nx, float ny, float nz, float tx, float ty);
  void d3d_model_vertex_normal_texture_color(const unsigned int id, float x, float y, float z, float nx, float ny, float nz, float tx, float ty, int col, double alpha);
  void d3d_model_wall(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep);
  void d3d_model_floor(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep);
  void d3d_model_block(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep, bool closed = true);
  void d3d_model_cylinder(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep, bool closed, int steps);
  void d3d_model_cone(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep, bool closed, int steps);
  void d3d_model_ellipsoid(const unsigned int id, float x1, float y1, float z1, float x2, float y2, float z2, float hrep, float vrep, int steps);
  void d3d_model_icosahedron(const unsigned int id);
  void d3d_model_torus(const unsigned int id, float x1, float y1, float z1, float hrep, float vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI = 2*3.14);
}

