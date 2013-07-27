/** Copyright (C) 2008-2013 Josh Ventura
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

const int pr_pointlist      = 1;     //GL_POINTS
const int pr_linelist       = 2;     //GL_LINES
const int pr_linestrip      = 3;     //GL_LINE_STRIP
const int pr_trianglelist   = 4;     //GL_TRIANGLES
const int pr_trianglestrip  = 5;     //GL_TRIANGLE_STRIP
const int pr_trianglefan    = 6;     //GL_TRIANGLE_FAN

const int pr_lineloop       = 7;     //GL_LINE_LOOP
const int pr_quadlist       = 8;     //GL_QUADS
const int pr_quadstrip      = 9;     //GL_QUAD_STRIP
const int pr_polygon        = 10;     //GL_POLYGON

const int pr_aa_nicest      = 4354;  //GL_NICEST
const int pr_aa_fastest     = 4353;  //GL_FASTEST
const int pr_aa_dontcare    = 4352;  //GL_DONT_CARE

void draw_set_primitive_aa(bool enable, int quality);
int draw_primitive_begin(int kind);
int draw_primitive_begin_texture(int kind, unsigned tex);
int draw_vertex(gs_scalar x, gs_scalar y);
int draw_vertex_color(gs_scalar x, gs_scalar y, int color, float alpha);
int draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty);
int draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha);

int draw_primitive_end();
