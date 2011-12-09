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
#include "GSd3d.h"
#include "../../Universal_System/var4.h"
#include "../../Universal_System/roomsystem.h"
#include <math.h>
#include "binding.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)

bool d3dMode = false;
double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

void d3d_start()
{
  // Enable depth buffering
  glEnable(GL_DEPTH_TEST);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);

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
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(0, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
}

void d3d_set_hidden(int enable)
{
    (enable?glEnable:glDisable)(GL_DEPTH_TEST);
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
 /* if (enable)
  {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, 1);
    glFogf(GL_FOG_DENSITY, 0.35f);
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
  }
  else
    glDisable(GL_FOG);*/
}//TODO: That fog is very wrong and breaks shit

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
    gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(0, 1, 0, 1);
    glMatrixMode(GL_MODELVIEW);
  } //Perspective not the same as in GM when turning off perspective and using d3d projection
}

void d3d_set_depth(double dep)
{

}//TODO: Write function

void d3d_set_shading(bool smooth)
{
    glShadeModel(smooth?GL_SMOOTH:GL_FLAT);
}

extern GLenum ptypes_by_id[16];
void d3d_primitive_begin(int kind)
{
    untexture();
    glBegin(ptypes_by_id[kind]);
}
void d3d_primitive_begin_texture(int kind, int texId)
{
    bind_texture(texId);
    glBegin(ptypes_by_id[kind]);
}

void d3d_primitive_end()
{
    glEnd();
}

void d3d_vertex(double x, double y, double z)
{
    glVertex3d(x,y,z);
}
void d3d_vertex_color(double x, double y, double z, int color, double alpha)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glVertex3d(x,y,z);
    glPopAttrib();
}
void d3d_vertex_texture(double x, double y, double z, double tx, double ty)
{
    glTexCoord2f(tx,ty);
    glVertex3d(x,y,z);
}
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glTexCoord2f(tx,ty);
    glVertex3d(x,y,z);
    glPopAttrib();
}

//TODO: fix push/pop with open primitives

void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz)
{
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
}
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
    glPopAttrib();
}
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
    glTexCoord2f(tx,ty);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
}
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glTexCoord2f(tx,ty);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
    glPopAttrib();
}

void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, -aspect, znear, zfar);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xfrom, yfrom, zfrom, xto, yto, zto, xup, yup, zup);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_set_projection_ortho(double x, double y, double width, double height, double angle)
{
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(1, -1, 1);
  glOrtho(x-1,x + width,y-1,y + height,0,1);
  glRotatef(angle,0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_set_projection_perspective(double x, double y, double width, double height, double angle)
{
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  glMatrixMode(GL_MODELVIEW);
  glScalef(1, -1, 1);
  glOrtho(x-1,x + width,y-1,y + height,0,1);
  glRotatef(angle,0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
  bind_texture(texId);
  glBegin(GL_QUADS);
    glTexCoord2d(0,0);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(0,vrep);
      glVertex3f(x1, y1, z2);
    glTexCoord2d(hrep,vrep);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(hrep,0);
      glVertex3f(x2, y2, z1);
  glEnd();
}

void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
  bind_texture(texId);
  glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
      glVertex3f(x1, y1, z1);
    glTexCoord2d(0, vrep);
      glVertex3f(x1, y2, z1);
    glTexCoord2d(hrep, vrep);
      glVertex3f(x2, y2, z2);
    glTexCoord2d(hrep, 0);
      glVertex3f(x2, y1, z2);
  glEnd();
}

void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed)
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
  if (closed)
  {
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
}

void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    steps = max(steps, 3);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, invstep = 1.0/steps, pr = 2*M_PI/steps;
    double a, px, py, tp;
    bind_texture(texId);
    glBegin(GL_TRIANGLE_STRIP);
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        glTexCoord2d(hrep*tp, 0);
          glVertex3f(px, py, z2);
        glTexCoord2d(hrep*tp, vrep);
          glVertex3f(px, py, z1);
        a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0, vrep);
          glVertex3f(cx, cy, z1);
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, z1);
            a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0, vrep);
          glVertex3f(cx, cy, z2);
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, z2);
            a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        glEnd();
    }
}

void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    steps = max(steps, 3);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, invstep = 1.0/steps, pr = 2*M_PI/steps;
    double a, px, py, tp;
    bind_texture(texId);
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2d(0, 0);
      glVertex3f(cx, cy, z2);
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        glTexCoord2d(hrep*tp, vrep);
          glVertex3f(px, py, z1);
        a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2d(0, vrep);
          glVertex3f(cx, cy, z1);
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, z1);
            a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        glEnd();
    }
}

