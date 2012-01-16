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
