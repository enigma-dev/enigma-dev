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
#include "../General/GStextures.h"
#include "../General/GLTextureStruct.h"
#include "../General/GSmath.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Bridges/General/GL3Context.h"
#include <math.h>

//using namespace std;

#include <floatcomp.h>

//These are going to be modified by the user via functions
enigma::Matrix4f projection_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), view_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), model_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

//These are just combinations for use in shaders
enigma::Matrix4f mv_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), mvp_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

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
      projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
    } else {
      //projection_matrix.InitPersProjTransform(0, 1, 0, 1); //they cannot be zeroes!
    }
  //glMatrixMode(GL_MODELVIEW);
  // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
  // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
  // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
    oglmgr->Transformation();
    (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
    projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
    view_matrix.InitCameraTransform(enigma::Vector3f(xfrom,yfrom,zfrom),enigma::Vector3f(xto,yto,zto),enigma::Vector3f(xup,yup,zup));
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
    if (angle == 0 || znear == 0) return; //THEY CANNOT BE 0!!!
    oglmgr->Transformation();
    (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);

    projection_matrix.InitPersProjTransform(angle, -aspect, znear, zfar);

    view_matrix.InitCameraTransform(enigma::Vector3f(xfrom,yfrom,zfrom),enigma::Vector3f(xto,yto,zto),enigma::Vector3f(xup,yup,zup));
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
    oglmgr->Transformation();
    projection_matrix.InitScaleTransform(1, -1, 1);
    projection_matrix.rotate(angle, 0, 0, 1);

    enigma::Matrix4f orhto;
    orhto.InitOtrhoProjTransform(x-0.5,x + width,y-0.5,y + height,32000,-32000);

    projection_matrix = projection_matrix * orhto;
    view_matrix.InitIdentity();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
    oglmgr->Transformation();
    projection_matrix.InitRotateVectorTransform(angle, enigma::Vector3f(0,0,1));

    enigma::Matrix4f persp, orhto;
    persp.InitPersProjTransform(60, 1, 0.1,32000);
    orhto.InitOtrhoProjTransform(x,x + width,y,y + height,0.1,32000);

    projection_matrix = projection_matrix * persp * orhto;
}

void d3d_transform_set_identity()
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    model_matrix.translate(xt, yt, zt);
}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    model_matrix.scale(xs, ys, zs);
}
void d3d_transform_add_rotation_x(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,1,0,0);
}
void d3d_transform_add_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,0,1,0);
}
void d3d_transform_add_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,0,0,1);
}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,x,y,z);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.translate(xt, yt, zt);
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.scale(xs, ys, zs);
}
void d3d_transform_set_rotation_x(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 1, 0, 0);
}
void d3d_transform_set_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 0, 1, 0);
}
void d3d_transform_set_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 0, 0, 1);
}
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, x, y, z);
}

}

#include <stack>
std::stack<enigma::Matrix4f> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{
    //if (trans_stack_size == 31) return false; //This limit no longer applies
    oglmgr->Transformation();
    trans_stack.push(model_matrix);
    trans_stack_size++;
    return true;
}

bool d3d_transform_stack_pop()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    model_matrix = trans_stack.top();
    trans_stack.pop();
    if (trans_stack_size > 0) trans_stack_size--;
    return true;
}

void d3d_transform_stack_clear()
{
    oglmgr->Transformation();
    do
      trans_stack.pop();
    while (trans_stack_size--);
    model_matrix.InitIdentity();
}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    model_matrix = trans_stack.top();
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
