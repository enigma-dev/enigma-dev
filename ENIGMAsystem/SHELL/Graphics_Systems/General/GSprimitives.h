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

#include "Universal_System/scalar.h"

namespace enigma_user
{
enum {
  pr_pointlist      = 1,     // GL_POINTS            D3DPT_POINTLIST
  pr_linelist       = 2,     // GL_LINES             D3DPT_LINELIST
  pr_linestrip      = 3,     // GL_LINE_STRIP        D3DPT_LINESTRIP
  pr_trianglelist   = 4,     // GL_TRIANGLES         D3DPT_TRIANGLELIST
  pr_trianglestrip  = 5,     // GL_TRIANGLE_STRIP    D3DPT_TRIANGLESTRIP
  pr_trianglefan    = 6      // GL_TRIANGLE_FAN      D3DPT_TRIANGLEFAN
};

int draw_primitive_begin(int kind);
int draw_primitive_begin_texture(int kind,unsigned tex);
int draw_vertex(gs_scalar x, gs_scalar y);
int draw_vertex_color(gs_scalar x, gs_scalar y, int color, float alpha);
int draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty);
int draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha);
int draw_primitive_end();
void d3d_primitive_begin(int kind);
void d3d_primitive_begin_texture(int kind, int texId);
void d3d_primitive_end();
void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z);
void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha);
void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty);
void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha);
void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz);
void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha);
void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty);
void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha);

}

