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

namespace enigma_user
{

bool sprite_exists(int spr) {
    return (unsigned(spr) < enigma::sprite_idmax) and bool(enigma::spritestructarray[spr]);
}

void draw_sprite(int spr,int subimg, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	D3DXVECTOR3 offset(spr2d->xoffset, spr2d->yoffset, 0);
	D3DXVECTOR3 pos(x, y, 0);
	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, NULL, &offset, &pos, 0xFFFFFFFF);
}

void draw_sprite_stretched(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	const float texw = spr2d->width, texh = spr2d->height;

	// Screen position of the sprite
	D3DXVECTOR2 center = D3DXVECTOR2(spr2d->xoffset, spr2d->yoffset);
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(width/texw, height/texh);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&center,0,&trans);

	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	// Screen position of the sprite
	D3DXVECTOR3 offset = D3DXVECTOR3(spr2d->xoffset, spr2d->yoffset, 0);

	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, NULL, &offset, NULL, 0xFFFFFFFF);

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_sprite_part(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;
	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, &rect, 0, &pos, 0xFFFFFFFF);
}

void draw_sprite_part_offset(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	D3DXVECTOR3 offset(spr2d->xoffset, spr2d->yoffset, 0);
	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;
	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, &rect, &offset, &pos, 0xFFFFFFFF);
}

void draw_sprite_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	// Screen position of the sprite
	D3DXVECTOR2 center = D3DXVECTOR2(spr2d->xoffset, spr2d->yoffset);
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&center,rot,&trans);

	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	// Screen position of the sprite
	D3DXVECTOR3 offset = D3DXVECTOR3(spr2d->xoffset, spr2d->yoffset, 0);

	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, NULL, &offset, NULL,
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_sprite_part_ext(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,0,0,&trans);

	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	// Screen position of the sprite
	D3DXVECTOR3 offset = D3DXVECTOR3(spr2d->xoffset, spr2d->yoffset, 0);

	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;

	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, NULL, &offset, NULL,
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
}

void draw_sprite_general(int spr, int subimg, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
{

}

void draw_sprite_stretched_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	const float
    texw = spr2d->width, texh = spr2d->height;

	// Screen position of the sprite
	D3DXVECTOR2 center = D3DXVECTOR2(spr2d->xoffset, spr2d->yoffset);
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(width/texw, height/texh);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&center,0,&trans);

	// Screen position of the sprite
	D3DXVECTOR3 offset = D3DXVECTOR3(spr2d->xoffset, spr2d->yoffset, 0);

	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, NULL, &offset, NULL,
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
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
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	const float
    tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi];

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby))) + 1;

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

	D3DXVECTOR3 offset(spr2d->xoffset, spr2d->yoffset, 0);
	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = hortil * spr2d->width; rect.bottom = vertil * spr2d->height;
	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, &rect, &offset, &pos, 0xFFFFFFFF);
}

void draw_sprite_tiled_ext(int spr, int subimg, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    get_spritev(spr2d,spr);
    const int usi = subimg >= 0 ? (subimg % spr2d->subcount) : int(((enigma::object_graphics*)enigma::instance_event_iterator->inst)->image_index) % spr2d->subcount;

	const float
    tbx  = spr2d->texbordxarray[usi], tby  = spr2d->texbordyarray[usi];

    const int
    hortil = int(ceil((view_enabled ? int(view_xview[view_current] + view_wview[view_current]) : room_width) / (spr2d->width*tbx))) + 1,
    vertil = int(ceil((view_enabled ? int(view_yview[view_current] + view_hview[view_current]) : room_height) / (spr2d->height*tby))) + 1;

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

	// Screen position of the sprite
	D3DXVECTOR2 center = D3DXVECTOR2(spr2d->xoffset, spr2d->yoffset);
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	D3DXVECTOR2 scaling(xscale, yscale);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&center,0,&trans);

	// Screen position of the sprite
	D3DXVECTOR3 offset = D3DXVECTOR3(spr2d->xoffset, spr2d->yoffset, 0);

	dsprite->SetTransform(&mat);

	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = hortil * spr2d->width; rect.bottom = vertil * spr2d->height;
	dsprite->Draw(GmTextures[spr2d->texturearray[usi]]->gTexture, &rect, &offset, NULL,
				D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

}

