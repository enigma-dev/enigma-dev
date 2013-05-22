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
#include "GLd3d.h"
#include "../General/GLtextures.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/GLbinding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

bool d3dMode = false;
bool d3dHidden = true;
bool d3dZWriteEnable = true;
double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

GLenum renderstates[22] = {
  GL_FILL, GL_LINE, GL_POINT, GL_FRONT, GL_BACK,
  GL_FRONT_AND_BACK, GL_CW, GL_CCW,
  GL_NICEST, GL_FASTEST, GL_DONT_CARE, GL_EXP, GL_EXP2,
  GL_LINEAR, GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL,
  GL_GREATER, GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS
};

namespace enigma_user
{

void d3d_start()
{
  // Set global ambient lighting to nothing.
  float global_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // Enable depth buffering
  d3dMode = true;
  d3dHidden = true;
  d3dZWriteEnable = true;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_NOTEQUAL, 0);
  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

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
  glDisable(GL_ALPHA_TEST);
  glDisable(GL_NORMALIZE);
  glDisable(GL_COLOR_MATERIAL);
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
    d3dHidden = enable;
}

void d3d_set_zwriteenable(bool enable)
{
    (enable?glEnable:glDisable)(GL_DEPTH_TEST);
    d3dZWriteEnable = enable;
}

void d3d_set_lighting(bool enable)
{
    (enable?glEnable:glDisable)(GL_LIGHTING);
}

void d3d_set_fog(bool enable, int color, double start, double end)
{
  d3d_set_fog_enabled(enable);
  d3d_set_fog_color(color);
  d3d_set_fog_start(start);
  d3d_set_fog_end(end);
  d3d_set_fog_hint(rs_nicest);
  d3d_set_fog_mode(rs_linear);
}//NOTE: fog can use vertex checks with less good graphic cards which screws up large textures (however this doesn't happen in directx)

void d3d_set_fog_enabled(bool enable)
{
  (enable?glEnable:glDisable)(GL_FOG);
}

void d3d_set_fog_mode(int mode)
{
  glFogi(GL_FOG_MODE, renderstates[mode]);
}

void d3d_set_fog_hint(int mode) {
  glHint(GL_FOG_HINT, mode);
}

void d3d_set_fog_color(int color)
{
   GLfloat fog_color[3];
   fog_color[0] = __GETR(color);
   fog_color[1] = __GETG(color);
   fog_color[2] = __GETB(color);
   glFogfv(GL_FOG_COLOR, fog_color);
}

void d3d_set_fog_start(double start)
{
  glFogf(GL_FOG_START, start);
}

void d3d_set_fog_end(double end)
{
  glFogf(GL_FOG_END, end);
}

void d3d_set_fog_density(double density)
{
  glFogf(GL_FOG_DENSITY, density);
}

void d3d_set_culling(bool enable)
{
  (enable?glEnable:glDisable)(GL_CULL_FACE);
  glFrontFace(GL_CW);
}

void d3d_set_culling_mode(int mode) {
  glCullFace(renderstates[mode]);
}

void d3d_set_culling_orientation(int mode) {
  glFrontFace(renderstates[mode]);
}

void d3d_set_render_mode(int face, int fill)
{
  glPolygonMode(renderstates[face], renderstates[fill]);
}
void d3d_set_line_width(float value) {
  glLineWidth(value);
}

void d3d_set_point_size(float value) {
  glPointSize(value);
}

void d3d_depth_clear() {
  d3d_depth_clear_value(1.0f);
}

void d3d_depth_clear_value(float value) {
  glClearDepthf(value);
}

void d3d_depth_operator(int mode) {
  glDepthFunc(renderstates[mode]);
}

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

}

extern GLenum ptypes_by_id[16];
namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

