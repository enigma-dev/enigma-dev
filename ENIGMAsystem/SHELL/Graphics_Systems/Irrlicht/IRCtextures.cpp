/** Copyright (C) 2013 Robert B. Colton
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
#include <string.h>
#include "../General/GStextures.h"
#include "IRCTextureStruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/graphics_mandatory.h"


vector<TextureStruct*> textureStructs(0);

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
}

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
  int graphics_create_texture(int fullwidth, int fullheight, void* pxdata, bool isfont)
  {

  }

  int graphics_duplicate_texture(int tex)
  {

  }

  void graphics_replace_texture_alpha_from_texture(int tex, int copy_tex)
  {

  }

  void graphics_delete_texture(int tex)
  {
	 textureStructs.erase(textureStructs.begin() + tex);
  }

  unsigned char* graphics_get_texture_rgba(unsigned texture)
  {

  }
}