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
#include "GSmatrix_impl.h"
#include "GSmatrix.h"
#include "GSd3d.h"

#include "Widget_Systems/widgets_mandatory.h" // for show_error
#include "Universal_System/scalar.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>

namespace {

// used for classic transform stack
std::stack<glm::mat4> trans_stack;
// used for ENIGMA projection stack
// first is view matrix second is projection matrix
std::stack<std::pair<glm::mat4, glm::mat4> > proj_stack;
// used for newer matrix stack functions
std::stack<glm::mat4> matrix_stack;

inline glm::mat4 matrix_rotation(gs_scalar x, gs_scalar y, gs_scalar z) {
  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-x), glm::vec3(1.0f, 0.0f, 0.0f));
  rotation = glm::rotate(rotation, (float)gs_angle_to_radians(-y), glm::vec3(0.0f, 1.0f, 0.0f));
  rotation = glm::rotate(rotation, (float)gs_angle_to_radians(-z), glm::vec3(0.0f, 0.0f, 1.0f));
  return rotation;
}

inline var matrix_vararray(const glm::mat4 &mat) {
  var pm;
  pm(16) = 0;
  pm(0) = mat[0][0], pm(4) = mat[0][1], pm(8)  = mat[0][2], pm(12) = mat[0][3],
  pm(1) = mat[1][0], pm(5) = mat[1][1], pm(9)  = mat[1][2], pm(13) = mat[1][3],
  pm(2) = mat[2][0], pm(6) = mat[2][1], pm(10) = mat[2][2], pm(14) = mat[2][3],
  pm(3) = mat[3][0], pm(7) = mat[3][1], pm(11) = mat[3][2], pm(15) = mat[3][3];
  return pm;
}

inline var vec3_vararray(const glm::vec3 &vector) {
  var pm;
  pm(3) = 0;
  pm(0) = vector.x, pm(1) = vector.y, pm(2) = vector.z;
  return pm;
}

template<typename T>
inline void stack_clear(std::stack<T> &stack) {
  while (!stack.empty()) {
    stack.pop();
  }
}

} // namespace anonymous

namespace enigma {

bool d3dPerspective = false;
glm::mat4 world = glm::mat4(1.0f),
          view  = glm::mat4(1.0f),
          projection = glm::mat4(1.0f);

} // namespace enigma

