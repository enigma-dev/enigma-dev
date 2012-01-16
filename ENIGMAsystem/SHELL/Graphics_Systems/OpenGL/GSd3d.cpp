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

#include "OpenGLHeaders.h"
#include "GSd3d.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "binding.h"

using namespace std;

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

bool d3d_get_mode()
{
    return d3dMode;
}

void d3d_set_hidden(bool enable)
{
    (enable?glEnable:glDisable)(GL_DEPTH_TEST);
}

void d3d_set_lighting(bool enable)
{
  if (enable)
    glEnable(GL_LIGHTING);
  else
    glDisable(GL_LIGHTING);
}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  if (enable)
  {
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, start);
    glFogf(GL_FOG_END, end);
    GLfloat fog_color[3];
    fog_color[0] = __GETR(color);
    fog_color[1] = __GETG(color);
    fog_color[2] = __GETB(color);
    glFogfv(GL_FOG_COLOR,fog_color);
  }
  else
    glDisable(GL_FOG);
}//NOTE: fog can use vertex checks with less good graphic cards which screws up large textures (however this doesn't happen in directx)

void d3d_set_culling(bool enable)
{
/*  if (enable)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);*/
}//TODO: Culling not working the same as in GM, not advised to enable it's use as it pretty much kills the drawing

void d3d_set_perspective(bool enable)
{
  if (enable)
  {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
    glMatrixMode(GL_MODELVIEW);
  }
  else
  {
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
namespace enigma {
  extern unsigned char currentcolor[4];
}

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
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}
void d3d_vertex_texture(double x, double y, double z, double tx, double ty)
{
    glTexCoord2f(tx,ty);
    glVertex3d(x,y,z);
}
void d3d_vertex_texture_color(double x, double y, double z, double tx, double ty, int color, double alpha)
{
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glTexCoord2f(tx,ty);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}

void d3d_vertex_normal(double x, double y, double z, double nx, double ny, double nz)
{
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
}
void d3d_vertex_normal_color(double x, double y, double z, double nx, double ny, double nz, int color, double alpha)
{
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}
void d3d_vertex_normal_texture(double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
    glTexCoord2f(tx,ty);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
}
void d3d_vertex_normal_texture_color(double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int color, double alpha)
{
    glColor4f(__GETR(color), __GETG(color), __GETB(color), alpha);
    glTexCoord2f(tx,ty);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}

void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
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
    float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y2, z1}, v3[] = {x2, y2, z2},
          t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
    bind_texture(texId);
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(t0);
      glVertex3fv(v0);
    glTexCoord2fv(t1);
      glVertex3fv(v1);
    glTexCoord2fv(t2);
      glVertex3fv(v2);
    glTexCoord2fv(t3);
      glVertex3fv(v3);
    glEnd();
}

void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep)
{
    float v0[] = {x1, y1, z1}, v1[] = {x1, y2, z1}, v2[] = {x2, y1, z2}, v3[] = {x2, y2, z2},
          t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
    bind_texture(texId);
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(t0);
      glVertex3fv(v0);
    glTexCoord2fv(t1);
      glVertex3fv(v1);
    glTexCoord2fv(t2);
      glVertex3fv(v2);
    glTexCoord2fv(t3);
      glVertex3fv(v3);
    glEnd();
}

void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed)
{
    float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y1, z1}, v3[] = {x2, y1, z2},
          v4[] = {x2, y2, z1}, v5[] = {x2, y2, z2}, v6[] = {x1, y2, z1}, v7[] = {x1, y2, z2},
          t0[] = {0, vrep}, t1[] = {0, 0}, t2[] = {hrep, vrep}, t3[] = {hrep, 0},
          t4[] = {hrep*2, vrep}, t5[] = {hrep*2, 0}, t6[] = {hrep*3, vrep}, t7[] = {hrep*3, 0},
          t8[] = {hrep*4, vrep}, t9[] = {hrep*4, 0};
    bind_texture(texId);
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2fv(t0);
      glVertex3fv(v0);
    glTexCoord2fv(t1);
      glVertex3fv(v1);
    glTexCoord2fv(t2);
      glVertex3fv(v2);
    glTexCoord2fv(t3);
      glVertex3fv(v3);
    glTexCoord2fv(t4);
      glVertex3fv(v4);
    glTexCoord2fv(t5);
      glVertex3fv(v5);
    glTexCoord2fv(t6);
      glVertex3fv(v6);
    glTexCoord2fv(t7);
      glVertex3fv(v7);
    glTexCoord2fv(t8);
      glVertex3fv(v0);
    glTexCoord2fv(t9);
      glVertex3fv(v1);
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2fv(t0);
          glVertex3fv(v0);
        glTexCoord2fv(t1);
          glVertex3fv(v2);
        glTexCoord2fv(t2);
          glVertex3fv(v6);
        glTexCoord2fv(t3);
          glVertex3fv(v4);
        glEnd();

        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2fv(t0);
          glVertex3fv(v1);
        glTexCoord2fv(t1);
          glVertex3fv(v3);
        glTexCoord2fv(t2);
          glVertex3fv(v7);
        glTexCoord2fv(t3);
          glVertex3fv(v5);
        glEnd();
    }
}

