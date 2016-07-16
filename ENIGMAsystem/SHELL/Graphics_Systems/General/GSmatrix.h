/** Copyright (C) 2008-2012 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton, Harijs Grinbergs
*** Copyright (C) 2015 Harijs Grinbergs
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
#include "Universal_System/var4.h"
#include <string>

namespace enigma
{
  //Forward declare
  struct Matrix3;
  struct Matrix4;

  extern Matrix4 projection_matrix, view_matrix, model_matrix;
  extern Matrix4 mv_matrix, mvp_matrix;
  extern Matrix3 normal_matrix;
  extern bool transform_needs_update;
  extern void transformation_update();
}

namespace enigma_user
{

enum {
	matrix_view,
	matrix_projection,
	matrix_world
};

//TODO: Transformation functions should probably not use gs_scalar as the angular type but implement their own scalar to avoid
//losing precision with math functions.
void d3d_set_perspective(bool enable);
void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto,gs_scalar xup, gs_scalar yup, gs_scalar zup);
void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar);
void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);
void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);
void d3d_set_projection_ortho_lookat(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup);

void d3d_transform_set_identity();
void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_add_rotation_x(gs_scalar angle);
void d3d_transform_add_rotation_y(gs_scalar angle);
void d3d_transform_add_rotation_z(gs_scalar angle);
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle);
void d3d_transform_add_rotation(gs_scalar x, gs_scalar y, gs_scalar z);
void d3d_transform_add_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup);

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_set_rotation_x(gs_scalar angle);
void d3d_transform_set_rotation_y(gs_scalar angle);
void d3d_transform_set_rotation_z(gs_scalar angle);
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle);
void d3d_transform_set_rotation(gs_scalar x, gs_scalar y, gs_scalar z);
void d3d_transform_set_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup);

void d3d_transform_set_array(const gs_scalar *matrix);
void d3d_transform_add_array(const gs_scalar *matrix);
gs_scalar * d3d_transform_get_array_pointer();
var d3d_transform_get_array();
void d3d_transform_force_update();

bool d3d_transform_stack_push();
bool d3d_transform_stack_pop();
void d3d_transform_stack_clear();
bool d3d_transform_stack_empty();
bool d3d_transform_stack_top();
bool d3d_transform_stack_discard();

bool d3d_projection_stack_push();
bool d3d_projection_stack_pop();
void d3d_projection_stack_clear();
bool d3d_projection_stack_empty();
bool d3d_projection_stack_top();
bool d3d_projection_stack_discard();
void d3d_projection_set_array(const gs_scalar *matrix);
void d3d_projection_add_array(const gs_scalar *matrix);
gs_scalar * d3d_projection_get_array_pointer();
var d3d_projection_get_array();

gs_scalar * d3d_view_get_array_pointer();
var d3d_view_get_array();

gs_scalar * d3d_transformation_get_mv();
gs_scalar * d3d_transformation_get_mvp();

/*gs_scalar* d3d_transform_vertex(gs_scalar x, gs_scalar y, gs_scalar z);

gs_scalar* matrix_get(int type);
void matrix_set(int type, gs_scalar* matrix);
gs_scalar* matrix_build(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar xrotation, gs_scalar yrotation, gs_scalar zrotation, gs_scalar xscale, gs_scalar yscale, gs_scalar zscale);
gs_scalar* matrix_multiply(gs_scalar* matrix1, gs_scalar* matrix2);*/

}

#endif // ENIGMA_GSMATRIX_H
