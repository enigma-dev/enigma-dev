/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include "OpenGLHeaders.h"
#include <stdio.h>

namespace enigma{extern unsigned bound_texture;}

namespace enigma
{
  unsigned graphics_create_texture(int fullwidth, int fullheight, void* pxdata)
  {
    GLuint texture;
    printf("Texture %d x %d\n",fullwidth,fullheight);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fullwidth, fullheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxdata); //OPENGLES changed 4 to GL_RGBA
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //GL_NEAREST to GL_LINEAR
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //GL_NEAREST to GL_LINEAR
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP); OPENGLES
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP); OPENGLES
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
  }
    
    //Retrieve image data from a texture, in unsigned char, RGBA format.
    //OpenGLES doesn't have support for glGetTexImage!!!
    unsigned char* graphics_get_texture_rgba(unsigned texture)
    {
        if (texture != enigma::bound_texture)
            glBindTexture(GL_TEXTURE_2D, texture);
        
        int w,h;
        //glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
        //glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);
        
        unsigned char* ret = new unsigned char[(w*h) << 2];
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret);
        
        if (texture != enigma::bound_texture)
            glBindTexture(GL_TEXTURE_2D, enigma::bound_texture);
        
        return ret;
  }
    
}
