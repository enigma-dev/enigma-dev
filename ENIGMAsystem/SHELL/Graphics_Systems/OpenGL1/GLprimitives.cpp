/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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
#include "../General/GStextures.h"
#include "../General/GSprimitives.h"
#include "GLTextureStruct.h"

#include <string>
#include "Widget_Systems/widgets_mandatory.h"

#define GETR(x) ((x & 0x0000FF))
#define GETG(x) ((x & 0x00FF00)>>8)
#define GETB(x) ((x & 0xFF0000)>>16)

#include <math.h>

using namespace std;

#include <floatcomp.h>

GLenum ptypes_by_id[16] = {
  GL_POINTS, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
  GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_POINTS, GL_POINTS,
  GL_POINTS, GL_POINTS,

  //These are padding.
  GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS
};

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

void draw_primitive_begin(int kind)
{
  // This has to be done because these immediate mode vertex functions will
  // blend with the texture whether or specify texture coordinates or not.
  texture_reset();
  glBegin(ptypes_by_id[ kind & 15 ]);
}

void draw_primitive_begin_texture(int kind, int tex)
{
  texture_set(tex);
  glBegin(ptypes_by_id[ kind & 15 ]);
}

void draw_primitive_end() {
  glEnd();
}

void draw_vertex(gs_scalar x, gs_scalar y)
{
  glVertex2f(x,y);
}

void draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
{
    glColor4ub(
      (col&0xFF),
      ((col&0xFF00)>>8),
      ((col&0xFF0000)>>16),
      (unsigned char)(alpha*255));
    glVertex2f(x,y);
    glColor4ubv(enigma::currentcolor);
}

void draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
}

void draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
{
    glColor4ub(
      (col&0xFF),
      ((col&0xFF00)>>8),
      ((col&0xFF0000)>>16),
      (unsigned char)(alpha*255));
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
    glColor4ubv(enigma::currentcolor);
}

void d3d_primitive_begin(int kind) {
	// This has to be done because these immediate mode vertex functions will
	// blend with the texture whether or specify texture coordinates or not.
	texture_reset();
  glBegin(ptypes_by_id[kind]);
}

void d3d_primitive_begin_texture(int kind, int texId) {
  texture_set(texId);
  glBegin(ptypes_by_id[kind]);
}

void d3d_primitive_end() {
  glEnd();
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z) {
  glVertex3d(x,y,z);
}

void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha) {
  glColor4ub(GETR(color), GETG(color), GETB(color), (unsigned char)(alpha*255));
  glVertex3d(x,y,z);
  glColor4ubv(enigma::currentcolor);
}

void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty) {
  glTexCoord2f(tx,ty);
  glVertex3d(x,y,z);
}

void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha) {
  glColor4ub(GETR(color), GETG(color), GETB(color), (unsigned char)(alpha*255));
  glTexCoord2f(tx,ty);
  glVertex3d(x,y,z);
  glColor4ubv(enigma::currentcolor);
}

void d3d_vertex_normal(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz)
{
  glNormal3f(nx, ny, nz);
  glVertex3d(x,y,z);
}

void d3d_vertex_normal_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, int color, double alpha)
{
  glColor4ub(GETR(color), GETG(color), GETB(color), (unsigned char)(alpha*255));
  glNormal3f(nx, ny, nz);
  glVertex3d(x,y,z);
  glColor4ubv(enigma::currentcolor);
}

void d3d_vertex_normal_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty)
{
  glTexCoord2f(tx,ty);
  glNormal3f(nx, ny, nz);
  glVertex3d(x,y,z);
}