void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, int steps)
{
    steps = max(steps, 3);
    const int zsteps = (ceil(steps/2) - 1)*2;
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = fabs(x2-x1)/2, ry = fabs(y2-y1)/2, rz = fabs(z2-z1)/2, invstep = 1.0/steps, pr = 2*M_PI/steps;
    double a, px, py, pz, pz2, tp;
    bind_texture(texId);
    pz = 0; pz2 = 0;
    for (int ii = 0; ii < (zsteps - 2)/2; ii++)
    {
        pz2 += 2*rz/zsteps;
        glBegin(GL_TRIANGLE_STRIP);
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, cz - pz);
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, cz - pz2);
            a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        glEnd();
        glBegin(GL_TRIANGLE_STRIP);
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, cz + pz);
            glTexCoord2d(hrep*tp, 0);
              glVertex3f(px, py, cz + pz2);
            a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        glEnd();
        pz = pz2;
    }
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2d(0, 0);
      glVertex3f(cx, cy, z1);
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        glTexCoord2d(hrep*tp, 0);
          glVertex3f(px, py, cz-pz);
        a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glTexCoord2d(0, 0);
      glVertex3f(cx, cy, z2);
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        glTexCoord2d(hrep*tp, 0);
          glVertex3f(px, py, cz+pz);
        a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
}//TODO: z and x,y coordinates properly (temporary just drew straight up). Texture properly.

//TODO: with all basic drawing add in normals, switch to vertex arrays

