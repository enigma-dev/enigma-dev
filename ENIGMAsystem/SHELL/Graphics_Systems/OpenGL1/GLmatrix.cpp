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
#include "Universal_System/var4.h"
#include <math.h>

using namespace std;

#include <floatcomp.h>

enigma::Matrix4f view_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1), model_matrix(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

//NOTE: THIS IS STILL FFP
#ifdef GS_SCALAR_64
#define glLoadMatrix(m)   glLoadMatrixd((gs_scalar*)m);
#define glMultMatrix(m)   glMultMatrixd((gs_scalar*)m);
#define glGet(m,n)        glGetDoublev(m,(gs_scalar*)n);
#define glScale(x,y,z)    glScaled(x,y,z);
#define glRotate(a,x,y,z) glRotated(a,x,y,z);
#else
#define glLoadMatrix(m)   glLoadMatrixf((gs_scalar*)m);
#define glMultMatrix(m)   glMultMatrixf((gs_scalar*)m);
#define glGet(m,n)        glGetFloatv(m,(gs_scalar*)n);
#define glScale(x,y,z)    glScalef(x,y,z);
#define glRotate(a,x,y,z) glRotatef(a,x,y,z);
#endif

namespace enigma_user
{

void d3d_set_perspective(bool enable)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (enable) {
    gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
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
  glGet(GL_MODELVIEW_MATRIX,view_matrix);
  glMultMatrix(model_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ext(gs_scalar xfrom, gs_scalar yfrom, gs_scalar zfrom, gs_scalar xto, gs_scalar yto, gs_scalar zto, gs_scalar xup, gs_scalar yup, gs_scalar zup, gs_scalar angle, gs_scalar aspect, gs_scalar znear, gs_scalar zfar)
{
  if (angle == 0 || znear == 0) return; //THEY CANNOT BE 0!!!
  (enigma::d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, -aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  glGet(GL_MODELVIEW_MATRIX,view_matrix);
  glMultMatrix(model_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_ortho(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScale(1, -1, 1);
  glRotate(angle,0,0,1);
  glOrtho(x-0.5,x + width,y-0.5,y + height,32000,-32000);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGet(GL_MODELVIEW_MATRIX,view_matrix);
  glMultMatrix(model_matrix);

  enigma::d3d_light_update_positions();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, gs_scalar angle)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glScale(1, 1, 1);
  glRotate(angle,0,0,1);
  gluPerspective(60, 1, 0.1,32000);
  glOrtho(x,x + width,y,y + height,0.1,32000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glGet(GL_MODELVIEW_MATRIX,view_matrix);
  glMultMatrix(model_matrix);
  enigma::d3d_light_update_positions();
}

//TODO: with all basic drawing add in normals

void d3d_transform_set_identity()
{
  model_matrix.InitIdentity();
  glLoadMatrix(view_matrix);
}

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
  glLoadIdentity();
  glScale(xs, ys, zs);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_add_rotation_x(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,1,0,0);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_add_rotation_y(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,0,1,0);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_add_rotation_z(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,0,0,1);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,x,y,z);
  glMultMatrix(model_matrix);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
{
  glLoadIdentity();
  glScale(xs, ys, zs);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_set_rotation_x(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,1,0,0);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_set_rotation_y(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,0,1,0);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_set_rotation_z(gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,0,0,1);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
}
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar angle)
{
  glLoadIdentity();
  glRotate(-angle,x,y,z);
  glGet(GL_MODELVIEW_MATRIX,model_matrix);
  glLoadMatrix(view_matrix);
  glMultMatrix(model_matrix);
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
    glPushMatrix();
    trans_stack.push(1);
    trans_stack_size++;
    return true;
}

bool d3d_transform_stack_pop()
{
    if (trans_stack_size == 0) return false;
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
