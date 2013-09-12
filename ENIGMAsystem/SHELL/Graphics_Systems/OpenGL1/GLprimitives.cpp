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
#include "../General/GLbinding.h"
#include "../General/GStextures.h"
#include "../General/GSprimitives.h"

#include <string>
#include "Widget_Systems/widgets_mandatory.h"

#define GETR(x) ((x & 0x0000FF))/255.0
#define GETG(x) ((x & 0x00FF00)>>8)/255.0
#define GETB(x) ((x & 0xFF0000)>>16)/255.0

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

int draw_primitive_begin(int dink)
{
  GLenum kind = ptypes_by_id[ dink & 15 ];
  glBegin(kind);
  return 0;
}

int draw_primitive_begin_texture(int dink,unsigned tex)
{
  texture_use(tex);
  GLenum kind = ptypes_by_id[ dink & 15 ];
  glBegin(kind);
  return 0;
}

int draw_vertex(gs_scalar x, gs_scalar y)
{
	glVertex2f(x,y);
	return 0;
}

int draw_vertex_color(gs_scalar x, gs_scalar y, int col, float alpha)
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
int draw_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty)
{
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
	return 0;
}
int draw_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar tx, gs_scalar ty, int col, float alpha)
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
    glBegin(ptypes_by_id[kind]);
}
void d3d_primitive_begin_texture(int kind, int texId)
{
    texture_use(get_texture(texId));
    glBegin(ptypes_by_id[kind]);
}

void d3d_primitive_end()
{
    glEnd();
}

void d3d_vertex(gs_scalar x, gs_scalar y, gs_scalar z)
{
    glVertex3d(x,y,z);
}
void d3d_vertex_color(gs_scalar x, gs_scalar y, gs_scalar z, int color, double alpha)
{
    glColor4f(GETR(color), GETG(color), GETB(color), alpha);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}
void d3d_vertex_texture(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty)
{
    glTexCoord2f(tx,ty);
    glVertex3d(x,y,z);
}
void d3d_vertex_texture_color(gs_scalar x, gs_scalar y, gs_scalar z, gs_scalar tx, gs_scalar ty, int color, double alpha)
{
    glColor4f(GETR(color), GETG(color), GETB(color), alpha);
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
    glColor4f(GETR(color), GETG(color), GETB(color), alpha);
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
    glColor4f(GETR(color), GETG(color), GETB(color), alpha);
    glTexCoord2f(tx,ty);
    glNormal3f(nx, ny, nz);
    glVertex3d(x,y,z);
    glColor4ubv(enigma::currentcolor);
}

}