void d3d_set_projection(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup)
{
  (d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  (d3dZWriteEnable?glEnable:glDisable)(GL_DEPTH_TEST);
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

void d3d_set_projection_ext(double xfrom,double yfrom,double zfrom,double xto,double yto,double zto,double xup,double yup,double zup,double angle,double aspect,double znear,double zfar)
{
  (d3dHidden?glEnable:glDisable)(GL_DEPTH_TEST);
  (d3dZWriteEnable?glEnable:glDisable)(GL_DEPTH_TEST);
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
  enigma::d3d_light_update_positions();
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
  enigma::d3d_light_update_positions();
}
void d3d_draw_wall(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep)
{
  if ((x1 == x2 && y1 == y2) || z1 == z2) {
    return;
  }

  float xd = x2-x1, yd = y2-y1, zd = z2-z1;
  float normal[3] = {xd*zd, zd*yd, 0};
  float mag = hypot(normal[0], normal[1]);
  normal[0] /= mag;
  normal[1] /= mag;
  if (x2 < x1) {
    normal[0]=-normal[0]; }
  if (y2 < y1) {
    normal[1]=-normal[1]; }

  GLfloat verts[] = {x1, y1, z1, x2, y2, z1, x1, y1, z2, x2, y2, z2},
          texts[] = {0, 0, hrep, 0, 0, vrep, hrep, vrep},
          norms[] = {normal[0], normal[1], normal[2], normal[0], normal[1], normal[2],
                     normal[0], normal[1], normal[2], normal[0], normal[1], normal[2]};

  GLubyte indices[] = {0, 1, 2, 3};

  if (x2>x1 || y2>y1) {
    indices[0] = 0;
  } else {
    indices[0] = 3;
  }

  if (x2<x1 || y2<y1) {
    indices[1] = 2;
  } else {
    indices[1] = 1;
  }

  if (x2<x1 || y2<y1) {
    indices[2] = 1;
  } else {
    indices[2] = 2;
  }

  if (x2>x1 || y2>y1) {
    indices[3] = 3;
  } else {
    indices[3] = 0;
  }

  texture_use(get_texture(texId));

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 4, 4, GL_UNSIGNED_BYTE, indices);
}

void d3d_draw_floor(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep)
{
  GLfloat verts[] = {x1, y1, z1, x2, y1, z2, x1, y2, z1, x2, y2, z2},
          texts[] = {0, 0, 0, vrep, hrep, 0, hrep, vrep},
          norms[] = {0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1};
  GLubyte ceil_indices[] = {0, 2, 1, 3};
  GLubyte floor_indices[] = {3, 1, 2, 0};

  texture_use(get_texture(texId));

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  if (x2>x1 || y2>y1) {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 4, 4, GL_UNSIGNED_BYTE, ceil_indices);
  } else {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 4, 4, GL_UNSIGNED_BYTE, floor_indices);
  }
}

void d3d_draw_block(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep, bool closed)
{
  GLfloat verts[] = {x1, y1, z1, x1, y1, z2, x1, y2, z1, x1, y2, z2, x2, y2, z1, x2, y2, z2, x2, y1, z1, x2, y1, z2},
          texts[] = {0, vrep, hrep, vrep, 0, 0, hrep, 0,
		     0, vrep, hrep, vrep, 0, 0, hrep, 0},
	  norms[] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5,
                     0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5};
  GLubyte indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, // sides
                       0, 2, 6, 4, 1, 7, 3, 5}; // top and bottom

  texture_use(get_texture(texId));
 // glClientActiveTexture(GL_TEXTURE0);

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  if (closed) {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 8, 18, GL_UNSIGNED_BYTE, indices);
  } else {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 8, 10, GL_UNSIGNED_BYTE, indices);
  }
}

