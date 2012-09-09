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

#include <stdio.h>
#include "OpenGLHeaders.h"
#include <string>
using std::string;
#include "GStextures.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "binding.h"

inline unsigned int lgpp2(unsigned int x){//Trailing zero count. lg for perfect powers of two
	x =  (x & -x) - 1;
	x -= ((x >> 1) & 0x55555555);
	x =  ((x >> 2) & 0x33333333) + (x & 0x33333333);
	x =  ((x >> 4) + x) & 0x0f0f0f0f;
	x += x >> 8;
	return (x + (x >> 16)) & 63;
}

namespace enigma
{
  bool interpolate_textures = false; //NOTE: set value here when game settings are used

  unsigned graphics_create_texture(int fullwidth, int fullheight, void* pxdata)
  {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, fullwidth, fullheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata);
    if (interpolate_textures)
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
  }

  unsigned graphics_duplicate_texture(int tex)
  {
    GLuint texture = tex;
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texture);
    int w, h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    char* bitmap = new char[(h<<(lgpp2(w)+2))|2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    unsigned dup_tex = graphics_create_texture(w, h, bitmap);
    delete[] bitmap;
    glPopAttrib();
    return dup_tex;
  }

  unsigned graphics_create_texture_alpha_from_texture(int tex, int copy_tex)
  {
    GLuint texture = tex, copy_texture = copy_tex;
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
    glColor4f(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);

    int w, h, size;
    glBindTexture(GL_TEXTURE_2D, texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT, &h);
    size = (h<<(lgpp2(w)+2))|2;
    char* bitmap = new char[size];
    char* bitmap2 = new char[size];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
    glBindTexture(GL_TEXTURE_2D, copy_texture);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap2);

    for (int i = 3; i < size; i += 4)
        bitmap[i] = (bitmap2[i-1] + bitmap2[i-2] + bitmap2[i-3])/3;

    unsigned dup_tex = graphics_create_texture(w, h, bitmap);
    delete[] bitmap;
    delete[] bitmap2;
    glPopAttrib();
    return dup_tex;
  }

  void graphics_delete_texture(int tex)
  {
    GLuint texture = tex;
    glDeleteTextures(1, &texture);
  }

  //Retrieve image data from a texture, in unsigned char, RGBA format.
  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {
    bind_texture(texture);

    int w,h;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

    unsigned char* ret = new unsigned char[(w*h) << 2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret);

    return ret;
  }
}
