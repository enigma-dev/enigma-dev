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
#include "../General/GStextures.h"
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"
#include <math.h>
#include "../General/GLbinding.h"

using namespace std;

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

#include <floatcomp.h>

namespace enigma {
  bool d3dMode = false;
  bool d3dHidden = false;
  bool d3dZWriteEnable = true;
  bool d3dCulling = 0;
}

double projection_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}, transformation_matrix[16] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

GLenum renderstates[6] = {
  GL_FRONT, GL_BACK, GL_FRONT_AND_BACK, 
  GL_NICEST, GL_FASTEST, GL_DONT_CARE
};

GLenum fogmodes[3] = {
  GL_EXP, GL_EXP2, GL_LINEAR
};

GLenum depthoperators[8] = {
  GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER, GL_NOTEQUAL,
  GL_GEQUAL, GL_ALWAYS
};

GLenum fillmodes[3] = {
  GL_POINT, GL_LINE, GL_FILL
};

GLenum cullingstates[3] = {
  0, GL_CW, GL_CCW
};

namespace enigma_user
{

void d3d_depth_clear() {
  d3d_depth_clear_value(1.0f);
}

void d3d_depth_clear_value(float value) {
  glClearDepthf(value);
}

void d3d_start()
{
  // Set global ambient lighting to nothing.
  float global_ambient[] = { 0.0f, 0.0f, 0.0f, 0.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

  // Enable depth buffering
  enigma::d3dMode = true;
  enigma::d3dHidden = true;
  enigma::d3dZWriteEnable = true;
  enigma::d3dCulling = rs_none;
  glDepthMask(true);
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
  enigma::d3dMode = false;
  enigma::d3dHidden = false;
  enigma::d3dZWriteEnable = false;
  enigma::d3dCulling = rs_none;
  glDepthMask(false);
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
    return enigma::d3dMode;
}

// disabling hidden surface removal in means there is no depth buffer
void d3d_set_hidden(bool enable)
{
    (enable?glEnable:glDisable)(GL_DEPTH_TEST);
    enigma::d3dHidden = enable;
	d3d_set_zwriteenable(enable);
}

// disabling zwriting can let you turn off testing for a single model, for instance
// to fix cutout billboards such as trees the alpha pixels on their edges may not depth sort
// properly particle effects are usually drawn with zwriting disabled because of this as well
void d3d_set_zwriteenable(bool enable)
{
	glDepthMask(enable);
	enigma::d3dZWriteEnable = enable;
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
  glFogi(GL_FOG_MODE, fogmodes[mode]);
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

void d3d_set_culling(int mode)
{
  enigma::d3dCulling = mode;
  ((mode > 0)?glEnable:glDisable)(GL_CULL_FACE);
  glFrontFace(cullingstates[mode]);
}

int d3d_get_culling() {
	return enigma::d3dCulling;
}

void d3d_set_fill_mode(int fill)
{
  glPolygonMode(GL_FRONT_AND_BACK, fillmodes[fill]);
}

void d3d_set_line_width(float value) {
  glLineWidth(value);
}

void d3d_set_point_size(float value) {
  glPointSize(value);
} 

void d3d_set_depth_operator(int mode) {
  glDepthFunc(depthoperators[mode]);
}

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

void d3d_set_depth(double dep)
{

}//TODO: Write function

void d3d_set_shading(bool smooth)
{
    glShadeModel(smooth?GL_SMOOTH:GL_FLAT);
}

}

namespace enigma_user
{

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
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(angle, 1, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glScalef(1, -1, 1);
  glOrtho(x-0.5,x + width,y-0.5,y + height,0,1);
  glRotatef(angle,0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
}

void d3d_set_projection_perspective(gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, double angle)
{
  glDisable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, -view_wview[view_current] / (double)view_hview[view_current], 1, 32000);
  glMatrixMode(GL_MODELVIEW);
  glScalef(1, -1, 1);
  glOrtho(x,x + width,y,y + height,0,1);
  glRotatef(angle,0,0,1);
  glGetDoublev(GL_MODELVIEW_MATRIX,projection_matrix);
  glMultMatrixd(transformation_matrix);
  enigma::d3d_light_update_positions();
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

  texture_use(get_texture(texId));

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

  texture_use(get_texture(texId));

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

void d3d_draw_block(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed)
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

void d3d_draw_cylinder(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar x2, gs_scalar y2, gs_scalar z2, int texId, gs_scalar hrep, gs_scalar vrep, bool closed, int steps)
{
    float v[100][3];
	float n[100][3];
    float t[100][3];
    steps = min(max(steps, 3), 48);
    const double cx = (x1+x2)/2, cy = (y1+y2)/2, rx = (x2-x1)/2, ry = (y2-y1)/2, invstep = (1.0/steps)*hrep, pr = 2*M_PI/steps;
    double a, px, py, tp;
    int k;
  texture_use(get_texture(texId));
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
    texture_use(get_texture(texId));
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
    texture_use(get_texture(texId));
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
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

  texture_use(get_texture(texId));
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
	texture_use(get_texture(texId));
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

void d3d_transform_add_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_add_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
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
void d3d_transform_add_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
{
  glLoadIdentity();
  glRotatef(-angle,x,y,z);
  glMultMatrixd(transformation_matrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}

void d3d_transform_set_translation(gs_scalar xt, gs_scalar yt, gs_scalar zt)
{
  glLoadIdentity();
  glTranslatef(xt, yt, zt);
  glGetDoublev(GL_MODELVIEW_MATRIX,transformation_matrix);
  glLoadMatrixd(projection_matrix);
  glMultMatrixd(transformation_matrix);
}
void d3d_transform_set_scaling(gs_scalar xs, gs_scalar ys, gs_scalar zs)
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
void d3d_transform_set_rotation_axis(gs_scalar x, gs_scalar y, gs_scalar z, double angle)
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
    gs_scalar x;
    gs_scalar y;
    gs_scalar z;
    gs_scalar w;
    posi(gs_scalar x1, gs_scalar y1, gs_scalar z1, gs_scalar w1) : x(x1), y(y1), z(z1), w(w1){}
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

    bool light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
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

    bool light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
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
        const float attenuation_calibration = 8.0;
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

bool d3d_light_define_direction(int id, gs_scalar dx, gs_scalar dy, gs_scalar dz, int col)
{
    return d3d_lighting.light_define_direction(id, dx, dy, dz, col);
}

bool d3d_light_define_point(int id, gs_scalar x, gs_scalar y, gs_scalar z, double range, int col)
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
