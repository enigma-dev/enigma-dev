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

#include "DirectX10Headers.h"
#include "DX10primitives.h"
#include "binding.h"

#include <string>
#include "Widget_Systems/widgets_mandatory.h"

#if PRIMBUFFER
enum __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

enum ptypes_by_id
{
	
};

void draw_set_primitive_aa(bool enable, int quality)
{

}

int draw_primitive_begin(int dink)
{

}

int draw_primitive_begin_texture(int dink,unsigned tex)
{

}

int draw_vertex(double x, double y)
{

}

int draw_vertex_color(float x, float y, int col, float alpha)
{

}

int draw_vertex_texture(float x, float y, float tx, float ty)
{

}

int draw_vertex_texture_color(float x, float y, float tx, float ty, int col, float alpha)
{

}

int draw_primitive_end()
{

}

void d3d_primitive_begin(int kind)
{

}
void d3d_primitive_begin_texture(int kind, int texId)
{

}

void d3d_primitive_end()
{
}

void d3d_vertex(double x, double y, double z)
{

}
void d3d_vertex_color(double x, double y, double z, int color, double alpha)
{

}
void d3d_vertex_texture(double x, double y, double z, double tx, double ty)
{

}
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha)
{

}

void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz)
{

}
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha)
{

}
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{

}
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha)
{

}
