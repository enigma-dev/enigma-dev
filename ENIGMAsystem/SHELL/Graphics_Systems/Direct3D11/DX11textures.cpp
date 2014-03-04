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
#include "Direct3D11Headers.h"
#include "Bridges/General/DX11Context.h"
#include <string.h>
using std::string;
#include "../General/GStextures.h"
#include "DX11TextureStruct.h"
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
  int graphics_create_texture(unsigned width, unsigned height, unsigned fullwidth, unsigned fullheight, void* pxdata, bool isfont)
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

  }

  unsigned char* graphics_get_texture_rgba(unsigned texture, unsigned* fillwidth, unsigned* fullheight)
  {

  }
}

namespace enigma_user
{

void texture_set_enabled(bool enable)
{

}

void texture_set_interpolation(int enable)
{

}

bool texture_get_interpolation()
{
    return enigma::interpolate_textures;
}

void texture_set_blending(bool enable)
{

}

gs_scalar texture_get_width(int texid)
{

}

gs_scalar texture_get_height(int texid)
{

}

unsigned texture_get_texel_width(int texid)
{

}

unsigned texture_get_texel_height(int texid)
{

}

void texture_set(int texid) {

}

void texture_set_stage(int stage, int texid) {

}

void texture_reset() {

}

void texture_set_repeat(bool repeat)
{

}

void texture_set_repeat(int texid, bool repeat)
{

}

void texture_set_wrap(int texid, bool wrapr, bool wraps, bool wrapt)
{

}

void texture_preload(int texid)
{

}//functionality has been removed in ENIGMA, all textures are automatically preloaded

void texture_set_priority(int texid, double prio)
{

}

void texture_set_border(int texid, int r, int g, int b, double a)
{

}

void texture_set_swizzle(int texid, int r, int g, int b, double a)
{

}

void texture_set_levelofdetail(int texid, double minlod, double maxlod, int maxlevel)
{

}

void texture_mipmapping_filter(int texid, int filter)
{

}

void texture_mipmapping_generate(int texid, int levels)
{

}

bool  texture_anisotropy_supported()
{

}

float texture_anisotropy_maxlevel()
{

}

void  texture_anisotropy_filter(int texid, gs_scalar levels)
{

}

bool  texture_multitexture_supported()
{

}

void texture_multitexture_enable(bool enable)
{

}

}

