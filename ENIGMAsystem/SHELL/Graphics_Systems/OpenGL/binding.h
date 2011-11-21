/** Copyright (C) 2008-2011 Josh Ventura, Serprex
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

#ifdef use_bound_texture_global
  namespace enigma { extern unsigned bound_texture; }
  #define untexture() if(enigma::bound_texture) glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);
  #define bind_texture(texid) if (enigma::bound_texture != unsigned(texid)) \
    glBindTexture(GL_TEXTURE_2D,enigma::bound_texture = texid)
#else
  #define untexture() glBindTexture(GL_TEXTURE_2D, 0)
  #define bind_texture(texid) glBindTexture(GL_TEXTURE_2D, texid)
#endif