void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    float v[100][3];
    float t[100][3];
    steps = min(max(steps, 3), 48);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
    double a, px, py, tp;
    int k;
    bind_texture(texId);
    glBegin(GL_TRIANGLE_STRIP);
    a = 0; px = cx+rx; py = cy; tp = 0; k = 0;
    for (int i = 0; i <= steps; i++)
    {
        v[k][0] = px; v[k][1] = py; v[k][2] = z2;
        t[k][0] = tp; t[k][1] = 0;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++;
        v[k][0] = px; v[k][1] = py; v[k][2] = z1;
        t[k][0] = tp; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
        t[k][0] = 0; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++;
        for (int i = 0; i <= steps*2; i+=2)
        {
            glTexCoord2fv(t[i]);
              glVertex3fv(v[i+1]);
        }
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
        t[k][0] = 0; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++;
        for (int i = 0; i <= steps*2; i+=2)
        {
            glTexCoord2fv(t[i]);
              glVertex3fv(v[i]);
        }
        glEnd();
    }
}

void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, bool closed, int steps)
{
    float v[51][3];
    float t[100][3];
    steps = min(max(steps, 3), 48);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
    double a, px, py, tp;
    int k = 0;
    bind_texture(texId);
    glBegin(GL_TRIANGLE_FAN);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
    t[k][0] = 0; t[k][1] = 0;
    glTexCoord2fv(t[k]);
      glVertex3fv(v[k]);
    k++;
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        v[k][0] = px; v[k][1] = py; v[k][2] = z1;
        t[k][0] = tp; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
        t[k][0] = 0; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++;
        tp = 0;
        for (int i = 1; i <= steps+1; i++)
        {
            t[k][0] = tp; t[k][1] = 0;
            glTexCoord2fv(t[k]);
              glVertex3fv(v[i]);
            k++; tp += invstep;
        }
        glEnd();
    }
}

void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texId, int hrep, int vrep, int steps)
{
    float v[277][3];
    float t[277][3];
    steps = min(max(steps, 3), 24);
    const int zsteps = ceil(steps/2);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
    double a, b, px, py, pz, tp, tzp, cosb;
    double cosx[25], siny[25], txp[25];
    a = pr; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
        txp[i] = tp;
        a += pr; tp += invstep;
    }
    int k = 0, kk;
    bind_texture(texId);
    glBegin(GL_TRIANGLE_FAN);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
    t[k][0] = 0; t[k][1] = vrep;
    glTexCoord2fv(t[k]);
      glVertex3fv(v[k]);
    k++;
    b = qr-M_PI/2;
    cosb = cos(b);
    pz = rz*sin(b);
    tzp = vrep-invstep2;
    px = cx+rx*cosb; py = cy;
    for (int i = 0; i <= steps; i++)
    {
        v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
        t[k][0] = txp[i]; t[k][1] = tzp;
        glTexCoord2fv(t[k]);
          glVertex3fv(v[k]);
        k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
    }
    glEnd();
    for (int ii = 0; ii < zsteps - 2; ii++)
    {
        b += qr;
        cosb = cos(b);
        pz = rz*sin(b);
        tzp -= invstep2;
        glBegin(GL_TRIANGLE_STRIP);
        px = cx+rx*cosb; py = cy;
        for (int i = 0; i <= steps; i++)
        {
            kk = k - steps - 1;
            glTexCoord2fv(t[kk]);
              glVertex3fv(v[kk]);
            v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
            t[k][0] = txp[i]; t[k][1] = tzp;
            glTexCoord2fv(t[k]);
              glVertex3fv(v[k]);
            k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
        }
        glEnd();
    }
    glBegin(GL_TRIANGLE_FAN);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
    t[k][0] = 0; t[k][1] = 0;
    glTexCoord2fv(t[k]);
      glVertex3fv(v[k]);
    k++;
    for (int i = k - steps - 2; i <= k - 2; i++)
    {
        glTexCoord2fv(t[i]);
          glVertex3fv(v[i]);
    }
    glEnd();
}