void d3d_transform_set_identity()
{
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

void d3d_transform_add_translation(double xt,double yt,double zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_scaling(double xs,double ys,double zs)
{
  glLoadIdentity();
  glScalef(xs, ys, zs);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_x(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,1,0,0);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_y(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,0,1,0);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_z(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,0,0,1);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_rotation_axis(double x, double y, double z, double angle)
{
  glLoadIdentity();
  glRotatef(-angle,x,y,z);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_transform_set_translation(double xt,double yt,double zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_scaling(double xs,double ys,double zs)
{
  glLoadIdentity();
  glScalef(xs, ys, zs);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_x(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,1,0,0);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_y(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,0,1,0);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_z(double angle)
{
  glLoadIdentity();
  glRotatef(-angle,0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_rotation_axis(double x, double y, double z, double angle)
{
  glLoadIdentity();
  glRotatef(-angle,x,y,z);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}

#include <stack>
stack<bool> trans_stack;
int trans_stack_size = 0;

bool d3d_transform_stack_push()
{
    if (trans_stack_size == 15) return false;
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

#include <map>
#include <vector>
#include "../../Universal_System/fileio.h"
class d3d_lights
{
    map<int,int> light_ind;

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    bool light_define_direction(int id, double dx, double dy, double dz, int col)
    {
        const int ms = light_ind.size();
        if (ms >= GL_MAX_LIGHTS)
            return false;
        light_ind.insert(pair<int,int>(id, ms));
        const float dir[3] = {dx, dy, dz}, color[4] = {__GETR(col), __GETG(col), __GETB(col), 0};
        glLightfv(GL_LIGHT0+ms, GL_SPOT_DIRECTION, dir);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);
        return true;
    }
    bool light_define_point(int id, double x, double y, double z, double range, int col)
    {
        const int ms = light_ind.size();
        if (ms >= GL_MAX_LIGHTS)
            return false;
        light_ind.insert(pair<int,int>(id, ms));
        const float pos[3] = {x, y, z}, color[4] = {__GETR(col), __GETG(col), __GETB(col), 0};
      	glLightfv(GL_LIGHT1, GL_POSITION, pos);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);
        return true;
    } //NOTE: spotlight is only 180 degress, and range cannot be defined
    bool light_enable(int id)
    {
        map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end())
        {
            const int ms = light_ind.size();
            if (ms >= GL_MAX_LIGHTS)
                return false;
            light_ind.insert(pair<int,int>(id, ms));
            glEnable(GL_LIGHT0+ms);
        }
        else
        {
            glEnable(GL_LIGHT0+(*it).second);
        }
        return true;
    }
    bool light_disable(int id)
    {
        map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end())
        {
            return false;
        }
        else
        {
            glDisable(GL_LIGHT0+(*it).second);
        }
        return true;
    }
} d3d_lighting;

bool d3d_light_define_direction(int id, double dx, double dy, double dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, double x, double y, double z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

void d3d_light_define_ambient(int col)
{
    const float color[4] = {__GETR(col), __GETG(col), __GETB(col), 0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
}

class d3d_model
{
    int something, model_call_maxid;
    vector<vector<double> > model_calls;

    public:
    d3d_model()
    {
        something = 100;
        model_call_maxid = 0;
    }
    ~d3d_model() {}

    void clear()
    {
        for (int i = 0; i < model_call_maxid; i++)
        {
            model_calls[i].clear();
        }
        model_calls.clear();
        model_call_maxid = 0;
    }

    void save(string fname)
    {
        int file = file_text_open_write(fname);
        file_text_write_real(file, something);
        file_text_writeln(file);
        file_text_write_real(file, model_call_maxid);
        file_text_writeln(file);
        for (int i = 0; i < model_call_maxid; i++)
        {
            for (int ii = 0; ii < 11; ii++)
            {
                file_text_write_real(file, model_calls[i][ii]);
            }
            file_text_writeln(file);
        }
        file_text_close(file);
    }

    void load(string fname)
    {
        int file = file_text_open_read(fname);
        something = file_text_read_real(file);
        file_text_readln(file);
        model_call_maxid = file_text_read_real(file);
        file_text_readln(file);
        for (int i = 0; i < model_call_maxid; i++)
        {
            model_calls.push_back(vector<double>());
            for (int ii = 0; ii < 11; ii++)
            {
                model_calls[i].push_back(file_text_read_real(file));
            }
            file_text_writeln(file);
        }
        file_text_close(file);
    }

    void draw(double x, double y, double z, int texId)
    {
        bind_texture(texId);
        for (int i = 0; i < model_call_maxid; i++)
        {
            switch (int(model_calls[i][0]))
            {
                case  0: d3d_primitive_begin_texture(model_calls[i][1],texId); break;
                case  1: d3d_primitive_end(); break;
                case  2: d3d_vertex(model_calls[i][1], model_calls[i][2], model_calls[i][3]); break;
                case  3: d3d_vertex_color(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5]); break;
                case  4: d3d_vertex_texture(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5]); break;
                case  5: d3d_vertex_texture_color(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], model_calls[i][7]); break;
                case  6: d3d_vertex_normal(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6]); break;
                case  7: d3d_vertex_normal_color(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], model_calls[i][7], model_calls[i][8]); break;
                case  8: d3d_vertex_normal_texture(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], model_calls[i][7], model_calls[i][8]); break;
                case  9: d3d_vertex_normal_texture_color(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], model_calls[i][7], model_calls[i][8], model_calls[i][9], model_calls[i][10]); break;
                case 10: d3d_draw_block(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8]); break;
                case 11: d3d_draw_cylinder(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8], model_calls[i][9], model_calls[i][10]); break;
                case 12: d3d_draw_cone(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8], model_calls[i][9], model_calls[i][10]); break;
                case 13: d3d_draw_ellipsoid(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8], model_calls[i][9]); break;
                case 14: d3d_draw_wall(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8]); break;
                case 15: d3d_draw_floor(model_calls[i][1], model_calls[i][2], model_calls[i][3], model_calls[i][4], model_calls[i][5], model_calls[i][6], texId, model_calls[i][7], model_calls[i][8]); break;
            }
        }
    }// TODO: Add translation, push/pop texture binding, efficient model drawing

    void primitive_begin(int kind)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(kind);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void primitive_end()
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(1);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex(double x, double y, double z)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(2);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_color(double x, double y, double z, int col, double alpha)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(3);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(col);
        model_calls[model_call_maxid].push_back(alpha);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_texture(double x, double y, double z, double tx, double ty)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(4);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(tx);
        model_calls[model_call_maxid].push_back(ty);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_texture_color(double x, double y, double z, double tx, double ty, int col, double alpha)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(5);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(tx);
        model_calls[model_call_maxid].push_back(ty);
        model_calls[model_call_maxid].push_back(col);
        model_calls[model_call_maxid].push_back(alpha);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_normal(double x, double y, double z, double nx, double ny, double nz)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(6);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(nx);
        model_calls[model_call_maxid].push_back(ny);
        model_calls[model_call_maxid].push_back(nz);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(7);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(nx);
        model_calls[model_call_maxid].push_back(ny);
        model_calls[model_call_maxid].push_back(nz);
        model_calls[model_call_maxid].push_back(col);
        model_calls[model_call_maxid].push_back(alpha);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(8);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(nx);
        model_calls[model_call_maxid].push_back(ny);
        model_calls[model_call_maxid].push_back(nz);
        model_calls[model_call_maxid].push_back(tx);
        model_calls[model_call_maxid].push_back(ty);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(9);
        model_calls[model_call_maxid].push_back(x);
        model_calls[model_call_maxid].push_back(y);
        model_calls[model_call_maxid].push_back(z);
        model_calls[model_call_maxid].push_back(nx);
        model_calls[model_call_maxid].push_back(ny);
        model_calls[model_call_maxid].push_back(nz);
        model_calls[model_call_maxid].push_back(tx);
        model_calls[model_call_maxid].push_back(ty);
        model_calls[model_call_maxid].push_back(col);
        model_calls[model_call_maxid].push_back(alpha);
        model_call_maxid++;
    }

    void draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(10);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(11);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(closed);
        model_calls[model_call_maxid].push_back(steps);
        model_call_maxid++;
    }

    void draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(12);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(closed);
        model_calls[model_call_maxid].push_back(steps);
        model_call_maxid++;
    }

    void draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(13);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(steps);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(14);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }

    void draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
    {
        model_calls.push_back(vector<double>());
        model_calls[model_call_maxid].push_back(15);
        model_calls[model_call_maxid].push_back(x1);
        model_calls[model_call_maxid].push_back(y1);
        model_calls[model_call_maxid].push_back(z1);
        model_calls[model_call_maxid].push_back(x2);
        model_calls[model_call_maxid].push_back(y2);
        model_calls[model_call_maxid].push_back(z2);
        model_calls[model_call_maxid].push_back(hrep);
        model_calls[model_call_maxid].push_back(vrep);
        model_calls[model_call_maxid].push_back(0);
        model_calls[model_call_maxid].push_back(0);
        model_call_maxid++;
    }
};

