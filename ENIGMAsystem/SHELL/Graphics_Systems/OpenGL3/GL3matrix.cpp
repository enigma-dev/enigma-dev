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
#define glLoadMatrix(m) glLoadMatrixd((gs_scalar*)m);
#else
#define glLoadMatrix(m) glLoadMatrixf((gs_scalar*)m);
#endif

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  if (enable) {
    projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);
  } else {
    projection_matrix.InitPersProjTransform(0, 1, 0, 1);
  }

  mvp_matrix = projection_matrix * view_matrix * model_matrix;

  //NOTE: THIS IS STILL FFP
  glMatrixMode(GL_PROJECTION);
  glLoadMatrix(projection_matrix);

  //glMatrixMode(GL_MODELVIEW);
  // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
  // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
  // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
  (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();

  projection_matrix.InitPersProjTransform(45, -view_wview[view_current] / (gs_scalar)view_hview[view_current], 1, 32000);

  //gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  view_matrix.InitCameraTransform(enigma::Vector3f(xto - xfrom, yto - yfrom, zto - zfrom),enigma::Vector3f(xup,yup,zup));

  //gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);

  mv_matrix = view_matrix * model_matrix;
  mvp_matrix = projection_matrix * mv_matrix;

  //NOTE: THIS IS STILL FFP
  glMatrixMode(GL_PROJECTION);
  glLoadMatrix(projection_matrix);

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(mv_matrix);

  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
  (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();

  projection_matrix.InitPersProjTransform(angle, -aspect, znear, zfar);

  view_matrix.InitCameraTransform(enigma::Vector3f(xto - xfrom, yto - yfrom, zto - zfrom),enigma::Vector3f(xup,yup,zup));

  //gluPerspective(angle, -aspect, znear, zfar);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  //glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  //glMultMatrixd(transformation_matrix);

  mv_matrix = view_matrix * model_matrix;
  mvp_matrix = projection_matrix * mv_matrix;

  //NOTE: THIS IS STILL FFP
  glMatrixMode(GL_PROJECTION);
  glLoadMatrix(projection_matrix);

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(mv_matrix);

  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  //glMatrixMode(GL_PROJECTION);
  //glLoadIdentity();
  projection_matrix.InitScaleTransform(1, -1, 1);
  projection_matrix.rotate(angle, 0, 0, 1);

  enigma::Matrix4f persp, orhto;
  persp.InitPersProjTransform(0, 1, 32000,-32000);
  orhto.InitOtrhoProjTransform(x-0.5,x + width,y-0.5,y + height,32000,-32000);

  projection_matrix = projection_matrix * persp * orhto;
  //glScalef(1, -1, 1);
  //glRotatef(angle,0,0,1);
  //gluPerspective(0, 1, 32000,-32000);
  //glOrtho(x-0.5,x + width,y-0.5,y + height,32000,-32000);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();

  //mv_matrix = view_matrix * model_matrix;
  mvp_matrix = projection_matrix * mv_matrix;

  //NOTE: THIS IS STILL FFP
  glMatrixMode(GL_PROJECTION);
  glLoadMatrix(projection_matrix);

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(mv_matrix);

  //glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  //glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  //glMatrixMode(GL_PROJECTION);
  projection_matrix.InitRotateVectorTransform(angle, enigma::Vector3f(0,0,1));

  enigma::Matrix4f persp, orhto;
  persp.InitPersProjTransform(60, 1, 0.1,32000);
  orhto.InitOtrhoProjTransform(x,x + width,y,y + height,0.1,32000);

  projection_matrix = projection_matrix * persp * orhto;
  mvp_matrix = projection_matrix * mv_matrix;

  //NOTE: THIS IS STILL FFP
  glMatrixMode(GL_PROJECTION);
  glLoadMatrix(projection_matrix);

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrix(mv_matrix);

  //glLoadIdentity();
  //glScalef(1, 1, 1);

  //glRotatef(angle,0,0,1);
  //gluPerspective(60, 1, 0.1,32000);
  //glOrtho(x,x + width,y,y + height,0.1,32000);
  //glMatrixMode(GL_MODELVIEW);
  //glLoadIdentity();
  //glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  //glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

//TODO: with all basic drawing add in normals

void d3d_transform_set_identity()
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*transformation_matrix[0] = 1;
    transformation_matrix[1] = 0;
    transformation_matrix[2] = 0;
    transformation_matrix[3] = 0;
    transformation_matrix[4] = 0;
    transformation_matrix[5] = 1;
    transformation_matrix[6] = 0;
    transformation_matrix[7] = 0;
    transformation_matrix[8] = 0;
    transformation_matrix[9] = 0;
    transformation_matrix[10] = 1;
    transformation_matrix[11] = 0;
    transformation_matrix[12] = 0;
    transformation_matrix[13] = 0;
    transformation_matrix[14] = 0;
    transformation_matrix[15] = 1;
    glLoadMatrixd(projection_matrix);*/
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    //model_matrix.InitIdentity();
    model_matrix.translate(xt, yt, zt);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    //glLoadIdentity();
    //glTranslatef(xt, yt, zt);
    //glMultMatrixd(transformation_matrix);
    //glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    //glLoadMatrixd(projection_matrix);
    //glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    //model_matrix.InitIdentity();
    model_matrix.scale(xs, ys, zs);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glScalef(xs, ys, zs);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_add_rotation_x(gs_scalar angle)
{
    model_matrix.rotate(-angle,1,0,0);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,1,0,0);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_add_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,0,1,0);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,0,1,0);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_add_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,0,0,1);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,0,0,1);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.rotate(-angle,x,y,z);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,x,y,z);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.translate(xt, yt, zt);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glTranslatef(xt, yt, zt);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.scale(xs, ys, zs);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glScalef(xs, ys, zs);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_set_rotation_x(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 1, 0, 0);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,1,0,0);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_set_rotation_y(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 0, 1, 0);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,0,1,0);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_set_rotation_z(gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, 0, 0, 1);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,0,0,1);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
}
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
    oglmgr->Transformation();
    model_matrix.InitIdentity();
    model_matrix.rotate(-angle, x, y, z);
    mv_matrix = view_matrix * model_matrix;
    mvp_matrix = projection_matrix * mv_matrix;

    //NOTE: THIS IS STILL FFP
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrix(mv_matrix);

    /*glLoadIdentity();
    glRotatef(-angle,x,y,z);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);*/
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
