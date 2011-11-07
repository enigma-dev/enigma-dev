/** Copyright (C) 2008-2011 Josh Ventura
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
#include "GSprmtvs.h"

#include <string>
#include "../../Widget_Systems/widgets_mandatory.h"

#if PRIMBUFFER
GLenum __primitivetype[PRIMDEPTH2];
int __primitivelength[PRIMDEPTH2];
float __primitivecolor[PRIMBUFFER][PRIMDEPTH2][4];
float __primitivexy[PRIMBUFFER][PRIMDEPTH2][2];
int __currentpcount[PRIMDEPTH2];
int __currentpdepth;
#endif

//namespace enigma{extern void untexture();}
namespace enigma{extern unsigned bound_texture;}
#define untexture() if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture = 0);

GLenum ptypes_by_id[16] = {
  GL_POINTS, GL_POINTS, GL_LINES, GL_LINE_STRIP, GL_TRIANGLES,
  GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_LINE_LOOP, GL_QUADS,
  GL_QUAD_STRIP, GL_POLYGON,

  //These are padding.
  GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS, GL_POINTS
};

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
  if (enigma::bound_texture != tex)
    glBindTexture(GL_TEXTURE_2D, enigma::bound_texture = tex);
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
  glPushAttrib(GL_CURRENT_BIT);
    glColor4f(1,1,1,1);
    glTexCoord2f(tx,ty);
    glVertex2f(x,y);
  glPopAttrib();
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
