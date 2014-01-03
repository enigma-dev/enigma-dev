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

#ifndef ENIGMA_GSMATRIX_H
#define ENIGMA_GSMATRIX_H 

#include "Universal_System/scalar.h"
#include <string>

extern double projection_matrix[16];
extern double transformation_matrix[16];

namespace enigma_user
{

enum {
	matrix_view,
	matrix_projection,
	matrix_world
};

void d3d_set_perspective(bool enable);
void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup);
void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, double angle, double aspect, double znear, double zfar);
void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle);
void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle);

void d3d_transform_set_identity();
void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_add_rotation_x(double angle);
void d3d_transform_add_rotation_y(double angle);
void d3d_transform_add_rotation_z(double angle);
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle);
void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_set_rotation_x(double angle);
void d3d_transform_set_rotation_y(double angle);
void d3d_transform_set_rotation_z(double angle);
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle);
bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_disgard();

gs_scalar* d3d_transform_vertex(gs_scalar x, gs_scalar y, gs_scalar z);

gs_scalar* matrix_get(int type);
void matrix_set(int type, gs_scalar* matrix);
gs_scalar* matrix_build(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar xrotation, gs_scalar yrotation, gs_scalar zrotation, gs_scalar xscale, gs_scalar yscale, gs_scalar zscale);
gs_scalar* matrix_multiply(gs_scalar* matrix1, gs_scalar* matrix2);

}

#endif // ENIGMA_GL3D3D_H 
