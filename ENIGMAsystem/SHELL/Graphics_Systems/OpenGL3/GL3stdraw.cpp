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

#include "GLSLshader.h"
#include "GL3shader.h"

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/General/GSstdraw.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include "Graphics_Systems/General/GStextures.h"
#include "Graphics_Systems/General/GScolor_macros.h"

#include "Universal_System/roomsystem.h"

#include <stdio.h>
#include <math.h>

namespace enigma {

extern unsigned bound_shader;
extern vector<enigma::ShaderProgram*> shaderprograms;

} // namespace enigma

namespace enigma_user {

void draw_clear_alpha(int col,float alpha)
{
	draw_batch_flush(batch_flush_deferred);
  //Unfortunately, we lack a 255-based method for setting ClearColor.
	glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}
void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

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

} // namespace enigma_user

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
