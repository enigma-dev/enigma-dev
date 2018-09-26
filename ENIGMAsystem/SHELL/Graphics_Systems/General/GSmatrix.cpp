/** Copyright (C) 2013-2014, 2018 Robert Colton
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
#include "GSmatrix_impl.h"
#include "GSmatrix.h"
#include "GSd3d.h"

#include "Universal_System/scalar.h"

#include <glm/gtc/matrix_transform.hpp>

#include <stack>

namespace {

std::stack<glm::mat4> trans_stack;

} // namespace anonymous

namespace enigma {
  bool d3dPerspective = false;
  glm::mat4 world = glm::mat4(1.0f),
            view  = glm::mat4(1.0f),
            projection = glm::mat4(1.0f);
} // namespace enigma

namespace enigma_user
{

void d3d_set_perspective(bool enable) {
  // in GM8.1 and GMS v1.4 this does not take effect
  // until the next frame in screen_redraw
  enigma::d3dPerspective = enable;
}

bool d3d_get_perspective() {
  return enigma::d3dPerspective;
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                        gs_scalar xto, gs_scalar yto, gs_scalar zto,
                        gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
  enigma::view = glm::lookAt(glm::vec3(xfrom, yfrom, zfrom), glm::vec3(xto, yto, zto), glm::vec3(xup, yup, zup));
  enigma::graphics_set_matrix(matrix_view);
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                            gs_scalar xto, gs_scalar yto, gs_scalar zto,
                            gs_scalar xup, gs_scalar yup, gs_scalar zup,
                            gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
  enigma::view = glm::lookAt(glm::vec3(xfrom, yfrom, zfrom), glm::vec3(xto, yto, zto), glm::vec3(xup, yup, zup));
  enigma::projection = glm::perspective((float)gs_angle_to_radians(angle), (float)(aspect), znear, zfar);
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  enigma::view = glm::ortho(x, x + width, y + height, y, 1.0f, 16000.0f);
  enigma::projection = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  enigma::view = glm::ortho(x, x + width, y + height, y, 1.0f, 16000.0f);
  enigma::projection = glm::perspective((float)gs_angle_to_radians(angle), width/height, 1.0f, 16000.0f);
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

void d3d_transform_set_identity()
{
  enigma::world = glm::mat4(1.0f);
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  enigma::world = glm::translate(glm::mat4(1.0f), glm::vec3(xt, yt, zt)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
  enigma::world = glm::scale(enigma::world, glm::vec3(xs, ys, zs));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_x(gs_scalar angle)
{
  enigma::world = glm::rotate(enigma::world, (float)gs_angle_to_radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_y(gs_scalar angle)
{
  enigma::world = glm::rotate(enigma::world, (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_z(gs_scalar angle)
{
  enigma::world = glm::rotate(enigma::world, (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  enigma::world = glm::rotate(enigma::world, (float)gs_angle_to_radians(angle), glm::vec3(x, y, z));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  enigma::world = glm::translate(glm::mat4(1.0f), glm::vec3(xt, yt, zt));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
  enigma::world = glm::scale(glm::mat4(1.0f), glm::vec3(xs, ys, zs));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_x(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_y(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_z(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(x, y, z));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_stack_clear()
{
  while (!trans_stack.empty()) {
    trans_stack.pop();
  }
}

bool d3d_transform_stack_empty()
{
  return trans_stack.empty();
}

bool d3d_transform_stack_push()
{
  trans_stack.emplace(enigma::world);
  return true;
}

bool d3d_transform_stack_top()
{
  if (trans_stack.empty()) return false;
  enigma::world = trans_stack.top();
  enigma::graphics_set_matrix(matrix_world);
  return true;
}

bool d3d_transform_stack_pop()
{
  if (trans_stack.empty()) return false;
  enigma::world = trans_stack.top();
  enigma::graphics_set_matrix(matrix_world);
  trans_stack.pop();
  return true;
}

bool d3d_transform_stack_discard()
{
  if (trans_stack.empty()) return false;
  trans_stack.pop();
  return true;
}

} // namespace enigma_user
