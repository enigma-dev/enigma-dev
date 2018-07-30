/** Copyright (C) 2008-2013 Robert B. Colton
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

#include "GLmanager.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GSmodel.h"

#include "Widget_Systems/widgets_mandatory.h"

#include <string>
#include <stdio.h>

namespace enigma_user
{

void draw_primitive_begin(int kind)
{
  oglmgr->BeginShapesBatching(-1);
  d3d_model_primitive_begin(oglmgr->GetShapesModel(), kind);
}

void draw_primitive_begin_texture(int kind, int texId)
{
  oglmgr->BeginShapesBatching(texId);
  d3d_model_primitive_begin(oglmgr->GetShapesModel(), kind);
}

void draw_primitive_end()
{
  d3d_model_primitive_end(oglmgr->GetShapesModel());
}

void draw_vertex(gs_scalar x, gs_scalar y)
{
  d3d_model_vertex_color(oglmgr->GetShapesModel(), x, y, draw_get_color(), draw_get_alpha());
}

void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
  d3d_model_vertex_color(oglmgr->GetShapesModel(), x, y, col, alpha);
}

void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(oglmgr->GetShapesModel(), x, y, tx, ty, draw_get_color(), draw_get_alpha());
}

void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
  d3d_model_vertex_texture_color(oglmgr->GetShapesModel(), x, y, tx, ty, col, alpha);
}

void d3d_primitive_begin(int kind)
{
  oglmgr->BeginShapesBatching(-1);
  d3d_model_primitive_begin(oglmgr->GetShapesModel(), kind);
}

void d3d_primitive_begin_texture(int kind, int texId)
{
  oglmgr->BeginShapesBatching(texId);
  d3d_model_primitive_begin(oglmgr->GetShapesModel(), kind);
}

void d3d_primitive_end()
{
  d3d_model_primitive_end(oglmgr->GetShapesModel());
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z)
{
  d3d_model_vertex_color(oglmgr->GetShapesModel(), x, y, z, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha)
{
  d3d_model_vertex_color(oglmgr->GetShapesModel(), x, y, z, color, alpha);
}

void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(oglmgr->GetShapesModel(), x, y, z, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_texture_color(oglmgr->GetShapesModel(), x, y, z, tx, ty, color, alpha);
}

void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  d3d_model_vertex_normal_color(oglmgr->GetShapesModel(), x, y, z, nx, ny, nz, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha)
{
  d3d_model_vertex_normal_color(oglmgr->GetShapesModel(), x, y, z, nx, ny, nz, color, alpha);
}

void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_normal_texture_color(oglmgr->GetShapesModel(), x, y, z, nx, ny, nz, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_normal_texture_color(oglmgr->GetShapesModel(), x, y, z, nx, ny, nz, tx, ty, color, alpha);
}

void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_floor(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_wall(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_block(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
}

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_cylinder(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_cone(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{
	oglmgr->BeginShapesBatching(texId);
	d3d_model_ellipsoid(oglmgr->GetShapesModel(), x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
}

void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps) {
}

void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius) {
	oglmgr->BeginShapesBatching(texId);
	d3d_model_torus(oglmgr->GetShapesModel(), x1, y1, z1, hrep, vrep, csteps, tsteps, radius, tradius);
}

}