void d3d_draw_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep, bool closed, int steps)
{
    float v[100][3];
    float t[100][3];
    steps = min(max(steps, 3), 48);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
    double a, px, py, tp;
    int k;
  texture_use(get_texture(texId));
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

void d3d_draw_cone(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep, bool closed, int steps)
{
    steps = min(max(steps, 3), 48);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
    float v[(steps + 1)*3 + 1][3];
    float t[(steps + 1)*3 + 1][2];
    double a, px, py, tp;
    int k = 0;
    texture_use(get_texture(texId));
    glBegin(GL_TRIANGLE_STRIP);
    a = 0; px = cx+rx; py = cy; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
        t[k][0] = tp; t[k][1] = vrep;
        glTexCoord2fv(t[k]);
        glVertex3fv(v[k]);
        k += steps + 1;
        v[k][0] = px; v[k][1] = py; v[k][2] = z1;
        t[k][0] = tp; t[k][1] = 0;
        glTexCoord2fv(t[k]);
        glVertex3fv(v[k]);
        k -= steps + 1;
        k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    k += steps + 1;
    glEnd();
    if (closed)
    {
        glBegin(GL_TRIANGLE_FAN);
        v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
        t[k][0] = 0; t[k][1] = 0;
        glTexCoord2fv(t[k]);
        glVertex3fv(v[k]);
        k++;
        tp = 0;
        for (int i = steps; i >= 0; i--)
        {
            v[k][0] = v[i + steps + 1][0]; v[k][1] = v[i + steps + 1][1]; v[k][2] = v[i + steps + 1][2];
            t[k][0] = 0; t[k][1] = 0;
            glTexCoord2fv(t[k]);
            glVertex3fv(v[k]);
            k++; tp += invstep;
        }
        glEnd();
    }
}

void d3d_draw_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, int texId, double hrep, double vrep, int steps)
{
    steps = min(max(steps, 3), 24);
    const int zsteps = ceil(steps/2.0);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
    float v[(steps+2)*(zsteps+2)][3];
    float t[(steps+2)*(zsteps+2)][2];
    double a, b, px, py, pz, tp, tzp, cosb;
    double cosx[steps+1], siny[steps+1], txp[steps+1];
    a = M_PI; tp = 0;
    for (int i = 0; i <= steps; i++)
    {
        cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
        txp[i] = tp;
        a += pr; tp += invstep;
    }
    int k = 0, kk;
    texture_use(get_texture(texId));
    b = M_PI/2;
    cosb = cos(b);
    pz = rz*sin(b);
    tzp = 0.0;
    for (int i = 0; i <= steps; i++)
    {
        px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
        v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
        t[k][0] = txp[i]; t[k][1] = tzp;
        k++;
    }
    for (int ii = 0; ii < zsteps; ii++)
    {
        b += qr;
        cosb = cos(b);
        pz = rz*sin(b);
        tzp += invstep2;
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= steps; i++)
        {
            px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
            kk = k - steps - 1;
            glTexCoord2fv(t[kk]);
            glVertex3fv(v[kk]);
            v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
            t[k][0] = txp[i]; t[k][1] = tzp;
            glTexCoord2fv(t[k]);
            glVertex3fv(v[k]);
            k++;
        }
        glEnd();
    }
}

void d3d_draw_icosahedron() {


}

