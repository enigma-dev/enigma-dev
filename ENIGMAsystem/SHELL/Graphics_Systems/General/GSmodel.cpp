/** Copyright (C) 2017 Robert Colton
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

#include "GSmodel.h"
#include "GSprimitives.h"

#include <math.h>

using namespace std;

namespace enigma_user {

void d3d_model_floor(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep) {
  // Setup U and V vectors
  gs_scalar vX = x2 - x1;
  gs_scalar vY = y2 - y1;
  gs_scalar vZ = z2 - z1;
  gs_scalar wX = x2 - x1;
  gs_scalar wZ = z2 - z1;

  // Cross-Product of vectors to create normal vector
  gs_scalar nX = (vY * wZ);
  gs_scalar nY = (vZ * wX) - (vX * wZ);
  gs_scalar nZ = -(vY * wX);

  // Normalize result
  gs_scalar mag = sqrt((nX*nX) + (nY*nY) + (nZ*nZ));
  nX /= mag, nY /= mag, nZ /= mag;

  // Create floor
  d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture(id, x1, y1, z1, nX, nY, nZ, 0,    0);
    d3d_model_vertex_normal_texture(id, x1, y2, z1, nX, nY, nZ, 0,    vrep);
    d3d_model_vertex_normal_texture(id, x2, y1, z2, nX, nY, nZ, hrep, 0);
    d3d_model_vertex_normal_texture(id, x2, y2, z2, nX, nY, nZ, hrep, vrep);
  d3d_model_primitive_end(id);
}

void d3d_model_wall(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep) {
  // Setup U and V vectors
  gs_scalar vX = x2 - x1;
  gs_scalar vY = y2 - y1;
  gs_scalar wZ = z2 - z1;

  // Cross-Product of vectors to create normal vector
  gs_scalar nX = (vY * wZ);
  gs_scalar nY = -(vX * wZ);

  // Normalize result
  gs_scalar mag = sqrt((nX*nX) + (nY*nY));
  nX /= mag, nY /= mag;

  // Create wall
  d3d_model_primitive_begin(id, pr_trianglestrip);
    d3d_model_vertex_normal_texture(id, x1, y1, z1, nX, nY, 0, 0, 0);
    d3d_model_vertex_normal_texture(id, x2, y2, z1, nX, nY, 0, hrep, 0);
    d3d_model_vertex_normal_texture(id, x1, y1, z2, nX, nY, 0, 0, vrep);
    d3d_model_vertex_normal_texture(id, x2, y2, z2, nX, nY, 0, hrep, vrep);
  d3d_model_primitive_end(id);
}

void d3d_model_block(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed)
{
  //NOTE: This is the fastest way to batch cubes with uninterpolated normals thanks to my model batching, still slower than a triangle strip with interpolated normals
  //however.
  // Negative X
  d3d_model_primitive_begin( id, pr_trianglefan );
  d3d_model_vertex_normal_texture( id, x1,y1,z1, -1,0,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x1,y1,z2, -1,0,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x1,y2,z2, -1,0,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x1,y2,z1, -1,0,0, 1,1 );
  d3d_model_primitive_end(id);

  // Positive X
  d3d_model_primitive_begin( id, pr_trianglefan );
  d3d_model_vertex_normal_texture( id, x2,y1,z1, 1,0,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x2,y2,z1, 1,0,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x2,y2,z2, 1,0,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x2,y1,z2, 1,0,0, 1,0 );
  d3d_model_primitive_end( id );

  // Negative Y
  d3d_model_primitive_begin( id, pr_trianglefan );
  d3d_model_vertex_normal_texture( id, x1,y1,z1, 0,-1,0, 0,1 );
  d3d_model_vertex_normal_texture( id, x2,y1,z1, 0,-1,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x2,y1,z2, 0,-1,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x1,y1,z2, 0,-1,0, 0,0 );
  d3d_model_primitive_end( id );

  // Positive Y
  d3d_model_primitive_begin( id, pr_trianglefan );
  d3d_model_vertex_normal_texture( id, x1,y2,z1, 0,1,0, 1,1 );
  d3d_model_vertex_normal_texture( id, x1,y2,z2, 0,1,0, 1,0 );
  d3d_model_vertex_normal_texture( id, x2,y2,z2, 0,1,0, 0,0 );
  d3d_model_vertex_normal_texture( id, x2,y2,z1, 0,1,0, 0,1 );
  d3d_model_primitive_end( id );

  if (closed) {
    // Negative Z
    d3d_model_primitive_begin( id, pr_trianglefan );
    d3d_model_vertex_normal_texture( id, x1,y1,z1, 0,0,-1, 0,0 );
    d3d_model_vertex_normal_texture( id, x1,y2,z1, 0,0,-1, 0,1 );
    d3d_model_vertex_normal_texture( id, x2,y2,z1, 0,0,-1, 1,1 );
    d3d_model_vertex_normal_texture( id, x2,y1,z1, 0,0,-1, 1,0 );
    d3d_model_primitive_end( id );

    // Positive Z
    d3d_model_primitive_begin( id, pr_trianglefan );
    d3d_model_vertex_normal_texture( id, x1,y1,z2, 0,0,1, 0,0 );
    d3d_model_vertex_normal_texture( id, x2,y1,z2, 0,0,1, 1,0 );
    d3d_model_vertex_normal_texture( id, x2,y2,z2, 0,0,1, 1,1 );
    d3d_model_vertex_normal_texture( id, x1,y2,z2, 0,0,1, 0,1 );
    d3d_model_primitive_end( id );
  }
}

void d3d_model_cylinder(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  float v[100][3];
  float t[100][3];
  steps = min(max(steps, 3), 48); // i think 48 should be circle_presicion
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k;
  a = 0; px = cx+rx; py = cy; tp = 0; k = 0;

  d3d_model_primitive_begin(id, pr_trianglestrip);
  for (int i = 0; i <= steps; i++) {
    v[k][0] = px; v[k][1] = py; v[k][2] = z2;
    t[k][0] = tp; t[k][1] = 0;
    d3d_model_vertex_normal_texture(id, px, py, z2, cos(a), sin(a), 0, tp, 0);
    k++;
    v[k][0] = px; v[k][1] = py; v[k][2] = z1;
    t[k][0] = tp; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, px, py, z1, cos(a), sin(a), 0, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  d3d_model_primitive_end(id);

  if (closed) {
    // BOTTOM
    d3d_model_primitive_begin(id, pr_trianglefan);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z1;
    t[k][0] = 0; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, cx, cy, z1, 0, 0, -1, 0, vrep);
    k++;
    for (int i = steps*2; i >= 0; i-=2) {
      d3d_model_vertex_normal_texture(id, v[i+1][0], v[i+1][1], v[i+1][2], 0, 0, -1, t[i][0], t[i][1]);
    }
    d3d_model_primitive_end(id);

    // TOP
    d3d_model_primitive_begin(id, pr_trianglefan);
    v[k][0] = cx; v[k][1] = cy; v[k][2] = z2;
    t[k][0] = 0; t[k][1] = vrep;
    d3d_model_vertex_normal_texture(id, cx, cy, z2, 0, 0, 1, 0, vrep);
    k++;
    for (int i = 0; i <= steps*2; i+=2) {
      d3d_model_vertex_normal_texture(id, v[i][0], v[i][1], v[i][2], 0, 0, 1, t[i][0], t[i][1]);
    }
    d3d_model_primitive_end(id);
  }
}

void d3d_model_cone(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
  steps = min(max(steps, 3), 48);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
  double a, px, py, tp;
  int k = 0;
  d3d_model_primitive_begin(id, pr_trianglefan);
  d3d_model_vertex_normal_texture(id, cx, cy, z2, 0, 0, 1, 0, 0);
  k++;
  a = 0; px = cx+rx; py = cy; tp = 0;
  for (int i = 0; i <= steps; i++) {
    d3d_model_vertex_normal_texture(id, px, py, z1, cos(a), sin(a), 0, tp, vrep);
    k++; a += pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
  }
  d3d_model_primitive_end(id);
  if (closed) {
    d3d_model_primitive_begin(id, pr_trianglefan);
    d3d_model_vertex_normal_texture(id, cx, cy, z1, 0, 0, -1, 0, vrep);
    k++;
    tp = 0;
    for (int i = 0; i <= steps + 1; i++) {
      d3d_model_vertex_normal_texture(id, px, py, z1, 0, 0, -1, tp, 0);
      k++; a -= pr; px = cx+cos(a)*rx; py = cy+sin(a)*ry; tp += invstep;
    }
    d3d_model_primitive_end(id);
  }
}

void d3d_model_ellipsoid(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{
  float v[277][3];
  float n[277][3];
  float t[277][3];
  steps = min(max(steps, 3), 24);
  const int zsteps = ceil(steps/2);
  const double cx = (x1+x2)/2, cy = (y1+y2)/2, cz = (z1+z2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, rz = (z2-z1)/2, invstep = (1.0/steps)*hrep, invstep2 = (1.0/zsteps)*vrep, pr = 2*M_PI/steps, qr = M_PI/zsteps;
  double a, b, px, py, pz, tp, tzp, cosb;
  double cosx[25], siny[25], txp[25];
  a = pr; tp = 0;
  for (int i = 0; i <= steps; i++) {
    cosx[i] = cos(a)*rx; siny[i] = sin(a)*ry;
    txp[i] = tp;
    a += pr; tp += invstep;
  }
  int k = 0, kk;
  // BOTTOM
  d3d_model_primitive_begin(id, pr_trianglefan);
  v[k][0] = cx; v[k][1] = cy; v[k][2] = cz - rz;
  n[k][0] = 0; n[k][1] = 0; n[k][2] = -1;
  t[k][0] = 0; t[k][1] = vrep;
  d3d_model_vertex_normal_texture(id, cx, cy, cz - rz, 0, 0, -1, 0, vrep);
  k++;
  b = qr-M_PI/2;
  cosb = cos(b);
  pz = rz*sin(b);
  tzp = vrep-invstep2;
  px = cx+rx*cosb; py = cy;
  for (int i = 0; i <= steps; i++) {
    v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
    n[k][0] = cosx[i]; n[k][1] = -siny[i]; n[k][2] = cosb;
    t[k][0] = txp[i]; t[k][1] = tzp;
    d3d_model_vertex_normal_texture(id, px, py, cz + pz, cosx[i], -siny[i], sin(b), txp[i], tzp);
    k++; px = cx+cosx[i]*cosb; py = cy-siny[i]*cosb;
  }
  d3d_model_primitive_end(id);
  // SIDES
  for (int ii = 0; ii < zsteps - 2; ii++) {
    b += qr;
    cosb = cos(b);
    pz = rz*sin(b);
    tzp -= invstep2;
    d3d_model_primitive_begin(id, pr_trianglestrip);
    px = cx+rx*cosb; py = cy;
    for (int i = 0; i <= steps; i++) {
      kk = k - steps - 1;
      d3d_model_vertex_normal_texture(id, v[kk][0], v[kk][1], v[kk][2], n[kk][0], n[kk][1], n[kk][2], t[kk][0], t[kk][1]);
      v[k][0] = px; v[k][1] = py; v[k][2] = cz + pz;
      n[k][0] = cosx[i]; n[k][1] = -siny[i]; n[k][2] = sin(b);
      t[k][0] = txp[i]; t[k][1] = tzp;
      d3d_model_vertex_normal_texture(id, px, py, cz + pz, cosx[i], -siny[i], sin(b), txp[i], tzp);
      k++; px = cx+cosx[i]*cosb; py = cy-siny[i]*cosb;
    }
    d3d_model_primitive_end(id);
  }
  // TOP
  d3d_model_primitive_begin(id, pr_trianglefan);
  v[k][0] = cx; v[k][1] = cy; v[k][2] = cz + rz;
  n[k][0] = 0; n[k][1] = 0; n[k][2] = 1;
  t[k][0] = 0; t[k][1] = 0;
  d3d_model_vertex_normal_texture(id, cx, cy, cz + rz, 0, 0, 1, 0, 0);
  k++;
  for (int i = k - 2; i >= k - steps - 2; i--) {
    d3d_model_vertex_normal_texture(id, v[i][0], v[i][1], v[i][2], n[i][0], n[i][1], n[i][2], t[i][0], t[i][1]);
  }
  d3d_model_primitive_end(id);
}

void d3d_model_icosahedron(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, gs_scalar hrep, gs_scalar vrep, int steps)
{
  gs_scalar width = (x2-x1), length = (y2-y1), height = (z2-z1);
  static gs_scalar vdata[12][3] = {
    {0, 0.5, 1}, {1, 0.5, 1}, {0, 0.5, 0}, {1, 0.5, 0},
    {0.5, 1, 1}, {0.5, 1, 0}, {0.5, 0, 1}, {0.5, 0, 0},
    {1, 1, 0.5}, {0, 1, 0.5}, {1, 0, 0.5}, {0, 0, 0.5}
  };

  static int tindices[20][3] = {
    {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
    {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
    {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
    {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11}
  };

  d3d_model_primitive_begin(id, pr_trianglelist);
  for (unsigned i = 0; i < 20; i++) {
    d3d_model_vertex(id, x1 + vdata[tindices[i][0]][0] * width, y1 + vdata[tindices[i][0]][1] * length, z1 + vdata[tindices[i][0]][2] * height);
    d3d_model_vertex(id, x1 + vdata[tindices[i][1]][0] * width, y1 + vdata[tindices[i][1]][1] * length, z1 + vdata[tindices[i][1]][2] * height);
    d3d_model_vertex(id, x1 + vdata[tindices[i][2]][0] * width, y1 + vdata[tindices[i][2]][1] * length, z1 + vdata[tindices[i][2]][2] * height);
    // TODO: Add normals, uv's, and subdivide by the number of steps
  }
  d3d_model_primitive_end(id);
}

void d3d_model_torus(int id, gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar hrep, gs_scalar vrep, int csteps, int tsteps, double radius, double tradius)
{
  double TWOPI = 2 * (double)M_PI;
  for (int i = 0; i < csteps; i++) {
    d3d_model_primitive_begin(id, pr_trianglestrip);
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

        d3d_model_vertex_normal_texture(id, x1 + x, y1 + y, z1 + z, nX, nY, nZ, u, v);
      }
    }
    d3d_model_primitive_end(id);
  }
}

}