namespace enigma_user
{

bool is_matrix(const var& value) {
  return (value.array_len() == 16);
}

bool is_vec3(const var& value) {
  return (value.array_len() == 3);
}

bool is_vec4(const var& value) {
  return (value.array_len() == 4);
}

var matrix_get(int type) {
  switch (type) {
    case matrix_world:
      return matrix_vararray(enigma::world);
    case matrix_view:
      return matrix_vararray(enigma::view);
    case matrix_projection:
      return matrix_vararray(enigma::projection);
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type: " + std::to_string(type), false);
      #endif
      break;
  }
  return matrix_vararray(glm::mat4(1.0f));
}

void matrix_set(int type, const var& matrix) {
  glm::mat4 glm_matrix = glm::make_mat4((gs_scalar*)matrix.values);
  switch (type) {
    case matrix_world:
      enigma::world = glm_matrix;
      break;
    case matrix_view:
      enigma::view = glm_matrix;
      break;
    case matrix_projection:
      enigma::projection = glm_matrix;
      break;
    default:
      #ifdef DEBUG_MODE
      show_error("Unknown matrix type: " + std::to_string(type), false);
      #endif
      break;
  }
  enigma::graphics_set_matrix(type);
}

var matrix_multiply(const var& matrix1, const var& matrix2) {
  glm::mat4 glm_matrix1 = glm::make_mat4((gs_scalar*)matrix1.values),
            glm_matrix2 = glm::make_mat4((gs_scalar*)matrix2.values);
  return matrix_vararray(glm_matrix1 * glm_matrix2);
}

var matrix_transform_vertex(const var& matrix, gs_scalar x, gs_scalar y, gs_scalar z) {
  glm::mat4 glm_matrix = glm::make_mat4((gs_scalar*)matrix.values);
  glm::vec4 vertex(x, y, z, 0);
  vertex = glm_matrix * vertex;
  return vec3_vararray(glm::vec3(vertex));
}

var matrix_build(gs_scalar x, gs_scalar y, gs_scalar z,
                 gs_scalar xrotation, gs_scalar yrotation, gs_scalar zrotation,
                 gs_scalar xscale, gs_scalar yscale, gs_scalar zscale) {
  glm::mat4 matrix = matrix_rotation(xrotation, yrotation, zrotation);
  matrix = glm::scale(matrix, glm::vec3(xscale, yscale, zscale));
  matrix = glm::translate(matrix, glm::vec3(x, y, z));
  return matrix_vararray(matrix);
}

var matrix_build_identity() {
  return matrix_vararray(glm::mat4(1.0f));
}

var matrix_build_lookat(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                        gs_scalar xto, gs_scalar yto, gs_scalar zto,
                        gs_scalar xup, gs_scalar yup, gs_scalar zup) {
  return matrix_vararray(glm::lookAt(glm::vec3(xfrom, yfrom, zfrom), glm::vec3(xto, yto, zto), glm::vec3(xup, yup, zup)));
}

var matrix_build_projection_ortho(gs_scalar width, gs_scalar height, gs_scalar znear, gs_scalar zfar) {
  return matrix_vararray(glm::ortho(0.0f, width, height, 0.0f, znear, zfar));
}

var matrix_build_projection_perspective(gs_scalar width, gs_scalar height, gs_scalar znear, gs_scalar zfar) {
  return matrix_vararray(glm::perspective((float)gs_angle_to_radians(40), width/height, znear, zfar));
}

var matrix_build_projection_perspective_fov(gs_scalar fov_y, gs_scalar aspect, gs_scalar znear, gs_scalar zfar) {
  return matrix_vararray(glm::perspective((float)gs_angle_to_radians(fov_y), aspect, znear, zfar));
}

bool matrix_stack_is_empty() {
  return matrix_stack.empty();
}

void matrix_stack_clear() {
  stack_clear(matrix_stack);
}

void matrix_stack_set(const var& matrix) {
  matrix_stack.pop();
  glm::mat4 glm_matrix = glm::make_mat4((gs_scalar*)matrix.values);
  matrix_stack.emplace(glm_matrix);
}

void matrix_stack_push(const var& matrix) {
  glm::mat4 glm_matrix = glm::make_mat4((gs_scalar*)matrix.values);
  matrix_stack.emplace(glm_matrix);
}

void matrix_stack_pop() {
  matrix_stack.pop();
}

var matrix_stack_top() {
  return matrix_vararray(matrix_stack.top());
}

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
  enigma::view = glm::ortho(x, x + width, y + height, y, -32000.0f, 32000.0f);
  enigma::projection = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

void d3d_set_projection_ortho_lookat(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle,
                                     gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,
                                     gs_scalar xto, gs_scalar yto, gs_scalar zto,
                                     gs_scalar xup, gs_scalar yup, gs_scalar zup) {
  enigma::view = glm::lookAt(glm::vec3(xfrom, yfrom, zfrom), glm::vec3(xto, yto, zto), glm::vec3(xup, yup, zup));
  enigma::projection = glm::ortho(x, x + width, y + height, y, -32000.0f, 32000.0f);
  enigma::projection = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) * enigma::projection;
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  enigma::view = glm::ortho(x, x + width, y + height, y, 1.0f, 32000.0f);
  enigma::projection = glm::perspective((float)gs_angle_to_radians(40.0f), width/height, 1.0f, 32000.0f);
  enigma::projection = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(angle), glm::vec3(0.0f, 0.0f, 1.0f)) * enigma::projection;
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
}

var d3d_transform_vertex(gs_scalar x, gs_scalar y, gs_scalar z) {
  glm::vec4 vertex(x, y, z, 0);
  vertex = enigma::world * vertex;
  return vec3_vararray(glm::vec3(vertex));
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
  enigma::world = glm::scale(glm::mat4(1.0f), glm::vec3(xs, ys, zs)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_x(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_y(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_z(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(x, y, z)) * enigma::world;
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_add_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
{
  glm::mat4 rotation = matrix_rotation(x, y, z);
  enigma::world = rotation * enigma::world;
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
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(1.0f, 0.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_y(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_z(gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(0.0f, 0.0f, 1.0f));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  enigma::world = glm::rotate(glm::mat4(1.0f), (float)gs_angle_to_radians(-angle), glm::vec3(x, y, z));
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_set_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
{
  enigma::world = matrix_rotation(x, y, z);
  enigma::graphics_set_matrix(matrix_world);
}

void d3d_transform_stack_clear()
{
  stack_clear(trans_stack);
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

bool d3d_projection_stack_push() {
  proj_stack.emplace(enigma::view, enigma::projection);
  return true;
}

bool d3d_projection_stack_pop() {
  if (proj_stack.empty()) return false;
  enigma::view = proj_stack.top().first;
  enigma::projection = proj_stack.top().second;
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
  proj_stack.pop();
  return true;
}

void d3d_projection_stack_clear() {
  stack_clear(proj_stack);
}

bool d3d_projection_stack_empty() {
  return proj_stack.empty();
}

bool d3d_projection_stack_top() {
  if (proj_stack.empty()) return false;
  enigma::view = proj_stack.top().first;
  enigma::projection = proj_stack.top().second;
  enigma::graphics_set_matrix(matrix_view);
  enigma::graphics_set_matrix(matrix_projection);
  return true;
}

bool d3d_projection_stack_discard() {
  if (proj_stack.empty()) return false;
  proj_stack.pop();
  return true;
}


} // namespace enigma_user
