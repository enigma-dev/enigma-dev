/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Serpex
*** Copyright (C) 2014 Seth N. Hetu
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
#include "Graphics_Systems/General/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/roomsystem.h"

#include <cstdlib>
#include <math.h>
#include <stdio.h>

//Proper calling convention is needed on Windows to prevent random crashing.
//However, we assume that if CALLBACK is already defined then it is defined correctly.
#ifndef CALLBACK
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif

namespace enigma_user
{

int draw_get_msaa_maxlevel()
{
  int maximumMSAA;
  glGetIntegerv(GL_MAX_SAMPLES, &maximumMSAA);
  return maximumMSAA;
}

bool draw_get_msaa_supported()
{
    return GLEW_EXT_multisample;
}

void draw_set_msaa_enabled(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
  (enable?glEnable:glDisable)(GL_MULTISAMPLE);
}

void draw_enable_alphablend(bool enable) {
  draw_batch_flush(batch_flush_deferred);
	(enable?glEnable:glDisable)(GL_BLEND);
}

bool draw_get_alpha_test() {
  return glIsEnabled(GL_ALPHA_TEST);
}

unsigned draw_get_alpha_test_ref_value()
{
  float ref;
  glGetFloatv(GL_ALPHA_TEST_REF, &ref);
  return ref*256;
}

void draw_set_alpha_test(bool enable)
{
  draw_batch_flush(batch_flush_deferred);
	(enable?glEnable:glDisable)(GL_ALPHA_TEST);
}

void draw_set_alpha_test_ref_value(unsigned val)
{
  draw_batch_flush(batch_flush_deferred);
	glAlphaFunc(GL_GREATER, val/256);
}

void draw_set_line_pattern(int pattern, int scale)
{
  draw_batch_flush(batch_flush_deferred);
  if (pattern == -1)
    glDisable(GL_LINE_STIPPLE);
  else
    glEnable(GL_LINE_STIPPLE),
    glLineStipple(scale,(short)pattern);
}

} // namespace enigma

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user {

int draw_getpixel(int x,int y)
{
    if (view_enabled)
    {
        x = x - enigma_user::view_xview[enigma_user::view_current];
        y = enigma_user::view_hview[enigma_user::view_current] - (y - enigma_user::view_yview[enigma_user::view_current]) - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::view_wview[enigma_user::view_current] || y > enigma_user::view_hview[enigma_user::view_current]) return 0;
    }
    else
    {
        y = enigma_user::room_height - y - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::room_width || y > enigma_user::room_height) return 0;
    }

    draw_batch_flush(batch_flush_deferred);

  #if defined __BIG_ENDIAN__ || defined __BIG_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&ret);
    return ret;
  #elif defined __LITTLE_ENDIAN__ || defined __LITTLE_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_BGR,GL_UNSIGNED_BYTE,&ret);
    return ret>>8;
  #else
    unsigned char rgb[3];
    glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,&rgb);
    return rgb[0] | rgb[1] << 8 | rgb[2] << 16;
  #endif
}

int draw_getpixel_ext(int x,int y)
{
    if (view_enabled)
    {
        x = x - enigma_user::view_xview[enigma_user::view_current];
        y = enigma_user::view_hview[enigma_user::view_current] - (y - enigma_user::view_yview[enigma_user::view_current]) - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::view_wview[enigma_user::view_current] || y > enigma_user::view_hview[enigma_user::view_current]) return 0;
    }
    else
    {
        y = enigma_user::room_height - y - 1;
        if (x < 0) x = 0;
        if (y < 0) y = 0;
        if (x > enigma_user::room_width || y > enigma_user::room_height) return 0;
    }

    draw_batch_flush(batch_flush_deferred);

  #if defined __BIG_ENDIAN__ || defined __BIG_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&ret);
    return ret;
  #elif defined __LITTLE_ENDIAN__ || defined __LITTLE_ENDIAN
    int ret;
    glReadPixels(x,y,1,1,GL_BGRA,GL_UNSIGNED_BYTE,&ret);
    return ret>>8;
  #else
    unsigned char rgba[4];
    glReadPixels(x,y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,&rgba);
    return rgba[0] | rgba[1] << 8 | rgba[2] << 16 | rgba[3] << 24;
  #endif
}

} // namespace enigma_user

