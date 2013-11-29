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

#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"
#include "../General/GScolors.h"
#include "../General/GSprimitives.h"
#include "../General/GStextures.h"
#include "../General/GSmodel.h"

#include <string>
#include "Widget_Systems/widgets_mandatory.h"

#if PRIMBUFFER
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

int ptypes_by_id[16] = {
  D3DPT_POINTLIST, D3DPT_POINTLIST, D3DPT_LINELIST, D3DPT_LINESTRIP, D3DPT_TRIANGLELIST,
  D3DPT_TRIANGLESTRIP, D3DPT_TRIANGLEFAN, D3DPT_POINTLIST, D3DPT_POINTLIST,
  D3DPT_POINTLIST, D3DPT_POINTLIST,

  //These are padding.
  D3DPT_POINTLIST, D3DPT_POINTLIST, D3DPT_POINTLIST, D3DPT_POINTLIST, D3DPT_POINTLIST
};

namespace enigma_user
{

void draw_primitive_begin(int kind)
{
  d3dmgr->BeginShapesBatching(-1);
  d3d_model_primitive_begin(d3dmgr->GetShapesModel(), kind);
}

void draw_primitive_begin_texture(int kind, int texId)
{
  d3dmgr->BeginShapesBatching(texId);
  d3d_model_primitive_begin(d3dmgr->GetShapesModel(), kind);
}

void draw_primitive_end()
{
  d3d_model_primitive_end(d3dmgr->GetShapesModel());
}

void draw_vertex(gs_scalar x, gs_scalar y)
{
  d3d_model_vertex_color(d3dmgr->GetShapesModel(), x, y, draw_get_color(), draw_get_alpha());
}

void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
  d3d_model_vertex_color(d3dmgr->GetShapesModel(), x, y, col, alpha);
}

void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(d3dmgr->GetShapesModel(), x, y, tx, ty, draw_get_color(), draw_get_alpha());
}

void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
  d3d_model_vertex_texture_color(d3dmgr->GetShapesModel(), x, y, tx, ty, col, alpha);
}

void d3d_primitive_begin(int kind)
{
  d3dmgr->BeginShapesBatching(-1);
  d3d_model_primitive_begin(d3dmgr->GetShapesModel(), kind);
}

void d3d_primitive_begin_texture(int kind, int texId)
{
  d3dmgr->BeginShapesBatching(texId);
  d3d_model_primitive_begin(d3dmgr->GetShapesModel(), kind);
}

void d3d_primitive_end()
{
  d3d_model_primitive_end(d3dmgr->GetShapesModel());
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z)
{
  d3d_model_vertex_color(d3dmgr->GetShapesModel(), x, y, z, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha)
{
  d3d_model_vertex_color(d3dmgr->GetShapesModel(), x, y, z, color, alpha);
}

void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_texture_color(d3dmgr->GetShapesModel(), x, y, z, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_texture_color(d3dmgr->GetShapesModel(), x, y, z, tx, ty, color, alpha);
}

void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  d3d_model_vertex_normal_color(d3dmgr->GetShapesModel(), x, y, z, nx, ny, nz, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha)
{
  d3d_model_vertex_normal_color(d3dmgr->GetShapesModel(), x, y, z, nx, ny, nz, color, alpha);
}

void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  d3d_model_vertex_normal_texture_color(d3dmgr->GetShapesModel(), x, y, z, nx, ny, nz, tx, ty, draw_get_color(), draw_get_alpha());
}

void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  d3d_model_vertex_normal_texture_color(d3dmgr->GetShapesModel(), x, y, z, nx, ny, nz, tx, ty, color, alpha);
}

}