void d3d_vertex_normal_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar nx, gs_scalar ny, gs_scalar nz, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
  glColor4ub(GETR(color), GETG(color), GETB(color), (unsigned char)(alpha*255));
  glTexCoord2f(tx,ty);
  glNormal3f(nx, ny, nz);
  glVertex3d(x,y,z);
  glColor4ubv(enigma::currentcolor);
}

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  GLfloat verts[] = {x1, y1, z1, x1, y1, z2, x1, y2, z1, x1, y2, z2, x2, y2, z1, x2, y2, z2, x2, y1, z1, x2, y1, z2},
          texts[] = {0, vrep, hrep, vrep, 0, 0, hrep, 0,
                     0, vrep, hrep, vrep, 0, 0, hrep, 0},
          norms[] = {-0.5, -0.5, -0.5, -0.5, -0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5,
                           0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5};
  GLubyte indices[] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, // sides
                       0, 2, 6, 4, 1, 7, 3, 5}; // top and bottom

  texture_set(texId);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  if (closed) {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 8, 18, GL_UNSIGNED_BYTE, indices);
  } else {
    glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 8, 10, GL_UNSIGNED_BYTE, indices);
  }

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void d3d_draw_floor(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
  gs_scalar nX = (y2-y1)*(z2-z1)*(z2-z1);
  gs_scalar nY = (z2-z1)*(x2-x1)*(x2-x1);
  gs_scalar nZ = (x2-x1)*(y2-y1)*(y2-y1);

  gs_scalar  m = sqrt(nX*nX + nY*nY + nZ*nZ);
  nX /= m; nY /= m; nZ /= m;

  GLfloat verts[] = {x1, y1, z1, x1, y2, z1, x2, y1, z2, x2, y2, z2},
          texts[] = {0, 0, 0, vrep, hrep, 0, hrep, vrep},
          norms[] = {-nX, nY, nZ, -nX, nY, nZ, -nX, nY, nZ, -nX, nY, nZ};
  GLubyte floor_indices[] = {0, 1, 2, 3};

  texture_set(texId);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 4, 4, GL_UNSIGNED_BYTE, floor_indices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void d3d_draw_wall(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep)
{
  gs_scalar nX = (y2-y1)*(z2-z1)*(z2-z1);
  gs_scalar nY = (z2-z1)*(x2-x1)*(x2-x1);
  gs_scalar nZ = (x2-x1)*(y2-y1)*(y2-y1);

  gs_scalar  m = sqrt(nX*nX + nY*nY + nZ*nZ);
  nX /= m; nY /= m; nZ /= m;

  GLfloat verts[] = {x1, y1, z1, x2, y2, z1, x1, y1, z2, x2, y2, z2},
          texts[] = {0, 0, hrep, 0, 0, vrep, hrep, vrep},
          norms[] = {nX, nY, nZ, nX, nY, nZ, nX, nY, nZ, nX, nY, nZ};

  GLubyte indices[] = {0, 1, 2, 3};

  texture_set(texId);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, verts);
  glNormalPointer(GL_FLOAT, 0, norms);
  glTexCoordPointer(2, GL_FLOAT, 0, texts);

  glDrawRangeElements(GL_TRIANGLE_STRIP, 0, 4, 4, GL_UNSIGNED_BYTE, indices);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  float v[100][3];
  float n[100][3];
  float t[100][3];
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k;
  
  texture_set(texId);
  
  //SIDES
  glBegin(GL_TRIANGLE_STRIP);
  a = 0; px = cx+rx; py = cy; tp = 0; k = 0;
  for (int i = 0; i <= steps; i++)
  {
    v[k][0] = px; v[k][1] = py; v[k][2] = z2;
    n[k][0] = cos(a); n[k][1] = sin(a); n[k][2] = 0;
    t[k][0] = tp; t[k][1] = 0;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k++;
    v[k][0] = px; v[k][1] = py; v[k][2] = z1;
    n[k][0] = cos(a); n[k][1] = sin(a); n[k][2] = 0;
    t[k][0] = tp; t[k][1] = vrep;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  glEnd();
  if (closed)
  {
    //BOTTOM
    glBegin(GL_TRIANGLE_FAN);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
    n[k][0] = 0; n[k][1] = 0; n[k][2] = -1;
    t[k][0] = 0; t[k][1] = vrep;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k++;
    for (int i = 0; i <= steps*2; i+=2)
    {
      glTexCoord2fv(t[i]);
      glNormal3fv(n[k]);
      glVertex3fv(v[i+1]);
    }
    glEnd();

    //TOP
    glBegin(GL_TRIANGLE_FAN);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
    n[k][0] = 0; n[k][1] = 0; n[k][2] = 1;
    t[k][0] = 0; t[k][1] = vrep;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k++;
    for (int i = 0; i <= steps*2; i+=2)
    {
      glTexCoord2fv(t[i]);
      glNormal3fv(n[i]);
      glVertex3fv(v[i]);
    }
    glEnd();
  }
}

void d3d_draw_cone(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  float v[(steps + 1)*3 + 1][3];
  float n[(steps + 1)*3 + 1][3];
  float t[(steps + 1)*3 + 1][2];
  double a, px, py, tp;
  int k = 0;
  
  texture_set(texId);
  
  glBegin(GL_TRIANGLE_STRIP);
  a = 0; px = cx+rx; py = cy; tp = 0;
  for (int i = 0; i <= steps; i++)
  {
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
    n[k][0] = cos(a); n[k][1] = sin(a); n[k][2] = 0;
    t[k][0] = tp; t[k][1] = vrep;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k += steps + 1;
    v[k][0] = px; v[k][1] = py; v[k][2] = z1;
    n[k][0] = cos(a); n[k][1] = sin(a); n[k][2] = 0;
    t[k][0] = tp; t[k][1] = 0;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
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
    n[k][0] = 0; n[k][1] = 0; n[k][2] = -1;
    t[k][0] = 0; t[k][1] = 0;
    glTexCoord2fv(t[k]);
    glNormal3fv(n[k]);
    glVertex3fv(v[k]);
    k++;
    tp = 0;
    for (int i = steps; i >= 0; i--)
    {
      v[k][0] = v[i + steps + 1][0]; v[k][1] = v[i + steps + 1][1]; v[k][2] = v[i + steps + 1][2];
      n[k][0] = 0; n[k][1] = 0; n[k][2] = -1;
      t[k][0] = 0; t[k][1] = 0;
      glTexCoord2fv(t[k]);
      glNormal3fv(n[k]);
      glVertex3fv(v[k]);
      k++; tp += invstep;
    }
    glEnd();
  }
}

void d3d_draw_ellipsoid(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{
  steps = min(max(steps, 3), 24);
  const int zsteps = ceil(steps/2.0);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
  float v[(steps+2)*(zsteps+2)][3];
  float n[(steps+2)*(zsteps+2)][3];
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
  
  texture_set(texId);
  
  b = M_PI/2;
  cosb = cos(b);
  pz = rz*sin(b);
  tzp = 0.0;
  for (int i = 0; i <= steps; i++)
  {
    px = cx+cosx[i]*cosb; py = cy+siny[i]*cosb;
    v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
    n[k][0] = -cosx[i]; n[k][1] = -siny[i]; n[k][2] = -sin(b);
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
      glNormal3fv(n[kk]);
      glVertex3fv(v[kk]);
      v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
      n[k][0] = -cosx[i]; n[k][1] = -siny[i]; n[k][2] = -sin(b);
      t[k][0] = txp[i]; t[k][1] = tzp;
      glTexCoord2fv(t[k]);
      glNormal3fv(n[k]);
      glVertex3fv(v[k]);
      k++;
    }
    glEnd();
  }
}

void d3d_draw_icosahedron(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, int steps)
{
  // gs_scalar width = (x2-x1), length = (y2-y1), height = (z2-z1);  UNUSED
  static gs_scalar vdata[12][3] = {
   {0, 0.5, 1}, {1, 0.5, 1}, {0, 0.5, 0}, {1, 0.5, 0},
   {0.5, 1, 1}, {0.5, 1, 0}, {0.5, 0, 1}, {0.5, 0, 0},
   {1, 1, 0.5}, {0, 1, 0.5}, {1, 0, 0.5}, {0, 0, 0.5}
  };

  static int tindices[20][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

  texture_set(texId);
  glBegin(GL_TRIANGLES);
  for (unsigned i = 0; i < 20; i++) {
    glVertex3fv(&vdata[tindices[i][0]][0]);
    glVertex3fv(&vdata[tindices[i][1]][0]);
    glVertex3fv(&vdata[tindices[i][2]][0]);
    // TODO: Add normals, uv's, and subdivide by the number of steps
  }
  glEnd();
}

void d3d_draw_torus(gs_scalar x1, gs_scalar y1, gs_scalar z1, int texId, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius) {
	texture_set(texId);
	double TWOPI = 2 * (double)M_PI;

  for (int i = 0; i < csteps; i++) {
    glBegin(GL_QUAD_STRIP);
    for (int j = 0; j <= tsteps; j++) {
      for (int k = 1; k >= 0; k--) {
        double s = (i + k) % csteps + 0.5;
        double t = j % tsteps;

        double x = (radius + tradius * cos(s * TWOPI / csteps)) * cos(t * TWOPI / tsteps);
        double y = (radius + tradius * cos(s * TWOPI / csteps)) * sin(t * TWOPI / tsteps);
        double z = tradius * sin(s * TWOPI / csteps);
        double u = ((i + k) / (float)csteps) * hrep;
        double v = (j / (float)tsteps) * vrep;

        gs_scalar nX = cos(s * TWOPI / csteps) * cos(t * TWOPI / tsteps);
        gs_scalar nY = cos(s * TWOPI / csteps) * sin(t * TWOPI / tsteps);
        gs_scalar nZ = sin(s * TWOPI / csteps);

        glTexCoord2f(v, u);
        glNormal3f(nX, nY, nZ);
        glVertex3f(x1 + x, y1 + y, z1 + z);
      }
    }
    glEnd();
  }
}

}