namespace {

std::list<GLdouble*> extra_vertices;

GLdouble* make_vertex(unsigned int numComp)
{
  GLdouble* vertex = (GLdouble*) malloc(numComp * sizeof(GLdouble));
  if (vertex) { extra_vertices.push_back(vertex); }
  return vertex;
}

void clear_free_extra_vertex_list()
{
  for (std::list<GLdouble*>::iterator it=extra_vertices.begin(); it!=extra_vertices.end(); it++) {
    free(*it);
  }
  extra_vertices.clear();
}

void CALLBACK combineCallback(GLdouble coords[3], GLdouble* vertex_data[4], GLfloat weight[4], GLdouble **dataOut)
{
  GLdouble* vertex = make_vertex(6);
  if (vertex) {
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];

    for (int i = 3; i < 6; i++) {
      vertex[i] = (weight[0] ? weight[0] * vertex_data[0][i] : 0)
                + (weight[1] ? weight[1] * vertex_data[1][i] : 0)
                + (weight[2] ? weight[2] * vertex_data[2][i] : 0)
                + (weight[3] ? weight[3] * vertex_data[3][i] : 0);
    }
  }

  //We must write back to *dataOut. If NULL, the error GLU_TESS_NEED_COMBINE_CALLBACK will occur,
  //  but we can't avoid that (not enough memory if vertex fails to be allocated).
  *dataOut = vertex;
}

void CALLBACK vertexCallback(GLvoid *vertex)
{
  GLdouble* ptr = (GLdouble*)vertex;
  glColor3dv(ptr + 3);
  glVertex2dv(ptr);
}

} // anonymous namespace

namespace enigma {

bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles)
{
  enigma_user::draw_batch_flush(enigma_user::batch_flush_deferred);
  //Supposedly this is required; see notes in GLPrimities.cpp
  enigma_user::texture_reset();

  //Create a GLU tessellation object.
  GLUtesselator* tessObj = gluNewTess();
  if (!tessObj) { return false; }
  //Assign callback functions for vertex drawing and combining.
  gluTessCallback(tessObj, GLU_TESS_BEGIN,   (GLvoid (CALLBACK*)()) &glBegin);
  gluTessCallback(tessObj, GLU_TESS_END,     (GLvoid (CALLBACK*)()) &glEnd);
  gluTessCallback(tessObj, GLU_TESS_VERTEX,  (GLvoid (CALLBACK*)()) &vertexCallback);
  gluTessCallback(tessObj, GLU_TESS_COMBINE, (GLvoid (CALLBACK*)()) &combineCallback);

  //Set the winding rule for overlapping edges.
  gluTessProperty(tessObj, GLU_TESS_WINDING_RULE,  (allowHoles?GLU_TESS_WINDING_ODD:GLU_TESS_WINDING_NONZERO));

  //Start drawing our polygon, which comprises a single contour.
  gluTessBeginPolygon(tessObj, NULL);
  gluTessBeginContour(tessObj);
  for (std::list<PolyVertex>::const_iterator it=vertices.begin(); it!=vertices.end(); it++) {
    //Account for the default color.
    defaultColor = (it->color!=-1 ? it->color : defaultColor);

    //Draw it.
    GLdouble* vertex = make_vertex(6);
    if (vertex) {
      vertex[0] = it->x;
      vertex[1] = it->y;
      vertex[2] = 0.0; //Not used
      vertex[3] = (defaultColor&0xFF) / 255.0;
      vertex[4] = ((defaultColor&0xFF00)>>8) / 255.0;
      vertex[5] = ((defaultColor&0xFF0000)>>16) / 255.0;
      //NOTE: We can put a per-vertex alpha component here if it's desired.
      gluTessVertex(tessObj, vertex, vertex);
    }
  }
  gluTessEndContour(tessObj);
  gluTessEndPolygon(tessObj);

  //Done, reclaim memory.
  gluDeleteTess(tessObj);
  clear_free_extra_vertex_list();
  return true;
}

} // namespace enigma
