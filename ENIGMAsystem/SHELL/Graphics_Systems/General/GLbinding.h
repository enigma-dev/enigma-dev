/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton, Serpex
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

#include "../General/GLTextureStruct.h"

#ifndef __GL_BINDING_H
#define __GL_BINDING_H

unsigned get_texture(int texid);

// OpenGL does not memorize the bound texture and significant slowdowns occur by constantly rebinding a texture
// glBind calls clog up the pipeline avoid them at all costs, texture paging is good
#define use_bound_texture_global
#ifdef use_bound_texture_global
  namespace enigma { extern unsigned bound_texture; }
  #define texture_reset() if(enigma::bound_texture) glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);
  #define texture_use(texid) if (enigma::bound_texture != unsigned(texid)) \
    glBindTexture(GL_TEXTURE_2D,enigma::bound_texture = texid)
#else
  #define texture_reset() glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0)
  #define texture_use(texid) glBindTexture(GL_TEXTURE_2D, texid)
#endif

#endif
