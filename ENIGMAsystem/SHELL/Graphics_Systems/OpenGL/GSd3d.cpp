/** Copyright (C) 2008-2011 DatZach, Josh Ventura, Polygone
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

using namespace std;

#include "OpenGLHeaders.h"
#include "GSprmtvs.h"
#include "GSd3d.h"
#include <string>
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"
#include <math.h>
#include "binding.h"

bool d3dMode = false;

void d3d_start()
{
  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 640);

  // Set up modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void d3d_end()
{
  d3dMode = false;
  glDisable(GL_DEPTH_TEST);
  glOrtho(-1, room_width, -1, room_height, 0, 1);
}

void d3d_set_hidden(int enable)
{

}

void d3d_set_lighting(int enable)
{
  if (enable)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);
}

void d3d_set_fog(int enable, int color, int start, int end)
{
  if (enable)
  {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, 1);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
  }
  else
    glDisable(GL_FOG);
}

void d3d_set_culling(int enable)
{
  if (enable)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
}

void d3d_set_perspective(int enable)
{
  if (enable)
  {
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45, view_wview[view_current] / (double)view_hview[view_current], 1, 640);
  }
  else
  {

  }
}

extern GLenum ptypes_by_id[16];
void d3d_primitive_begin(int kind) {
  glBegin(ptypes_by_id[kind]);
}
void d3d_vertex(double x, double y, double z) {
  glVertex3d(x,y,z);
}
void d3d_primitive_end() {
  glEnd();
}

void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
}

void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, -aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
}

void d3d_set_projection_ortho(int x, int y, int width, int height, int angle)
{
  glOrtho(x,x + width,y,y + height,0,1);
  glRotated(angle,0,0,1);
}

void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
  bind_texture(texId);
  glBegin(GL_QUADS);
    glTexCoord2d(0,1);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(0,0);
      glVertex3f(x1, y1, z2);
    glTexCoord2d(1,0);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(1,1);
      glVertex3f(x2, y2, z1);
  glEnd();
}

void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
  bind_texture(texId);
  glBegin(GL_QUADS);
    glTexCoord2d(0, vrep);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(0,0);
      glVertex3f(x1, y2, z1);
    glTexCoord2d(hrep, 0);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(hrep, vrep);
      glVertex3f(x2, y1, z2);
  glEnd();
}

void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
  bind_texture(texId);
  glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2d(0, vrep);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(0,0);
      glVertex3f(x1, y1, z2);
    glTexCoord2d(hrep, vrep);
      glVertex3f(x2, y1, z1);
    glTexCoord2d(hrep, 0);
      glVertex3f(x2, y1, z2);
    glTexCoord2d(hrep*2, vrep);
      glVertex3f(x2, y2, z1);
    glTexCoord2d(hrep*2, 0);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(hrep*3, vrep);
      glVertex3f(x1, y2, z1);
    glTexCoord2d(hrep*3, 0);
      glVertex3f(x1, y2, z2);
    glTexCoord2d(hrep*4, vrep);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(hrep*4, 0);
      glVertex3f(x1, y1, z2);
  glEnd();
  glBegin(GL_QUADS);
    glTexCoord2d(0, vrep);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(hrep,vrep);
      glVertex3f(x2, y1, z1);
    glTexCoord2d(hrep, 0);
      glVertex3f(x2, y2, z1);
    glTexCoord2d(0, 0);
      glVertex3f(x1, y2, z1);
    glTexCoord2d(0, vrep);
      glVertex3f(x1, y1, z2);
    glTexCoord2d(hrep,vrep);
      glVertex3f(x2, y1, z2);
    glTexCoord2d(hrep, 0);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(0, 0);
      glVertex3f(x1, y2, z2);
  glEnd();
}

void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    steps = max(steps, 3);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, invstep = 1.0/steps, pr = 2*M_PI/steps;
    double a1, a2, px1, py1, px2, py2, tp1, tp2;
    bind_texture(texId);
    if (closed)
    {
        a2 = 0; px2 = rx; py2 = 0; tp2 = 0;
        for (int i = 0; i < steps; i++)
        {
            a1 = a2; a2 += pr; px1 = px2; py1 = py2; px2 = cos(a2)*rx; py2 = sin(a2)*ry; tp1 = tp2; tp2 += invstep;
            glBegin(GL_QUADS);
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx+px1, cy+py1, z2);
            glTexCoord2d(hrep*tp2, 0);
            glVertex3f(cx+px2, cy+py2, z2);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
            glEnd();
            glBegin(GL_TRIANGLES);
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx, cy, z1);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z2);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx, cy, z2);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z2);
            glEnd();
        }
    }
    else
    {
        glBegin(GL_QUADS);
        a2 = 0; px2 = rx; py2 = 0; tp2 = 0;
        for (int i = 0; i < steps; i++)
        {
            a1 = a2; a2 += pr; px1 = px2; py1 = py2; px2 = cos(a2)*rx; py2 = sin(a2)*ry; tp1 = tp2; tp2 += invstep;
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx+px1, cy+py1, z2);
            glTexCoord2d(hrep*tp2, 0);
            glVertex3f(cx+px2, cy+py2, z2);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
        }
        glEnd();
    }
}

void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    steps = max(steps, 3);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, invstep = 1.0/steps, pr = 2*M_PI/steps;
    double a1, a2, px1, py1, px2, py2, tp1, tp2;
    bind_texture(texId);
    if (closed)
    {
        a2 = 0; px2 = rx; py2 = 0; tp2 = 0;
        for (int i = 0; i < steps; i++)
        {
            a1 = a2; a2 += pr; px1 = px2; py1 = py2; px2 = cos(a2)*rx; py2 = sin(a2)*ry; tp1 = tp2; tp2 += invstep;
            glBegin(GL_TRIANGLES);
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx, cy, z2);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
            glEnd();
            glBegin(GL_TRIANGLES);
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx, cy, z1);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
            glEnd();
        }
    }
    else
    {
        glBegin(GL_TRIANGLES);
        a2 = 0; px2 = rx; py2 = 0; tp2 = 0;
        for (int i = 0; i < steps; i++)
        {
            a1 = a2; a2 += pr; px1 = px2; py1 = py2; px2 = cos(a2)*rx; py2 = sin(a2)*ry; tp1 = tp2; tp2 += invstep;
            glTexCoord2d(hrep*tp1, vrep);
            glVertex3f(cx+px1, cy+py1, z1);
            glTexCoord2d(hrep*tp1, 0);
            glVertex3f(cx, cy, z2);
            glTexCoord2d(hrep*tp2, vrep);
            glVertex3f(cx+px2, cy+py2, z1);
        }
        glEnd();
    }
}

void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, int steps)
{
    steps = max(steps, 3);
    const int zsteps = ceil(steps/2);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, rz = fabs(z2-z1)/2, pr = 2*M_PI/steps, qr = M_PI/zsteps;
    double a1, a2, px1, py1, px2, py2, b1, b2, qz1, qz2, qx11, qx12, qy11, qy12, qx21, qx22, qy21, qy22, zr1, zr2;
    bind_texture(texId);
    a2 = 0; px2 = rx; py2 = 0;
    for (int i = 0; i < steps; i++)
    {
        a1 = a2; a2 += pr; px1 = px2; py1 = py2; px2 = cos(a2)*rx; py2 = sin(a2)*ry;
        b2 = 0; qz2 = 0; qx12 = px1; qy12 = py1; qx22 = px2; qy22 = py2; zr1 = sqrt(px1*px1 + py1*py1); zr2 = sqrt(px2*px2 + py2*py2);
        glBegin(GL_QUADS);
        for (int ii = 0; ii < zsteps - 2; ii++)
        {
            b1 = b2; b2 += qr; qz1 = qz2; qz2 = sin(b2)*rz; qx11 = qx12; qy11 = qy12; qx21 = qx22; qy21 = qy22; qx12 = cos(b2)*zr1; qy12 = sin(b2)*zr1; qx22 = cos(b2)*zr2; qy22 = sin(b2)*zr2;
            glVertex3f(cx+qx11, cy+qy11, cz+qz1);
            glVertex3f(cx+qx12, cy+qy12, cz+qz2);
            glVertex3f(cx+qx22, cy+qy22, cz+qz2);
            glVertex3f(cx+qx21, cy+qy21, cz+qz1);
            glVertex3f(cx+qx11, cy+qy11, cz-qz1);
            glVertex3f(cx+qx12, cy+qy12, cz-qz2);
            glVertex3f(cx+qx22, cy+qy22, cz-qz2);
            glVertex3f(cx+qx21, cy+qy21, cz-qz1);
        } //TODO: Fix quads (ie use code that actually makes sense)
        glEnd();
        glBegin(GL_TRIANGLES);
        glVertex3f(cx+qx12, cy+qy12, cz+qz2);
        glVertex3f(cx, cy, z2);
        glVertex3f(cx+qx22, cy+qy22, cz+qz2);
        glVertex3f(cx+qx12, cy+qy12, cz-qz2);
        glVertex3f(cx, cy, z1);
        glVertex3f(cx+qx22, cy+qy22, cz-qz2);
        glEnd();
    }
}//TODO: Also needs texturing

void d3d_transform_set_identity()
{
  glLoadIdentity();
  glScalef(1,-1,1);
  glOrtho(-1,room_width,-1,room_height,0,1);
}

void d3d_transform_add_translation(double xt,double yt,double zt) {
  glTranslated(xt, yt, zt);
}

void d3d_transform_add_scaling(double xs,double ys,double zs) {
  glScaled(xs, ys, zs);
}
void d3d_transform_add_rotation_x(double angle) {
  glRotated(-angle,1,0,0);
}

void d3d_transform_add_rotation_y(double angle) {
  glRotated(-angle,0,1,0);
}
void d3d_transform_add_rotation_z(double angle) {
  glRotatef(-angle,0,0,1);
}

void d3d_transform_stack_push() {
  glPushMatrix();
}

void d3d_transform_stack_pop() {
  glPopMatrix();
}

void d3d_transform_set_translation(double xt,double yt,double zt)
{
  d3d_transform_set_identity();
  glTranslated(xt, yt, zt);
}

void d3d_transform_set_scaling(double xs,double ys,double zs)
{
  d3d_transform_set_identity();
  glScaled(xs, ys, zs);
}
void d3d_transform_set_rotation_x(double angle)
{
  d3d_transform_set_identity();
  glRotated(-angle,1,0,0);
}

void d3d_transform_set_rotation_y(double angle)
{
  d3d_transform_set_identity();
  glRotated(-angle,0,1,0);
}
void d3d_transform_set_rotation_z(double angle)
{
  d3d_transform_set_identity();
  glRotated(-angle,0,0,1);
}
