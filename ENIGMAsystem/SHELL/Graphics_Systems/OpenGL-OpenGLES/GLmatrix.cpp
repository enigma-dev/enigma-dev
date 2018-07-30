/** Copyright (C) 2008-2012 Josh Ventura, DatZach, Polygone
*** Copyright (C) 2013-2015 Robert B. Colton, Polygone, Harijs Grinbergs
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
#include "OpenGLHeaders.h"
#include "Graphics_Systems/General/GSd3d.h"
#include "Graphics_Systems/General/GSmatrix.h"
#include "Graphics_Systems/General/GSmath.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

#include <math.h>

#define M_PI    3.14159265358979323846

//using namespace std;

#include <floatcomp.h>

namespace enigma
{
    #define degtorad(x) x*(M_PI/180.0)

    //These are going to be modified by the user via functions
    enigma::Matrix4 projection_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), view_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), model_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    //These are just combinations for use in shaders
    enigma::Matrix4 mv_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), mvp_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

    enigma::Matrix3 normal_matrix(1,0,0,0,1,0,0,0,1);

    bool transform_needs_update = false;

    void transformation_update(){
        if (enigma::transform_needs_update == true){
            //Recalculate matrices
            enigma::mv_matrix = enigma::view_matrix * enigma::model_matrix;
            enigma::mvp_matrix = enigma::projection_matrix * enigma::mv_matrix;

            //normal_matrix = invert(transpose(mv_submatrix)), where mv_submatrix is modelview top-left 3x3 matrix
            enigma::Matrix3 tmpNorm(mv_matrix(0,0),mv_matrix(0,1),mv_matrix(0,2),
                                    mv_matrix(1,0),mv_matrix(1,1),mv_matrix(1,2),
                                    mv_matrix(2,0),mv_matrix(2,1),mv_matrix(2,2));

            enigma::normal_matrix = tmpNorm.inverse().transpose();

            enigma::transform_needs_update = false;
        }
    }
}

namespace enigma_user
{
  void d3d_set_perspective(bool enable)
  {
      oglmgr->Transformation();
      if (enable) {
        enigma::projection_matrix.init_perspective_proj_transform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
      } else {
        //projection_matrix.init_perspective_proj_transform(0, 1, 0, 1); //they cannot be zeroes!
      }
      enigma::transform_needs_update = true;
    // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
    // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
    // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
  }

  void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
  {
      oglmgr->Transformation();
     // (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
      enigma::projection_matrix.init_perspective_proj_transform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
      enigma::view_matrix.init_camera_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
      enigma::transform_needs_update = true;
      //enigma::d3d_light_update_positions();
  }

  void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
  {
      if (angle == 0 || znear == 0) return; //THEY CANNOT BE 0!!!
      oglmgr->Transformation();
      //(enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);

      enigma::projection_matrix.init_perspective_proj_transform(angle, -aspect, znear, zfar);

      enigma::view_matrix.init_camera_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
      enigma::transform_needs_update = true;
      //enigma::d3d_light_update_positions();
  }

  void d3d_set_projection_ortho_lookat(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
  {
      // This fixes font glyph edge artifacting and vertical scroll gaps
      // seen by mostly NVIDIA GPU users.  Rounds x and y and adds +0.01 offset.
      // This will prevent the fix from being negated through moving projections
      // and fractional coordinates.
      x = round(x) + 0.01f; y = round(y) + 0.01f;

      oglmgr->Transformation();
      enigma::projection_matrix.init_identity();

      enigma::Matrix4 ortho;
      ortho.init_ortho_proj_transform(x,x + width,y,y + height,32000,-32000);

      enigma::projection_matrix = ortho * enigma::projection_matrix;
      enigma::view_matrix.init_camera_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
      enigma::transform_needs_update = true;
      //enigma::d3d_light_update_positions();
  }

  void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
  {
      // This fixes font glyph edge artifacting and vertical scroll gaps
      // seen by mostly NVIDIA GPU users.  Rounds x and y and adds +0.01 offset.
      // This will prevent the fix from being negated through moving projections
      // and fractional coordinates.
      x = round(x) + 0.01f; y = round(y) + 0.01f;
      oglmgr->Transformation();
      if (angle!=0){
        enigma::projection_matrix.init_translation_transform(-x-width/2.0, -y-height/2.0, 0);
        enigma::projection_matrix.rotate_z(degtorad(-angle));
        enigma::projection_matrix.translate(x+width/2.0, y+height/2.0, 0);
      }else{
        enigma::projection_matrix.init_identity();
      }

      enigma::Matrix4 ortho;
      ortho.init_ortho_proj_transform(x,x + width,y + height,y,32000,-32000);

      enigma::projection_matrix = ortho * enigma::projection_matrix;
      enigma::view_matrix.init_identity();
      enigma::transform_needs_update = true;
      //enigma::d3d_light_update_positions();
  }

  void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::projection_matrix.init_rotate_z_transform(angle);

      enigma::Matrix4 persp, ortho;
      persp.init_perspective_proj_transform(60, 1, 0.1,32000);
      ortho.init_ortho_proj_transform(x,x + width,y,y + height,0.1,32000);

      enigma::projection_matrix = enigma::projection_matrix * persp * ortho;
      enigma::transform_needs_update = true;
      //enigma::d3d_light_update_positions();
  }

  void d3d_transform_set_identity()
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_identity();
      enigma::transform_needs_update = true;
  }

  void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
  {
      oglmgr->Transformation();
      enigma::model_matrix.translate(xt, yt, zt);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
  {
      oglmgr->Transformation();
      enigma::model_matrix.scale(xs, ys, zs);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_rotation_x(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.rotate_x(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_rotation_y(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.rotate_y(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_rotation_z(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.rotate_z(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.rotate(degtorad(-angle),x,y,z);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
  {
      oglmgr->Transformation();
      enigma::model_matrix.rotate(x,y,z);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
  {
      oglmgr->Transformation();
      enigma::Matrix4 m;
      m.init_look_at_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
      enigma::model_matrix = m*enigma::model_matrix;
      enigma::transform_needs_update = true;
  }

  void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_translation_transform(xt, yt, zt);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_scale_transform(xs, ys, zs);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_rotation_x(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_rotate_x_transform(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_rotation_y(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_rotate_y_transform(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_rotation_z(gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_rotate_z_transform(degtorad(-angle));
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_identity();
      enigma::model_matrix.rotate(degtorad(-angle), x, y, z);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_rotation(gs_scalar x, gs_scalar y, gs_scalar z)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_identity();
      enigma::model_matrix.rotate(x,y,z);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_set_look_at(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom,gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
  {
      oglmgr->Transformation();
      enigma::model_matrix.init_look_at_transform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
      enigma::transform_needs_update = true;
  }

  void d3d_transform_set_array(const gs_scalar *matrix)
  {
      oglmgr->Transformation();
      enigma::model_matrix = enigma::Matrix4(matrix);
      enigma::transform_needs_update = true;
  }
  void d3d_transform_add_array(const gs_scalar *matrix)
  {
      oglmgr->Transformation();
      enigma::Matrix4 m(matrix);
      enigma::model_matrix = m*enigma::model_matrix;
     enigma::transform_needs_update = true;
  }

  gs_scalar * d3d_transform_get_array_pointer(){
      return enigma::model_matrix;
  }

  var d3d_transform_get_array(){
      enigma::transformation_update();
      var mm;
      mm(4,4) = 0;
      mm(0,0) = enigma::model_matrix(0,0), mm(0,1) = enigma::model_matrix(0,1), mm(0,2) = enigma::model_matrix(0,2), mm(0,3) = enigma::model_matrix(0,3),
      mm(1,0) = enigma::model_matrix(1,0), mm(1,1) = enigma::model_matrix(1,1), mm(1,2) = enigma::model_matrix(1,2), mm(1,3) = enigma::model_matrix(1,3),
      mm(2,0) = enigma::model_matrix(2,0), mm(2,1) = enigma::model_matrix(2,1), mm(2,2) = enigma::model_matrix(2,2), mm(2,3) = enigma::model_matrix(2,3),
      mm(3,0) = enigma::model_matrix(3,0), mm(3,1) = enigma::model_matrix(3,1), mm(3,2) = enigma::model_matrix(3,2), mm(3,3) = enigma::model_matrix(3,3);
      return mm;
  }

  void d3d_transform_force_update(){
      oglmgr->Transformation();
      enigma::transformation_update();
  }

  void d3d_projection_set_array(const gs_scalar *matrix)
  {
      oglmgr->Transformation();
      enigma::projection_matrix = enigma::Matrix4(matrix);
      enigma::view_matrix.init_identity();
      enigma::transform_needs_update = true;
  }

  void d3d_projection_add_array(const gs_scalar *matrix)
  {
      oglmgr->Transformation();
      enigma::Matrix4 m(matrix);
      enigma::projection_matrix = m*enigma::projection_matrix;
      enigma::view_matrix.init_identity();
      enigma::transform_needs_update = true;
  }

  gs_scalar * d3d_projection_get_array_pointer(){
      return enigma::projection_matrix;
  }

  var d3d_projection_get_array(){
      enigma::transformation_update();
      var pm;
      pm(4,4) = 0;
      pm(0,0) = enigma::projection_matrix(0,0), pm(0,1) = enigma::projection_matrix(0,1), pm(0,2) = enigma::projection_matrix(0,2), pm(0,3) = enigma::projection_matrix(0,3),
      pm(1,0) = enigma::projection_matrix(1,0), pm(1,1) = enigma::projection_matrix(1,1), pm(1,2) = enigma::projection_matrix(1,2), pm(1,3) = enigma::projection_matrix(1,3),
      pm(2,0) = enigma::projection_matrix(2,0), pm(2,1) = enigma::projection_matrix(2,1), pm(2,2) = enigma::projection_matrix(2,2), pm(2,3) = enigma::projection_matrix(2,3),
      pm(3,0) = enigma::projection_matrix(3,0), pm(3,1) = enigma::projection_matrix(3,1), pm(3,2) = enigma::projection_matrix(3,2), pm(3,3) = enigma::projection_matrix(3,3);
      return pm;
  }

  gs_scalar * d3d_view_get_array_pointer(){
      return enigma::view_matrix;
  }

  var d3d_view_get_array(){
      enigma::transformation_update();
      var pm;
      pm(4,4) = 0;
      pm(0,0) = enigma::view_matrix(0,0), pm(0,1) = enigma::view_matrix(0,1), pm(0,2) = enigma::view_matrix(0,2), pm(0,3) = enigma::view_matrix(0,3),
      pm(1,0) = enigma::view_matrix(1,0), pm(1,1) = enigma::view_matrix(1,1), pm(1,2) = enigma::view_matrix(1,2), pm(1,3) = enigma::view_matrix(1,3),
      pm(2,0) = enigma::view_matrix(2,0), pm(2,1) = enigma::view_matrix(2,1), pm(2,2) = enigma::view_matrix(2,2), pm(2,3) = enigma::view_matrix(2,3),
      pm(3,0) = enigma::view_matrix(3,0), pm(3,1) = enigma::view_matrix(3,1), pm(3,2) = enigma::view_matrix(3,2), pm(3,3) = enigma::view_matrix(3,3);
      return pm;
  }

  gs_scalar * d3d_transformation_get_mv(){
      enigma::transformation_update();
      return enigma::mv_matrix;
  }

  gs_scalar * d3d_transformation_get_mvp(){
      enigma::transformation_update();
      return enigma::mvp_matrix;
  }
}
  #include <stack>
  std::stack<enigma::Matrix4> trans_stack;
  std::stack<enigma::Matrix4> proj_stack;
  std::stack<enigma::Matrix4> view_stack;

namespace enigma_user
{
  bool d3d_transform_stack_push()
  {
      //if (trans_stack.size() == 31) return false; //This is a GM limitation that ENIGMA doesn't have
      oglmgr->Transformation();
      trans_stack.push(enigma::model_matrix);
      return true;
  }

  bool d3d_transform_stack_pop()
  {
      if (trans_stack.size() == 0) return false;
      oglmgr->Transformation();
      enigma::model_matrix = trans_stack.top();
      trans_stack.pop();
      enigma::transform_needs_update = true;
      return true;
  }

  void d3d_transform_stack_clear()
  {
      oglmgr->Transformation();
      trans_stack = std::stack<enigma::Matrix4>(); //Clear the stack
      enigma::model_matrix.init_identity();
      enigma::transform_needs_update = true;
  }

  bool d3d_transform_stack_empty()
  {
      return (trans_stack.size() == 0);
  }

  bool d3d_transform_stack_top()
  {
      if (trans_stack.size() == 0) return false;
      oglmgr->Transformation();
      enigma::model_matrix = trans_stack.top();
      enigma::transform_needs_update = true;
      return true;
  }

  bool d3d_transform_stack_discard()
  {
      if (proj_stack.size() == 0) return false;
      proj_stack.pop();
      return true;
  }

  bool d3d_projection_stack_push()
  {
      //if (proj_stack.size() == 31) return false; //This is a GM limitation that ENIGMA doesn't have
      oglmgr->Transformation();
      proj_stack.push(enigma::projection_matrix);
      view_stack.push(enigma::view_matrix);
      return true;
  }

  bool d3d_projection_stack_pop()
  {
      if (proj_stack.size() == 0) return false;
      oglmgr->Transformation();
      enigma::projection_matrix = proj_stack.top();
      enigma::view_matrix = view_stack.top();
      proj_stack.pop();
      view_stack.pop();
      enigma::transform_needs_update = true;
      return true;
  }

  void d3d_projection_stack_clear()
  {
      oglmgr->Transformation();
      proj_stack = std::stack<enigma::Matrix4>(); //Clear the stack
      view_stack = std::stack<enigma::Matrix4>();
      enigma::projection_matrix.init_identity();
      enigma::view_matrix.init_identity();
      enigma::transform_needs_update = true;
  }

  bool d3d_projection_stack_empty()
  {
      return (proj_stack.size() == 0);
  }

  bool d3d_projection_stack_top()
  {
      if (proj_stack.size() == 0) return false;
      oglmgr->Transformation();
      enigma::projection_matrix = proj_stack.top();
      enigma::view_matrix = view_stack.top();
      enigma::transform_needs_update = true;
      return true;
  }

  bool d3d_projection_stack_discard()
  {
      if (proj_stack.size() == 0) return false;
      proj_stack.pop();
      view_stack.pop();
      return true;
  }
}
