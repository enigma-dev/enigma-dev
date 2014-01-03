/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, DatZach, Polygone
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
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include "Bridges/General/GL3Context.h"
#include <math.h>

using namespace std;

#include <floatcomp.h>

double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (enable) {
    gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  } else {
    gluPerspective(0, 1, 0, 1);
  }
  glMatrixMode(GL_MODELVIEW);
  // Unverified note: Perspective not the same as in GM when turning off perspective and using d3d projection
  // Unverified note: GM has some sort of dodgy behaviour where this function doesn't affect anything when calling after d3d_set_projection_ext
  // See also OpenGL3/GL3d3d.cpp Direct3D9/DX9d3d.cpp OpenGL1/GLd3d.cpp
}

void d3d_set_projection(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup)
{
  (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, double angle, double aspect, double znear, double zfar)
{
  (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, -aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(1, -1, 1);
  glRotatef(angle,0,0,1);
  gluPerspective(0, 1, 32000,-32000);
  glOrtho(x-0.5,x + width,y-0.5,y + height,32000,-32000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScalef(1, 1, 1);
  glRotatef(angle,0,0,1);
  gluPerspective(60, 1, 0.1,32000);
  glOrtho(x,x + width,y,y + height,0.1,32000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

//TODO: with all basic drawing add in normals

void d3d_transform_set_identity()
{
    oglmgr->Transformation();
    transformation_matrix[0] = 1;
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
    glLoadMatrixd(projection_matrix);
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glTranslatef(xt, yt, zt);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glScalef(xs, ys, zs);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_x(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,1,0,0);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_y(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,0,1,0);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_z(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,0,0,1);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,x,y,z);
    glMultMatrixd(transformation_matrix);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glTranslatef(xt, yt, zt);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glScalef(xs, ys, zs);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_x(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,1,0,0);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_y(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,0,1,0);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_z(double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,0,0,1);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{
    oglmgr->Transformation();
    glLoadIdentity();
    glRotatef(-angle,x,y,z);
    glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
    glLoadMatrixd(projection_matrix);
    glMultMatrixd(transformation_matrix);
}

}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

namespace enigma_user
{

bool d3d_transform_stack_push()
{
    if (trans_stack_size == 31) return false;
    oglmgr->Transformation();
    glPushMatrix();
    trans_stack.push(1);
    trans_stack_size++;
    return true;
}

bool d3d_transform_stack_pop()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    while (trans_stack.top() == 0)
    {
        glPopMatrix();
        trans_stack.pop();
    }
    if (trans_stack_size > 0) trans_stack_size--;
    return true;
}

void d3d_transform_stack_clear()
{
    oglmgr->Transformation();
    do
      glPopMatrix();
    while (trans_stack_size--);
    glLoadIdentity();
}

bool d3d_transform_stack_empty()
{
    return (trans_stack_size == 0);
}

bool d3d_transform_stack_top()
{
    if (trans_stack_size == 0) return false;
    oglmgr->Transformation();
    while (trans_stack.top() == 0)
    {
        glPopMatrix();
        trans_stack.pop();
    }
    glPushMatrix();
    return true;
}

bool d3d_transform_stack_disgard()
{
    if (trans_stack_size == 0) return false;
    trans_stack.push(0);
    trans_stack_size--;
    return true;
}

}