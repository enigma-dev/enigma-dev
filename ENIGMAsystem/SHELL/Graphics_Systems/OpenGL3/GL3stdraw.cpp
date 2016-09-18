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

#include <stdio.h>
#include <math.h>

#include "../General/OpenGLHeaders.h"
#include "../General/GSstdraw.h"
#include "../General/GStextures.h"
#include "Universal_System/roomsystem.h"
#include "Bridges/General/GL3Context.h"
#include "GLSLshader.h"
#include "GL3shader.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma {
  extern unsigned char currentcolor[4];
  extern unsigned bound_shader;
  extern vector<enigma::ShaderProgram*> shaderprograms;
}

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
  (enable?glEnable:glDisable)(GL_MULTISAMPLE);
}

void draw_enable_alphablend(bool enable) {
	(enable?glEnable:glDisable)(GL_BLEND);
}

bool draw_get_alpha_test() {
  return false;//glIsEnabled(GL_ALPHA_TEST);
}

unsigned draw_get_alpha_test_ref_value()
{
  //float ref;
  //glGetFloatv(GL_ALPHA_TEST_REF, &ref);
  return 0;//ref*256;
}

void draw_set_alpha_test(bool enable)
{
    enigma_user::glsl_uniformi(enigma::shaderprograms[enigma::bound_shader]->uni_alphaTestEnable, enable);
}

void draw_set_alpha_test_ref_value(unsigned val)
{
    enigma_user::glsl_uniformf(enigma::shaderprograms[enigma::bound_shader]->uni_alphaTest, (gs_scalar)val/256.0);
}

void draw_set_line_pattern(int pattern, int scale)
{
  if (pattern == -1)
      glDisable(GL_LINE_STIPPLE);
  else
    glEnable(GL_LINE_STIPPLE),
    glLineStipple(scale,(short)pattern);
}

}

//#include <endian.h>
//TODO: Though serprex, the author of the function below, never included endian.h,
//   // Doing so is necessary for the function to work at its peak.
//   // When ENIGMA generates configuration files, one should be included here.

namespace enigma_user
{

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
  oglmgr->ReadPixels();
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
  oglmgr->ReadPixels();
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

}

namespace enigma
{


bool fill_complex_polygon(const std::list<PolyVertex>& vertices, int defaultColor, bool allowHoles)
{
  //TODO: Complex polygon supported only in OpenGL1 at the moment. By returning false here, we fall back
  //      on a convex-only polygon drawing routine that works on any platform.
  return false;
}


}