void d3d_draw_torus(double x1, double y1, double z1, int texId, double hrep, double vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI) {
        int numc = csteps, numt = tsteps;
	texture_use(get_texture(texId));
        for (int i = 0; i < numc; i++) {
            glBegin(GL_QUAD_STRIP);

            for (int j = 0; j <= numt; j++) {
                for (int k = 1; k >= 0; k--) {

                    double s = (i + k) % numc + 0.5;
                    double t = j % numt;

                    double x = (radius + tradius * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
                    double y = (radius + tradius * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
                    double z = tradius * sin(s * TWOPI / numc);
		    double u = (i + k) / (float)numc;
		    double v = t / (float)numt;

		    glTexCoord2f(v, u);
                    glVertex3f(x1 + x, y1 + y, z1 + z);
                }
            }
	    glEnd();
        }
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

#include <map>
#include <list>
#include "Universal_System/fileio.h"

struct posi { // Homogenous point.
    double x;
    double y;
    double z;
    double w;
    posi(double x1, double y1, double z1, double w1) : x(x1), y(y1), z(z1), w(w1){}
};

class d3d_lights
{
    map<int,int> light_ind;
    map<int,posi> ind_pos; // Internal index to position.

    public:
    d3d_lights() {}
    ~d3d_lights() {}

    void light_update_positions()
    {
        map<int, posi>::iterator end = ind_pos.end();
        for (map<int, posi>::iterator it = ind_pos.begin(); it != end; it++) {
            const posi pos1 = (*it).second;
            const float pos[4] = {pos1.x, pos1.y, pos1.z, pos1.w};
            glLightfv(GL_LIGHT0+(*it).first, GL_POSITION, pos);
        }
    }

    bool light_define_direction(int id, double dx, double dy, double dz, int col)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(-dx, -dy, -dz, 0.0f)));
        }

        const float dir[4] = {-dx, -dy, -dz, 0.0f}, color[4] = {__GETR(col), __GETG(col), __GETB(col), 1};
        glLightfv(GL_LIGHT0+ms, GL_POSITION, dir);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);
        light_update_positions();
        return true;
    }

    bool light_define_point(int id, double x, double y, double z, double range, int col)
    {
        if (range <= 0.0) {
            return false;
        }
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
            multimap<int,posi>::iterator it = ind_pos.find(ms);
            if (it != ind_pos.end())
                ind_pos.erase(it);
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;

            light_ind.insert(pair<int,int>(id, ms));
            ind_pos.insert(pair<int,posi>(ms, posi(x, y, z, 1)));
        }
        const float pos[4] = {x, y, z, 1}, color[4] = {__GETR(col), __GETG(col), __GETB(col), 1},
            specular[4] = {0, 0, 0, 0}, ambient[4] = {0, 0, 0, 0};
        glLightfv(GL_LIGHT0+ms, GL_POSITION, pos);
        glLightfv(GL_LIGHT0+ms, GL_DIFFUSE, color);
        glLightfv(GL_LIGHT0+ms, GL_SPECULAR, specular);
        glLightfv(GL_LIGHT0+ms, GL_AMBIENT, ambient);
        // Limit the range of the light through attenuation.
        glLightf(GL_LIGHT0+ms, GL_CONSTANT_ATTENUATION, 1.0);
        glLightf(GL_LIGHT0+ms, GL_LINEAR_ATTENUATION, 0.0);
        // 48 is a number gotten through manual calibration. Make it lower to increase the light power.
        const double attenuation_calibration = 48.0;
        glLightf(GL_LIGHT0+ms, GL_QUADRATIC_ATTENUATION, attenuation_calibration/(range*range));
        return true;
    }

    bool light_define_specularity(int id, int r, int g, int b, double a)
    {
        int ms;
        if (light_ind.find(id) != light_ind.end())
        {
            ms = (*light_ind.find(id)).second;
        }
        else
        {
            ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
                return false;
        }
        float specular[4] = {r, g, b, a};
        glLightfv(GL_LIGHT0+ms, GL_SPECULAR, specular);
        return true;
    }

    bool light_enable(int id)
    {
        map<int, int>::iterator it = light_ind.find(id);
        if (it == light_ind.end())
        {
            const int ms = light_ind.size();
            int MAX_LIGHTS;
            glGetIntegerv(GL_MAX_LIGHTS, &MAX_LIGHTS);
            if (ms >= MAX_LIGHTS)
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

namespace enigma_user
{

bool d3d_light_define_direction(int id, double dx, double dy, double dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, double x, double y, double z, double range, int col)
{
    return d3d_lighting.light_define_point(id, x, y, z, range, col);
}

bool d3d_light_define_specularity(int id, int r, int g, int b, double a)
{
    return d3d_lighting.light_define_specularity(id, r, g, b, a);
}

void d3d_light_specularity(int facemode, int r, int g, int b, double a)
{
  float specular[4] = {r, g, b, a};
  glMaterialfv(renderstates[facemode], GL_SPECULAR, specular);
}

void d3d_light_shininess(int facemode, int shine)
{
  glMateriali(renderstates[facemode], GL_SHININESS, shine);
}

void d3d_light_define_ambient(int col)
{
    const float color[4] = {__GETR(col), __GETG(col), __GETB(col), 1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, color);
}

bool d3d_light_enable(int id, bool enable)
{
    return enable?d3d_lighting.light_enable(id):d3d_lighting.light_disable(id);
}

}

namespace enigma {
    void d3d_light_update_positions()
    {
        d3d_lighting.light_update_positions();
    }
}

#include "Universal_System/estring.h"
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
    }//TODO: format needs to be decided on

    bool load(string fname)  //TODO: this needs to be rewritten properly not using the file_text functions
    {
        int file = file_text_open_read(fname);
        if (file == -1)
            return false;
        something = file_text_read_real(file);
        if (something != 100)
            return false;
        file_text_readln(file);
        file_text_read_real(file);  //don't see the use in this value, it doesn't equal the number of calls left exactly
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
        texture_reset();
        texture_use(get_texture(texId));
        glPushAttrib(GL_CURRENT_BIT);
        glLoadIdentity();
        glLoadMatrixd(projection_matrix);
        glTranslatef(x, y, z);
        glMultMatrixd(transformation_matrix);
        glCallList(model);
        glLoadIdentity();
        glLoadMatrixd(projection_matrix);
        glMultMatrixd(transformation_matrix);
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
    //TODO: Model basic shapes need to be changed to the basic new oens
    void model_block(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed)
    {
        float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y1, z1}, v3[] = {x2, y1, z2},
              v4[] = {x2, y2, z1}, v5[] = {x2, y2, z2}, v6[] = {x1, y2, z1}, v7[] = {x1, y2, z2},
              t0[] = {0, vrep}, t1[] = {0, 0}, t2[] = {hrep, vrep}, t3[] = {hrep, 0},
              t4[] = {hrep*2, vrep}, t5[] = {hrep*2, 0}, t6[] = {hrep*3, vrep}, t7[] = {hrep*3, 0},
              t8[] = {hrep*4, vrep}, t9[] = {hrep*4, 0},
	      n0[] = {-0.5, -0.5, -0.5}, n1[] = {-0.5, -0.5, 0.5}, n2[] = {-0.5, 0.5, -0.5}, n3[] = {-0.5, 0.5, 0.5},
              n4[] = {0.5, 0.5, -0.5}, n5[] = {0.5, 0.5, 0.5}, n6[] = {0.5, -0.5, -0.5}, n7[] = {0.5, -0.5, 0.5};

        model_primitive_begin(GL_TRIANGLE_STRIP);
        model_vertex_normal_texture(v0,n0,t0);
        model_vertex_normal_texture(v1,n1,t1);
        model_vertex_normal_texture(v6,n2,t2);
        model_vertex_normal_texture(v7,n3,t3);
        model_vertex_normal_texture(v4,n4,t4);
        model_vertex_normal_texture(v5,n5,t5);
        model_vertex_normal_texture(v2,n6,t8);
        model_vertex_normal_texture(v3,n7,t9);
        model_vertex_normal_texture(v0,n0,t6);
        model_vertex_normal_texture(v1,n1,t7);
        model_primitive_end();
        if (closed)
        {
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_normal_texture(v0,n4,t0);
            model_vertex_normal_texture(v2,n6,t1);
            model_vertex_normal_texture(v6,n2,t2);
            model_vertex_normal_texture(v4,n0,t3);
            model_primitive_end();
            model_primitive_begin(GL_TRIANGLE_STRIP);
            model_vertex_normal_texture(v1,n1,t0);
            model_vertex_normal_texture(v3,n7,t1);
            model_vertex_normal_texture(v7,n3,t2);
            model_vertex_normal_texture(v5,n5,t3);
            model_primitive_end();
        }
    }

    void model_cylinder(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
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

    void model_cone(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
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
        for (int i = steps + 1; i >= 0; i--)
        {
                t[k][0] = tp; t[k][1] = 0;
                model_vertex_texture(v[i],t[k]);
                k++; tp += invstep;
            }
            model_primitive_end();
        }
    }

    void model_ellipsoid(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, int steps)
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

    void model_icosahedron()
    {

    }

    void model_torus(double x1, double y1, double z1, double hrep, double vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI = 2*3.14)
    {
        int numc = csteps, numt = tsteps;

        for (int i = 0; i < numc; i++) {
            model_primitive_begin(GL_QUAD_STRIP);
            for (int j = 0; j <= numt; j++) {
                for (int k = 1; k >= 0; k--) {

                    double s = (i + k) % numc + 0.5;
                    double t = j % numt;

                    double x = (radius + tradius * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
                    double y = (radius + tradius * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
                    double z = tradius * sin(s * TWOPI / numc);
		    double u = (i + k) / (float)numc;
		    double v = t / (float)numt;

		    float ver[] = {x1 + x, y1 + y, z1 + z}, uv[] = {u, v};
		    model_vertex_texture(ver, uv);
                }
            }
	    model_primitive_end();
        }
    }

    void model_wall(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
    {
    if ((x1 == x2 && y1 == y2) || z1 == z2) {
        return;
    }
    float v0[] = {x1, y1, z1}, v1[] = {x1, y1, z2}, v2[] = {x2, y2, z1}, v3[] = {x2, y2, z2},
          t0[] = {0, 0}, t1[] = {0, vrep}, t2[] = {hrep, 0}, t3[] = {hrep, vrep};
    model_primitive_begin(GL_TRIANGLE_STRIP);

    const float xd = x1-x2, yd = y2-y1, zd = z2-z1;
    const float usize = fabs(zd), vsize = hypotf(xd, yd);
    const float uz = zd/usize, vx = xd/vsize, vy = yd/vsize;
    glNormal3f(uz*vy, uz*vx, 0);

    if (x2>x1 || y2>y1) {
      model_vertex_texture(v0,t0);
    } else {
      model_vertex_texture(v3,t3);
    }
    if (x1>x2 || y1>y2) {
      model_vertex_texture(v1,t1);
    } else {
      model_vertex_texture(v2,t2);
    }
    if (x1>x2 || y1>y2) {
      model_vertex_texture(v2,t2);
    } else {
      model_vertex_texture(v1,t1);
    }
    if (x2>x1 || y2>y1) {
      model_vertex_texture(v3,t3);
    } else {
      model_vertex_texture(v0,t0);
    }
    model_primitive_end();
    }

    void model_floor(double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
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

namespace enigma_user
{

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

void d3d_model_block(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed)
{
    d3d_models[id].model_block(x1, y1, z1, x2, y2, z2, hrep, vrep, closed);
}

void d3d_model_cylinder(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
{
    d3d_models[id].model_cylinder(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_cone(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, bool closed, int steps)
{
    d3d_models[id].model_cone(x1, y1, z1, x2, y2, z2, hrep, vrep, closed, steps);
}

void d3d_model_ellipsoid(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep, int steps)
{
    d3d_models[id].model_ellipsoid(x1, y1, z1, x2, y2, z2, hrep, vrep, steps);
}

void d3d_model_icosahedron(const unsigned int id)
{
    d3d_models[id].model_icosahedron();
}

void d3d_model_torus(const unsigned int id, double x1, double y1, double z1, double hrep, double vrep, int csteps, int tsteps, double radius, double tradius, double TWOPI)
{
    d3d_models[id].model_torus(x1, y1, z1, hrep, vrep, csteps, tsteps, radius, tradius, TWOPI);
}

void d3d_model_wall(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
{
    d3d_models[id].model_wall(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

void d3d_model_floor(const unsigned int id, double x1, double y1, double z1, double x2, double y2, double z2, double hrep, double vrep)
{
    d3d_models[id].model_floor(x1, y1, z1, x2, y2, z2, hrep, vrep);
}

}

