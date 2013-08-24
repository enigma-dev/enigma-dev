/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */

#include "Universal_System/scalar.h"

#ifndef _GLTEXTURES__H
#define _GLTEXTURES__H

namespace enigma
{
    extern bool interpolate_textures;
}

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

namespace enigma_user {
enum {
  tx_none,
  tx_nearest,
  tx_bilinear,
  tx_trilinear
};
}

namespace enigma_user
{

void texture_set_enabled(bool enable);
bool texture_get_interpolation();
int texture_get_texel_width(int texid);
int texture_get_texel_height(int texid);
void texture_set_stage(int stage, int texid);
void texture_set_blending(bool enable);
void texture_set_repeat(bool repeat);
void texture_set_repeat(int texid, bool repeat);
void texture_set_wrap(int texid, bool wrapr, bool wraps, bool wrapt);
void texture_preload(int texid);
void texture_set_priority(int texid, double prio);
void texture_set_border(int texid, int r, int g, int b, double a);
void texture_set_swizzle(int texid, int r, int g, int b, double a);
void texture_set_levelofdetail(int texid, gs_scalar minlod, gs_scalar maxlod, int maxlevel);
void texture_mipmapping_filter(int texid, int enable);
void texture_mipmapping_generate(int texid, int levels);
bool  texture_anisotropy_supported();
float texture_anisotropy_maxlevel();
void  texture_anisotropy_filter(int texid, gs_scalar levels);
bool texture_multitexture_supported();
void texture_multitexture_enable(bool enable);

}

#endif
