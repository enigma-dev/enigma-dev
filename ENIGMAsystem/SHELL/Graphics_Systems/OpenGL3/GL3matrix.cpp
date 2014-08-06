/** Copyright (C) 2008-2012 Josh Ventura, DatZach, Polygone
*** Copyright (C) 2013-2014 Robert B. Colton, Polygone, Harijs Grinbergs
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

#include "../General/OpenGLHeaders.h"
#include "../General/GSd3d.h"
#include "../General/GSmatrix.h"
#include "../General/GSmath.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Bridges/General/GL3Context.h"
#include <math.h>

//using namespace std;

#include <floatcomp.h>

namespace enigma
{
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
            enigma::Matrix4 tmpNorm = enigma::mv_matrix.Transpose().Inverse();
            enigma::normal_matrix = enigma::Matrix3(tmpNorm(0,0),tmpNorm(0,1),tmpNorm(0,2),
                                                    tmpNorm(1,0),tmpNorm(1,1),tmpNorm(1,2),
                                                    tmpNorm(2,0),tmpNorm(2,1),tmpNorm(2,2));
            enigma::transform_needs_update = false;
        }
    }
}

//NOTE: THIS IS STILL FFP
#ifdef GS_SCALAR_64
#define glLoadMatrix(m) glLoadMatrixd((gs_scalar*)m.Transpose());
#define glGet(m,n)        glGetDoublev(m,(gs_scalar*)n); //For debug

#else
#define glLoadMatrix(m) glLoadMatrixf((gs_scalar*)m.Transpose());
#define glGet(m,n)        glGetFloatv(m,(gs_scalar*)n); //For debug

#endif

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{
    oglmgr->Transformation();
    if (enable) {
      enigma::projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
    } else {
      //projection_matrix.InitPersProjTransform(0, 1, 0, 1); //they cannot be zeroes!
    }
    enigma::transform_needs_update = true;
  // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
  // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
  // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
    oglmgr->Transformation();
    (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
    enigma::projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
    enigma::view_matrix.InitCameraTransform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
    enigma::transform_needs_update = true;
    //enigma::d3d_light_update_positions();
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
    if (angle == 0 || znear == 0) return; //THEY CANNOT BE 0!!!
    oglmgr->Transformation();
    (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);

    enigma::projection_matrix.InitPersProjTransform(angle, -aspect, znear, zfar);

    enigma::view_matrix.InitCameraTransform(enigma::Vector3(xfrom,yfrom,zfrom),enigma::Vector3(xto,yto,zto),enigma::Vector3(xup,yup,zup));
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
      enigma::projection_matrix.InitTranslationTransform(-x-width/2.0, -y-height/2.0, 0);
      enigma::projection_matrix.rotateZ(-angle);
      enigma::projection_matrix.translate(x+width/2.0, y+height/2.0, 0);
    }else{
      enigma::projection_matrix.InitIdentity();
    }

    enigma::Matrix4 ortho;
    ortho.InitOrthoProjTransform(x,x + width,y + height,y,32000,-32000);

    enigma::projection_matrix = ortho * enigma::projection_matrix;
    enigma::view_matrix.InitIdentity();
    enigma::transform_needs_update = true;
    //enigma::d3d_light_update_positions();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::projection_matrix.InitRotateZTransform(angle);

    enigma::Matrix4 persp, ortho;
    persp.InitPersProjTransform(60, 1, 0.1,32000);
    ortho.InitOrthoProjTransform(x,x + width,y,y + height,0.1,32000);

    enigma::projection_matrix = enigma::projection_matrix * persp * ortho;
    enigma::transform_needs_update = true;
    //enigma::d3d_light_update_positions();
}

void d3d_transform_set_identity()
{
    oglmgr->Transformation();
    enigma::model_matrix.InitIdentity();
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
    enigma::model_matrix.rotateX(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_add_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.rotateY(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_add_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.rotateZ(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.rotate(-angle,x,y,z);
    enigma::transform_needs_update = true;
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitTranslationTransform(xt, yt, zt);
    enigma::transform_needs_update = true;
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitScaleTransform(xs, ys, zs);
    enigma::transform_needs_update = true;
}
void d3d_transform_set_rotation_x(gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitRotateXTransform(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_set_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitRotateYTransform(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_set_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitRotateZTransform(-angle);
    enigma::transform_needs_update = true;
}
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    enigma::model_matrix.InitIdentity();
    enigma::model_matrix.rotate(-angle, x, y, z);
    enigma::transform_needs_update = true;
}

}

#include <stack>
std::stack<enigma::Matrix4> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{
    //if (trans_stack_size == 31) return false; //This limit no longer applies
    oglmgr->Transformation();
    trans_stack.push(enigma::model_matrix);
    trans_stack_size++;
    return true;
}

bool d3d_transform_stack_pop()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    enigma::model_matrix = trans_stack.top();
    trans_stack.pop();
    if (trans_stack_size > 0) trans_stack_size--;
    enigma::transform_needs_update = true;
    return true;
}

void d3d_transform_stack_clear()
{
    oglmgr->Transformation();
    do
      trans_stack.pop();
    while (trans_stack_size--);
    enigma::model_matrix.InitIdentity();
    enigma::transform_needs_update = true;
}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    enigma::model_matrix = trans_stack.top();
    enigma::transform_needs_update = true;
    return true;
}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.pop();
    trans_stack_size--;
    return true;
}

}
