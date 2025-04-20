/** Copyright (C) 2008-2012 Josh Ventura
*** Copyright (C) 2013-2014 Robert Colton, Harijs Grinbergs
*** Copyright (C) 2015 Harijs Grinbergs
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

#ifndef ENIGMA_GSMATRIX_H
#define ENIGMA_GSMATRIX_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"
#include <string>

//TODO: Transformation functions should probably not use gs_scalar as the angular type but implement their own scalar to avoid
//losing precision with math functions.

namespace enigma_user
{

enum {
	matrix_view,
	matrix_projection,
	matrix_world
};

var matrix_get(int type);
void matrix_set(int type, const var& matrix);
var matrix_multiply(const var& matrix1, const var& matrix2);
var matrix_transform_vertex(const var& matrix, gs_scalar x, gs_scalar y, gs_scalar z);

var matrix_build(gs_scalar x, gs_scalar y, gs_scalar z,
                 gs_scalar xrotation, gs_scalar yrotation, gs_scalar zrotation,
                 gs_scalar xscale, gs_scalar yscale, gs_scalar zscale);
var matrix_build_identity();
var matrix_build_lookat(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                        gs_scalar xto, gs_scalar yto, gs_scalar zto,
                        gs_scalar xup, gs_scalar yup, gs_scalar zup);
var matrix_build_projection_ortho(gs_scalar width, gs_scalar height, gs_scalar znear, gs_scalar zfar);
var matrix_build_projection_perspective(gs_scalar width, gs_scalar height, gs_scalar znear, gs_scalar zfar);
var matrix_build_projection_perspective_fov(gs_scalar fov_y, gs_scalar aspect, gs_scalar znear, gs_scalar zfar);

bool matrix_stack_is_empty();
void matrix_stack_clear();
void matrix_stack_set(const var& matrix);
void matrix_stack_push(const var& matrix);
void matrix_stack_pop();
var matrix_stack_top();

bool is_matrix(const var& value);
bool is_vec3(const var& value);
bool is_vec4(const var& value);

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                        gs_scalar xto, gs_scalar yto, gs_scalar zto,
                        gs_scalar xup, gs_scalar yup, gs_scalar zup);
void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                            gs_scalar xto, gs_scalar yto, gs_scalar zto,
                            gs_scalar xup, gs_scalar yup, gs_scalar zup,
                            gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar);
void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);
void d3d_set_projection_ortho_lookat(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle,
                                     gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                                     gs_scalar xto, gs_scalar yto, gs_scalar zto,
                                     gs_scalar xup, gs_scalar yup, gs_scalar zup);
void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle);

var d3d_transform_vertex(gs_scalar x, gs_scalar y, gs_scalar z);

void d3d_transform_set_identity();
void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_add_rotation_x(gs_scalar angle);
void d3d_transform_add_rotation_y(gs_scalar angle);
void d3d_transform_add_rotation_z(gs_scalar angle);
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle);
void d3d_transform_add_rotation(gs_scalar x, gs_scalar y, gs_scalar z);

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt);
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs);
void d3d_transform_set_rotation_x(gs_scalar angle);
void d3d_transform_set_rotation_y(gs_scalar angle);
void d3d_transform_set_rotation_z(gs_scalar angle);
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle);
void d3d_transform_set_rotation(gs_scalar x, gs_scalar y, gs_scalar z);

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

}

#endif // ENIGMA_GSMATRIX_H