//TODO: with all basic drawing add in normals

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

#include <map>
#include <list>
#include "Universal_System/fileio.h"

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
    } //NOTE: range cannot be defined for spotlights in opengl
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
    int something;
    list<GLuint> primitive_calls;
    GLuint model;

    public:
    d3d_model(): something(100) {model = glGenLists(1);}
    ~d3d_model() {}

    void clear()
    {
        while (!primitive_calls.empty())
        {
            glDeleteLists(primitive_calls.front(), 1);
            primitive_calls.pop_front();
        }
        primitive_calls.clear();
        glDeleteLists(model, 1);
        model = glGenLists(1);
    }

    void save(string fname)
    {
      /* */
    }//removed save for now

    bool load(string fname)
    {
        int file = file_text_open_read(fname);
        if (file == -1)
            return false;
        something = file_text_read_real(file);
        if (something != 100)
            return false;
        file_text_readln(file);
        /*int calls = FIXME: POLYFUCK */file_text_read_real(file);
        file_text_readln(file);
        int kind;
        float v[3], n[3], t[2];
        double col, alpha;
        while (!file_text_eof(file))
        {
            switch (int(file_text_read_real(file)))
            {
                case  0:
                    kind = file_text_read_real(file);
                    model_primitive_begin(kind);
                    break;
                case  1:
                    model_primitive_end();
                    break;
                case  2:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     model_vertex(v);
                    break;
                case  3:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_color(v,col,alpha);
                    break;
                case  4:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     model_vertex_texture(v,t);
                    break;
                case  5:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_texture_color(v,t,col,alpha);
                    break;
                case  6:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     model_vertex_normal(v,n);
                    break;
                case  7:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_normal_color(v,n,col,alpha);
                    break;
                case  8:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     model_vertex_normal_texture(v,n,t);
                    break;
                case  9:
                     v[0] = file_text_read_real(file); v[1] = file_text_read_real(file); v[2] = file_text_read_real(file);
                     n[0] = file_text_read_real(file); n[1] = file_text_read_real(file); n[2] = file_text_read_real(file);
                     t[0] = file_text_read_real(file); t[1] = file_text_read_real(file);
                     col = file_text_read_real(file); alpha = file_text_read_real(file);
                     model_vertex_normal_texture_color(v,n,t,col,alpha);
                    break;
                case  10:
                    model_block(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file), true);
                    break;
                case  11:
                    model_cylinder(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  12:
                    model_cone(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  13:
                    model_ellipsoid(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  14:
                    model_wall(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
                case  15:
                    model_floor(file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file),file_text_read_real(file));
                    break;
            }
            file_text_readln(file);
        }
        file_text_close(file);
        return true;
    }

    void draw(double x, double y, double z, int texId)
    {
        untexture();
        bind_texture(texId);
        glPushAttrib(GL_CURRENT_BIT);
        glTranslatef(x, y, z);
        glCallList(model);
        glTranslatef(-x, -y, -z);
        glPopAttrib();
    }

    void model_primitive_begin(int kind)
    {
        GLuint index = glGenLists(1);
        primitive_calls.push_back(index);
        glNewList(index, GL_COMPILE);
        glBegin(ptypes_by_id[kind]);
    }

    void model_primitive_end()
    {
        glEnd();
        glEndList();
        glDeleteLists(model, 1);
        model = glGenLists(1);
        glNewList(model, GL_COMPILE);
        list<GLuint>::iterator i;
        for (i = primitive_calls.begin(); i != primitive_calls.end(); i++)
        {
            glCallList(*i);
        }
        glEndList();
    }

    void model_vertex(float v[])
    {
        glVertex3fv(v);
    }

    void model_vertex_color(float v[], int col, double alpha)
    {
        glColor4f(__GETR(col), __GETG(col), __GETB(col), alpha);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_texture(float v[], float t[])
    {
        glTexCoord2fv(t);
        glVertex3fv(v);
    }

    void model_vertex_texture_color(float v[], float t[], int col, double alpha)
    {
        glColor4f(__GETR(col), __GETG(col), __GETB(col), alpha);
        glTexCoord2fv(t);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_normal(float v[], float n[])
    {
        glNormal3fv(n);
        glVertex3fv(v);
    }

    void model_vertex_normal_color(float v[], float n[], int col, double alpha)
    {
        glColor4f(__GETR(col), __GETG(col), __GETB(col), alpha);
        glNormal3fv(n);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_vertex_normal_texture(float v[], float n[], float t[])
    {
        glTexCoord2fv(t);
        glNormal3fv(n);
        glVertex3fv(v);
    }

    void model_vertex_normal_texture_color(float v[], float n[], float t[], int col, double alpha)
    {
        glColor4f(__GETR(col), __GETG(col), __GETB(col), alpha);
        glTexCoord2fv(t);
        glNormal3fv(n);
        glVertex3fv(v);
        glColor4ubv(enigma::currentcolor);
    }

    void model_block(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y1, z1}, v3[] = {x2, y1, z2},
              v4[] = {x2, y2, z1}, v5[] = {x2, y2, z2}, v6[] = {x1, y2, z1}, v7[] = {x1, y2, z2},
              t0[] = {0, vrep}, t1[] = {0, 0}, t2[] = {hrep, vrep}, t3[] = {hrep, 0},
              t4[] = {hrep*2, vrep}, t5[] = {hrep*2, 0}, t6[] = {hrep*3, vrep}, t7[] = {hrep*3, 0},
              t8[] = {hrep*4, vrep}, t9[] = {hrep*4, 0};
        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_texture(v0,t0);
        model_vertex_texture(v1,t1);
        model_vertex_texture(v2,t2);
        model_vertex_texture(v3,t3);
        model_vertex_texture(v4,t4);
        model_vertex_texture(v5,t5);
        model_vertex_texture(v6,t6);
        model_vertex_texture(v7,t7);
        model_vertex_texture(v0,t8);
        model_vertex_texture(v1,t9);
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_texture(v0,t0);
            model_vertex_texture(v2,t1);
            model_vertex_texture(v6,t2);
            model_vertex_texture(v4,t3);
            model_primitive_end();
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_texture(v1,t0);
            model_vertex_texture(v3,t1);
            model_vertex_texture(v7,t2);
            model_vertex_texture(v5,t3);
            model_primitive_end();
        }
    }

    void model_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
    {
        float v[100][3];
        float t[100][3];
        steps = min(max(steps, 3), 48);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
        double a, px, py, tp;
        int k;
        model_primitive_begin(GL_TRIANGLE_STRIP);
        a = 0; px = cx+rx; py = cy; tp = 0; k = 0;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = z2;
            t[k][0] = tp; t[k][1] = 0;
            model_vertex_texture(v[k],t[k]);
            k++;
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            for (int i = 0; i <= steps*2; i+=2)
            {
                model_vertex_texture(v[i+1],t[i]);
            }
            model_primitive_end();

            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            for (int i = 0; i <= steps*2; i+=2)
            {
                model_vertex_texture(v[i],t[i]);
            }
            model_primitive_end();
        }
    }

    void model_cone(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
    {
        float v[51][3];
        float t[100][3];
        steps = min(max(steps, 3), 48);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
        double a, px, py, tp;
        int k = 0;
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
        t[k][0] = 0; t[k][1] = 0;
        model_vertex_texture(v[k],t[k]);
        k++;
        a = 0; px = cx+rx; py = cy; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = z1;
            t[k][0] = tp; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
        }
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_FAN);
            v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
            t[k][0] = 0; t[k][1] = vrep;
            model_vertex_texture(v[k],t[k]);
            k++;
            tp = 0;
            for (int i = 1; i <= steps+1; i++)
            {
                t[k][0] = tp; t[k][1] = 0;
                model_vertex_texture(v[i],t[k]);
                k++; tp += invstep;
            }
            model_primitive_end();
        }
    }

    void model_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps)
    {
        float v[277][3];
        float t[277][3];
        steps = min(max(steps, 3), 24);
        const int zsteps = ceil(steps/2);
        const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
        double a, b, px, py, pz, tp, tzp, cosb;
        double cosx[25], siny[25], txp[25];
        a = pr; tp = 0;
        for (int i = 0; i <= steps; i++)
        {
            cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
            txp[i] = tp;
            a += pr; tp += invstep;
        }
        int k = 0, kk;
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
        t[k][0] = 0; t[k][1] = vrep;
        model_vertex_texture(v[k],t[k]);
        k++;
        b = qr-M_PI/2;
        cosb = cos(b);
        pz = rz*sin(b);
        tzp = vrep-invstep2;
        px = cx+rx*cosb; py = cy;
        for (int i = 0; i <= steps; i++)
        {
            v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
            t[k][0] = txp[i]; t[k][1] = tzp;
            model_vertex_texture(v[k],t[k]);
            k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
        }
        model_primitive_end();
        for (int ii = 0; ii < zsteps - 2; ii++)
        {
            b += qr;
            cosb = cos(b);
            pz = rz*sin(b);
            tzp -= invstep2;
            model_primitive_begin(GL_TRIANGLE_STRIP);
            px = cx+rx*cosb; py = cy;
            for (int i = 0; i <= steps; i++)
            {
                kk = k - steps - 1;
                model_vertex_texture(v[kk],t[kk]);
                v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
                t[k][0] = txp[i]; t[k][1] = tzp;
                model_vertex_texture(v[k],t[k]);
                k++; px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
            }
            model_primitive_end();
        }
        model_primitive_begin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
        t[k][0] = 0; t[k][1] = 0;
        model_vertex_texture(v[k],t[k]);
        k++;
        for (int i = k - steps - 2; i <= k - 2; i++)
        {
            model_vertex_texture(v[i],t[i]);
        }
        model_primitive_end();
    }

    void model_wall(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y2, z1}, v3[] = {x2, y2, z2},
              t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_texture(v0,t0);
        model_vertex_texture(v1,t1);
        model_vertex_texture(v2,t2);
        model_vertex_texture(v3,t3);
        model_primitive_end();
    }

    void model_floor(double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y2, z1}, v2[] = {x2, y1, z2}, v3[] = {x2, y2, z2},
              t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_texture(v0,t0);
        model_vertex_texture(v1,t1);
        model_vertex_texture(v2,t2);
        model_vertex_texture(v3,t3);
        model_primitive_end();
    }
};

