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

#include "OpenGLHeaders.h"
#include "GLprmtvs.h"
#include "binding.h"
#include "GLtextures.h"

#include <string>
#include "Widget_Systems/widgets_mandatory.h"

#if PRIMBUFFER
GLenum __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

#define __GETR(x) ((x & 0x0000FF))/255.0
#define __GETG(x) ((x & 0x00FF00)>>8)/255.0
#define __GETB(x) ((x & 0xFF0000)>>16)/255.0

GLenum ptypes_by_id[16] = {
  GL_POINTS, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
  GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINE_LOOP, GL_QUADS,
  GL_QUAD_STRIP, GL_POLYGON,

  //These are padding.
  GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS
};

namespace enigma {
  extern unsigned char currentcolor[4];
}

void draw_set_primitive_aa(bool enable, int quality)
{
    if (enable==1) {
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, quality);
        glHint(GL_POINT_SMOOTH_HINT, quality);
        glHint(GL_POLYGON_SMOOTH_HINT, quality);
        glPointSize(1.1);
        glLineWidth(1.1);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_POINT_SMOOTH);
        glPointSize(1);
        glLineWidth(1);
    }
}

int draw_primitive_begin(int dink)
{
	untexture();
	GLenum kind = ptypes_by_id[ dink & 15 ];
  glBegin(kind);
  return 0;
}

int draw_primitive_begin_texture(int dink,unsigned tex)
{
  bind_texture(tex);
	GLenum kind = ptypes_by_id[ dink & 15 ];
	glBegin(kind);
  return 0;
}

int draw_vertex(double x, double y)
{
	glVertex2f(x,y);
	return 0;
}

int draw_vertex_color(float x, float y, int col, float alpha)
{
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(
      (col&0xFF)/255.0,
      ((col&0xFF00)>>8)/255.0,
      ((col&0xFF0000)>>16)/255.0,
      alpha);
    glVertex2f(x,y);
  glPopAttrib();
	return 0;
}
int draw_vertex_texture(float x, float y, float tx, float ty)
{
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
	return 0;
}
int draw_vertex_texture_color(float x, float y, float tx, float ty, int col, float alpha)
{
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(
      (col&0xFF)/255.0,
      ((col&0xFF00)>>8)/255.0,
      ((col&0xFF0000)>>16)/255.0,
      alpha);
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
  glPopAttrib();
	return 0;
}
int draw_primitive_end()
{
	glEnd();
	return 0;
}

void d3d_primitive_begin(int kind)
{
    untexture();
    glBegin(ptypes_by_id[kind]);
}
void d3d_primitive_begin_texture(int kind, int texId)
{
    bind_texture(get_texture(texId));
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