static map<unsigned int, d3d_model> d3d_models;
static unsigned int d3d_models_maxid = 0;

unsigned int d3d_model_create()
{
    d3d_models.insert(pair<unsigned int, d3d_model>(d3d_models_maxid++, d3d_model()));
    return d3d_models_maxid-1;
}

void d3d_model_destroy(const unsigned int id)
{
    d3d_models[id].clear();
    d3d_models.erase(d3d_models.find(id));
}

void d3d_model_copy(const unsigned int id, const unsigned int source)
{
    d3d_models[id] = d3d_models[source];
}

unsigned int d3d_model_duplicate(const unsigned int source)
{
    d3d_models.insert(pair<unsigned int, d3d_model>(d3d_models_maxid++, d3d_model()));
    d3d_models[d3d_models_maxid-1] = d3d_models[source];
    return d3d_models_maxid-1;
}

bool d3d_model_exists(const unsigned int id)
{
    return (d3d_models.find(id) != d3d_models.end());
}

void d3d_model_clear(const unsigned int id)
{
    d3d_models[id].clear();
}

void d3d_model_save(const unsigned int id, string fname)
{
    d3d_models[id].save(fname);
}

void d3d_model_load(const unsigned int id, string fname)
{
    d3d_models[id].clear();
    d3d_models[id].load(fname);
}

void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId)
{
    d3d_models[id].draw(x, y, z, texId);
}

void d3d_model_primitive_begin(const unsigned int id, int kind)
{
    d3d_models[id].primitive_begin(kind);
}

void d3d_model_primitive_end(const unsigned int id)
{
    d3d_models[id].primitive_end();
}

void d3d_model_vertex(const unsigned int id, double x, double y, double z)
{
    d3d_models[id].vertex(x, y, z);
}

void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha)
{
    d3d_models[id].vertex_color(x, y, z, col, alpha);
}

void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty)
{
    d3d_models[id].vertex_texture(x, y, z, tx, ty);
}

void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha)
{
    d3d_models[id].vertex_texture_color(x, y, z, tx, ty, col, alpha);
}

void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz)
{
    d3d_models[id].vertex_normal(x, y, z, nx, ny, nz);
}

void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
{
    d3d_models[id].vertex_normal_color(x, y, z, nx, ny, nz, col, alpha);
}

void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
    d3d_models[id].vertex_normal_texture(x, y, z, nx, ny, nz, tx, ty);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
{
    d3d_models[id].vertex_normal_texture_color(x, y, z, nx, ny, nz, tx, ty, col, alpha);
}

void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
    d3d_models[id].draw_block(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{
    d3d_models[id].draw_cylinder(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{
    d3d_models[id].draw_cone(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps)
{
    d3d_models[id].draw_ellipsoid(x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
}

void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
    d3d_models[id].draw_wall(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
    d3d_models[id].draw_floor(x1, y1, z1, x2, y2, z2, hrep, vrep);
}