static map<unsigned int, d3d_model> d3d_models;
static unsigned int d3d_models_maxid = 0;

#include <iostream>
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

bool d3d_model_load(const unsigned int id, string fname)
{
    d3d_models[id].clear();
    return d3d_models[id].load(fname);
}

void d3d_model_draw(const unsigned int id, double x, double y, double z, int texId)
{
    d3d_models[id].draw(x, y, z, texId);
}

void d3d_model_primitive_begin(const unsigned int id, int kind)
{
    d3d_models[id].model_primitive_begin(kind);
}

void d3d_model_primitive_end(const unsigned int id)
{
    d3d_models[id].model_primitive_end();
}

void d3d_model_vertex(const unsigned int id, double x, double y, double z)
{
    float v[] = {x, y, z};
    d3d_models[id].model_vertex(v);
}

void d3d_model_vertex_color(const unsigned int id, double x, double y, double z, int col, double alpha)
{
    float v[] = {x, y, z};
    d3d_models[id].model_vertex_color(v, col, alpha);
}

void d3d_model_vertex_texture(const unsigned int id, double x, double y, double z, double tx, double ty)
{
    float v[] = {x, y, z}, t[] = {tx, ty};
    d3d_models[id].model_vertex_texture(v, t);
}

void d3d_model_vertex_texture_color(const unsigned int id, double x, double y, double z, double tx, double ty, int col, double alpha)
{
    float v[] = {x, y, z}, t[] = {tx, ty};
    d3d_models[id].model_vertex_texture_color(v, t, col, alpha);
}

