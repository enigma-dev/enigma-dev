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

#include <cmath>
#include <cstdlib>
#include <string>
using std::string;

#include "Direct3D9Headers.h"
#include "../General/GSsprite.h"
#include "../General/GStextures.h"
#include "../General/DXbinding.h"

#include "Universal_System/spritestruct.h"
#include "Universal_System/instance_system.h"
#include "Universal_System/graphics_object.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)


#ifdef DEBUG_MODE
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_sprite(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax or !enigma::spritestructarray[id]) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    if (id < -1 or size_t(id) > enigma::sprite_idmax) { \
      show_error("Cannot access sprite with id " + toString(id), false); \
      return r; \
    } const enigma::sprite *const spr = enigma::spritestructarray[id];
#else
  #define get_sprite(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_spritev(spr,id) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
  #define get_sprite_null(spr,id,r) \
    const enigma::sprite *const spr = enigma::spritestructarray[id];
#endif

#include "Direct3D9Headers.h"
#include "Bridges/General/DX9Device.h"
#include "DX9TextureStruct.h"

	LPD3DXSPRITE sprite=NULL;
	
namespace enigma_user
{

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

void draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

    const float tbx = spr2d->texbordxarray[usi], tby = spr2d->texbordyarray[usi],
                xvert1 = x-spr2d->xoffset, xvert2 = xvert1 + spr2d->width,
                yvert1 = y-spr2d->yoffset, yvert2 = yvert1 + spr2d->height;
	

	if (sprite == NULL) {
	if (SUCCEEDED(D3DXCreateSprite(d3ddev,&sprite)))
	{
		// created OK
	}
	}
	
	D3DXVECTOR3 pos;

	pos.x = x;
	pos.y = y;
	pos.z = 0.0f;

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture,NULL,NULL,&pos,0xFFFFFFFF);
	sprite->End();
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{

}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{

}

void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{

}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int blend, double alpha)
{

}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

/* Copyright (C) 2010 Harijs Grînbergs, Josh Ventura
 * The applicable license does not change for this portion of the file.
 */

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{

}

void draw_sprite_stretched_ext(int spr,int subimg,gs_scalar x, gs_scalar y,gs_scalar width, gs_scalar height, int blend, double alpha)
{

}

}

// These two leave a bad taste in my mouth because they depend on views, which should be removable.
// However, for now, they stay.

#include <string>
using std::string;
#include "Universal_System/var4.h"
#include "Universal_System/roomsystem.h"

namespace enigma_user
{

void draw_sprite_tiled(int spr, int subimg, gs_scalar x, gs_scalar y)
{

}

void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

}

