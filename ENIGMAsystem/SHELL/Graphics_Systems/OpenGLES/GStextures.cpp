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

namespace enigma{extern unsigned cur_bou_tha_noo_sho_eve_cha_eve;}

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
  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {
    if (texture != enigma::cur_bou_tha_noo_sho_eve_cha_eve)
      glBindTexture(GL_TEXTURE_2D, texture);
    
    int r,g,b,a;
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_RED_SIZE,  &r);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_GREEN_SIZE,&g);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_BLUE_SIZE, &b);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_ALPHA_SIZE,&a);
    
    // Set r to the largest of the four
    r = r > g ? r : g, r = r > b ? r : b, r = r > a ? r : a;
    
    unsigned char* ret = new unsigned char[r << 2];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret);
    
    if (texture != enigma::cur_bou_tha_noo_sho_eve_cha_eve)
      glBindTexture(GL_TEXTURE_2D, enigma::cur_bou_tha_noo_sho_eve_cha_eve);
    
    return ret;
  }
}