void d3d_model_vertex_normal(const unsigned int id, double x, double y, double z, double nx, double ny, double nz)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz};
    d3d_models[id].model_vertex_normal(v, n);
}

void d3d_model_vertex_normal_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, int col, double alpha)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz};
    d3d_models[id].model_vertex_normal_color(v, n, col, alpha);
}

void d3d_model_vertex_normal_texture(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz}, t[] = {tx, ty};
    d3d_models[id].model_vertex_normal_texture(v, n, t);
}

void d3d_model_vertex_normal_texture_color(const unsigned int id, double x, double y, double z, double nx, double ny, double nz, double tx, double ty, int col, double alpha)
{
    float v[] = {x, y, z}, n[] = {nx, ny, nz}, t[] = {tx, ty};
    d3d_models[id].model_vertex_normal_texture_color(v, n, t, col, alpha);
}

void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed)
{
    d3d_models[id].model_block(x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
}

void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{
    d3d_models[id].model_cylinder(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, bool closed, int steps)
{
    d3d_models[id].model_cone(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep, int steps)
{
    d3d_models[id].model_ellipsoid(x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
}

void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
    d3d_models[id].model_wall(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, int hrep, int vrep)
{
    d3d_models[id].model_floor(x1, y1, z1, x2, y2, z2, hrep, vrep);
}
