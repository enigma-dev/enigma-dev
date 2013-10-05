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
#include "Graphics_Systems/General/GSbackground.h"
#include "Graphics_Systems/General/GStextures.h"
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
  D3DCOLOR get_currentcolor();
}

#include "DX9binding.h"
#include <string.h> // needed for querying ARB extensions

#include "Bridges/General/DX9Device.h"
#include "DX9TextureStruct.h"

namespace enigma_user
{

void draw_background(int back, gs_scalar x, gs_scalar y)
{
  get_background(bck2d, back);
  D3DXVECTOR3 pos(x, y, 0);
  dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, &pos, enigma::get_currentcolor());
}

void draw_background_stretched(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height)
{
	get_background(bck2d, back);

	const float texw = bck2d->width, texh = bck2d->height;

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

	D3DXVECTOR2 scaling(width/texw, height/texh);

	// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);

	// Tell the sprite about the matrix
	dsprite->SetTransform(&mat);

	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL, enigma::get_currentcolor());

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_part(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{
	get_background(bck2d, back);

	D3DXVECTOR3 pos(x, y, 0);
	tagRECT rect;
	rect.left = left; rect.top = top; rect.right = left + width; rect.bottom = top + height;
	dsprite->Draw(GmTextures[bck2d->texture]->gTexture, &rect, NULL, &pos, enigma::get_currentcolor());
}

void draw_background_tiled(int back, gs_scalar x, gs_scalar y)
{
    get_background(bck2d,back);

    x = bck2d->width-fmod(x,bck2d->width);
    y = bck2d->height-fmod(y,bck2d->height);

    const float
    tbx = bck2d->texbordx,tby=bck2d->texbordy;

    const int
    hortil = int (ceil(room_width/(bck2d->width*tbx))) + 1,
    vertil = int (ceil(room_height/(bck2d->height*tby))) + 1;

    float xvert1 = -x, xvert2 = xvert1 + bck2d->width, yvert1, yvert2;
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -y; yvert2 = yvert1 + bck2d->height;
        for (int c=0; c<vertil; c++)
        {
			
			D3DXVECTOR3 pos(xvert1, yvert1, 0);
			dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, &pos, enigma::get_currentcolor());
	
            yvert1 = yvert2;
            yvert2 += bck2d->height;
        }
        xvert1 = xvert2;
        xvert2 += bck2d->width;
    }
}

void draw_background_tiled_area(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2)
{
	get_background(bck2d,back);

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width;
    sh = bck2d->height;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;
		  
		D3DXVECTOR3 pos(X, Y, 0);
		dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, &pos, enigma::get_currentcolor());
      }
      j = jj;
    }
}

void draw_background_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

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

void draw_background_stretched_ext(int back, gs_scalar x, gs_scalar y, gs_scalar width, gs_scalar height, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	const float texw = bck2d->width, texh = bck2d->height;

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

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

void draw_background_part_ext(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
	get_background(bck2d, back);

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	
	// Screen position of the sprite
	D3DXVECTOR2 trans = D3DXVECTOR2(x, y);

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

void draw_background_tiled_ext(int back, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
    get_background(bck2d,back);

    const float
    tbx = bck2d->texbordx, tby = bck2d->texbordy,
    width_scaled = bck2d->width*xscale, height_scaled = bck2d->height*yscale;

    x = width_scaled-fmod(x,width_scaled);
    y = height_scaled-fmod(y,height_scaled);

    const int
    hortil = int(ceil(room_width/(width_scaled*tbx))) + 1,
    vertil = int(ceil(room_height/(height_scaled*tby))) + 1;

    float xvert1 = -x, xvert2 = xvert1 + width_scaled, yvert1, yvert2;
	
	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
			
    for (int i=0; i<hortil; i++)
    {
        yvert1 = -y; yvert2 = yvert1 + height_scaled;
        for (int c=0; c<vertil; c++)
        {
		
			// Screen position of the sprite
			D3DXVECTOR2 trans = D3DXVECTOR2(xvert1, yvert1);

			D3DXVECTOR2 scaling(xscale, yscale);

			// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
			D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);

			// Tell the sprite about the matrix
			dsprite->SetTransform(&mat);
			
			dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL,
				D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
	
            yvert1 = yvert2;
            yvert2 += height_scaled;
        }
        xvert1 = xvert2;
        xvert2 += width_scaled;
    }
	
	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_tiled_area_ext(int back, gs_scalar x, gs_scalar y, gs_scalar x1, gs_scalar y1, gs_scalar x2, gs_scalar y2, gs_scalar xscale, gs_scalar yscale, int color, gs_scalar alpha)
{
	get_background(bck2d,back);

    const float tbx=bck2d->texbordx,tby=bck2d->texbordy;
    float sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = bck2d->width*xscale;
    sh = bck2d->height*yscale;

    i = x1-(fmod(x1,sw) - fmod(x,sw)) - sw*(fmod(x1,sw)<fmod(x,sw));
    j = y1-(fmod(y1,sh) - fmod(y,sh)) - sh*(fmod(y1,sh)<fmod(y,sh));
    jj = j;

	// Build our matrix to rotate, scale and position our sprite
	D3DXMATRIX mat;
	
    for(; i<=x2; i+=sw)
    {
      for(; j<=y2; j+=sh)
      {
        if(i <= x1) left = x1-i;
        else left = 0;
        X = i+left;

        if(j <= y1) top = y1-j;
        else top = 0;
        Y = j+top;

        if(x2 <= i+sw) width = ((sw)-(i+sw-x2)+1)-left;
        else width = sw-left;

        if(y2 <= j+sh) height = ((sh)-(j+sh-y2)+1)-top;
        else height = sh-top;

		// Screen position of the sprite
		D3DXVECTOR2 trans = D3DXVECTOR2(X, Y);

		D3DXVECTOR2 scaling(xscale, yscale);

		// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
		D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,NULL,0,&trans);

		// Tell the sprite about the matrix
		dsprite->SetTransform(&mat);

		dsprite->Draw(GmTextures[bck2d->texture]->gTexture, NULL, NULL, NULL,
			D3DCOLOR_ARGB(char(alpha*255), __GETR(color), __GETG(color), __GETB(color)));
      }
      j = jj;
    }

	D3DXMatrixTransformation2D(&mat,NULL,0.0,0,NULL,0,0);
	dsprite->SetTransform(&mat);
}

void draw_background_general(int back, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, gs_scalar a1, gs_scalar a2, gs_scalar a3, gs_scalar a4)
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

