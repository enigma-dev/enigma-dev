/** Copyright (C) 2013 Harijs Grinbergs
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

#ifndef GL3BINDING_H

#include <vector>
using std::vector;

//GL3 uses VBO's, so when the texture is switched it not only binds it, but also renders the batched VBO
  namespace enigma {
   extern vector<float> globalVBO_data;
   extern unsigned bound_texture;
  }
  void draw_globalVBO();
  
  //The (now removed) enigma::globalVBO_data.size()>enigma::globalVBO_data.max_size()-100 is used to check if we don't fill it over top and segfault
  //This only happens when drawing over 5000000 (5 million) sprites at once. When texture atlas is implemented there is a greater chance of this happening

	
#define use_bound_texture_global
#ifdef use_bound_texture_global
  #define texture_reset() if(enigma::bound_texture) draw_globalVBO(), glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D,enigma::bound_texture=0);
  #define texture_use(texid) if (enigma::bound_texture != unsigned(texid)) \
    draw_globalVBO(), glBindTexture(GL_TEXTURE_2D,enigma::bound_texture = texid)
 #else
  #define texture_reset() draw_globalVBO(), glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0)
  #define texture_use(texid) draw_globalVBO(), glBindTexture(GL_TEXTURE_2D, texid)
#endif

#endif
