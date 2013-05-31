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

/* These functions are primarily for use of the engine. Experienced users
 * who are familiar with C++ can make use of these, but they were made to
 * use at load time with data read from the executable. These both expect
 * RAW format, RGB only.
 */

#ifndef _DXTEXTURES__H
#define _DXTEXTURES__H

namespace enigma
{
    extern bool interpolate_textures;
}

#include <vector>
using std::vector;

struct GmTexture {
	GmTexture(unsigned gtex);
	~GmTexture();
};
extern vector<GmTexture*> GmTextures;


namespace enigma_user {
enum {
  tx_none,
  tx_nearest,
  tx_bilinear,
  tx_trilinear
};
}

unsigned get_texture(int texid); // fail safe macro

namespace enigma_user
{

/** description
@param enable
**/
void texture_set_enabled(bool enable);
/** description
@param enable
**/
void texture_set_interpolation(int enable);
/** description
**/
bool texture_get_interpolation();
/** description
@param texid
**/
int texture_get_pixwidth(int texid);
/** description
@param texid
**/
int texture_get_pixheight(int texid);
/** description
@param enable
**/
void texture_set_blending(bool enable);
/** description
@param repeat
**/
void texture_set_repeat(bool repeat);
/** description
@param texid
@param repeat
**/
void texture_set_repeat(int texid, bool repeat);
/** description
@param texid
@param repeatu
@param repeatv
@param repeatw
**/
void texture_set_repeat(int texid, bool repeatu, bool repeatv, bool repeatw);
/** description
@param texid
**/
void texture_preload(int texid);
/** description
@param texid
@param prio
**/
void texture_set_priority(int texid, double prio);
/** description
@param texid
@param r
@param g 
@param b
@param a
**/
void texture_set_border(int texid, int r, int g, int b, double a);
/** description
@param texid
@param enable
**/
void texture_mipmapping_filter(int texid, int enable);
/** description
@param texid
@param levels
**/
void texture_mipmapping_generate(int texid, int levels);
/** description
**/
bool  texture_anisotropy_supported();
/** description
**/
float texture_anisotropy_maxlevel();
/** description
@param texid
@param levels
**/
void  texture_anisotropy_filter(int texid, float levels);
/** description
**/
bool texture_multitexture_supported();
/** description
@param enable
**/
void texture_multitexture_enable(bool enable);

}

#endif
