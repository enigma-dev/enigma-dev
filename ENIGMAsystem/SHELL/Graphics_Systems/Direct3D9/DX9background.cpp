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

#include <cstddef>

#include <math.h>
#include "Direct3D9Headers.h"
#include "../General/GSbackground.h"
#include "../General/GStextures.h"
#include "Universal_System/backgroundstruct.h"
#include "Universal_System/spritestruct.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_background(bck2d,back)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    if (back < 0 or size_t(back) >= enigma::background_idmax or !enigma::backgroundstructarray[back]) {\
      show_error("Attempting to draw non-existing background " + toString(back), false);\
      return r;\
    }\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#else
  #define get_background(bck2d,back)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
  #define get_backgroundnv(bck2d,back,r)\
    const enigma::background *const bck2d = enigma::backgroundstructarray[back];
#endif

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
  extern int room_width, room_height;
}
namespace enigma {
  extern size_t background_idmax;
}

#include "../General/DXbinding.h"
#include <string.h> // needed for querying ARB extensions

#include "Bridges/General/DX9Device.h"
#include "DX9TextureStruct.h"

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y)
{
  get_background(bck2d, back);
  D3DXVECTOR3 pos(x, y, 0);
  dsprite->Draw(GmTextures[bck2d->texture]->gTexture,NULL,NULL,&pos,0xFFFFFFFF);
}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
	get_background(bck2d, back);
  
	const float texw = bck2d->width, texh = bck2d->height;
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(width/texw, height/texh);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);
	
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL, 0xFFFFFFFF);
	
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
	get_background(bck2d, back);

	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, &pos, 0xFFFFFFFF);
}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);

    const float
    tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil(room_width/(bck2d->width*tbx))) + 1,
    vertil = int (ceil(room_height/(bck2d->height*tby))) + 1;

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = hortil * bck2d->width; rect.bottom = vertil * bck2d->height;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, &pos, 0xFFFFFFFF);
}

void draw_background_tiled_area(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
	get_background(bck2d,back);

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

	D3DXVECTOR3 pos(x1, y1, 0);
	tagRECT rect;
	rect.left = x1 + x; rect.top = y1 + y; rect.right = x2 + x; rect.bottom = y2 + y;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, &pos, 0xFFFFFFFF);
}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, double alpha)
{
	get_background(bck2d, back);
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,rot,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);
	
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL, 
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
		
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, double alpha)
{
	get_background(bck2d, back);
	
	const float texw = bck2d->width, texh = bck2d->height;
		
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(width/texw, height/texw);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);
	
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL, 
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
		
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{
	get_background(bck2d, back);
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);
	
	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;
	
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, NULL, 
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
		
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{
    get_background(bck2d,back);

    const float
    tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil(room_width/(bck2d->width*tbx))) + 1,
    vertil = int (ceil(room_height/(bck2d->height*tby))) + 1;

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	tagRECT rect;
	rect.left = 0; rect.top = 0; rect.right = hortil * bck2d->width; rect.bottom = vertil * bck2d->height;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, NULL,
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
		
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{
	get_background(bck2d,back);

	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );

	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x1, y1);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;

	D3DXVECTOR2 scaling(xscale, yscale);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);
	
	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);
	
	tagRECT rect;
	rect.left = x1 + x; rect.top = y1 + y; rect.right = x2 + x; rect.bottom = y2 + y;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, NULL,
		D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
		
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{

}

int background_get_texture(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texture;
}

int background_get_width(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->width;
}

int background_get_height(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->height;
}

double background_get_texture_width_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordx;
}

double background_get_texture_height_factor(int backId) {
  get_backgroundnv(bck2d,backId,-1);
  return bck2d->texbordy;
}

}

