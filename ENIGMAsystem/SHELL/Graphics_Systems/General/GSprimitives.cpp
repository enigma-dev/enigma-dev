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

#include "GSprimitives.h"
#include "GSmodel.h"
#include "GScolors.h"
#include "GStextures.h"

namespace {

int draw_primitive_model = enigma_user::d3d_model_create(enigma_user::model_stream);
int draw_primitive_texture = -1;

} // anonymous namespace

namespace enigma_user
{

unsigned draw_primitive_count(int kind, unsigned vertex_count) {
  switch (kind) {
    case pr_pointlist: return vertex_count;
    case pr_linelist: return vertex_count / 2;
    case pr_linestrip: if (vertex_count > 1) return vertex_count - 1; break;
    case pr_trianglelist: return vertex_count / 3;
    case pr_trianglestrip: if (vertex_count > 2) return vertex_count - 2; break;
    case pr_trianglefan: if (vertex_count > 2) return vertex_count - 2; break;
  }
  return 0;
}

void draw_primitive_begin(int kind)
{
  d3d_model_primitive_begin(draw_primitive_model, kind);
  draw_primitive_texture = -1;
}

void draw_primitive_begin_texture(int kind, int texId)
{
  d3d_model_primitive_begin(draw_primitive_model, kind);
  draw_primitive_texture = texId;
}

void draw_primitive_end()
{
  d3d_model_primitive_end(draw_primitive_model);
  d3d_model_draw(draw_primitive_model, draw_primitive_texture);
  d3d_model_clear(draw_primitive_model);
}

void draw_vertex(gs_scalar x, gs_scalar y)
{
  d3d_model_vertex_color(draw_primitive_model, x, y, draw_get_color(), draw_get_alpha());
}

void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
  d3d_model_vertex_color(draw_primitive_model, x, y, col, alpha);
}

void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(draw_primitive_model, x, y, tx, ty, draw_get_color(), draw_get_alpha());
}

void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
  d3d_model_vertex_texture_color(draw_primitive_model, x, y, tx, ty, col, alpha);
}

void d3d_primitive_begin(int kind)
{
  d3d_model_primitive_begin(draw_primitive_model, kind);
  draw_primitive_texture = -1;
}

void d3d_primitive_begin_texture(int kind, int texId)
{
  d3d_model_primitive_begin(draw_primitive_model, kind);
  draw_primitive_texture = texId;
}

void d3d_primitive_end()
{
  d3d_model_primitive_end(draw_primitive_model);
  d3d_model_draw(draw_primitive_model, draw_primitive_texture);
  d3d_model_clear(draw_primitive_model);
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z)
{
  d3d_model_vertex_color(draw_primitive_model, x, y, z, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha)
{
  d3d_model_vertex_color(draw_primitive_model, x, y, z, color, alpha);
}

void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(draw_primitive_model, x, y, z, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_texture_color(draw_primitive_model, x, y, z, tx, ty, color, alpha);
}

void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  d3d_model_vertex_normal_color(draw_primitive_model, x, y, z, nx, ny, nz, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha)
{
  d3d_model_vertex_normal_color(draw_primitive_model, x, y, z, nx, ny, nz, color, alpha);
}

void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_normal_texture_color(draw_primitive_model, x, y, z, nx, ny, nz, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_normal_texture_color(draw_primitive_model, x, y, z, nx, ny, nz, tx, ty, color, alpha);
}

void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
	d3d_model_floor(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
	d3d_model_wall(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
{
	d3d_model_block(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
	d3d_model_cylinder(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
	d3d_model_cone(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{
	d3d_model_ellipsoid(draw_primitive_model, x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps) {
}

void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius) {
	d3d_model_torus(draw_primitive_model, x1, y1, z1, hrep, vrep, csteps, tsteps, radius, tradius);
  d3d_model_draw(draw_primitive_model, texId);
  d3d_model_clear(draw_primitive_model);
}

}
