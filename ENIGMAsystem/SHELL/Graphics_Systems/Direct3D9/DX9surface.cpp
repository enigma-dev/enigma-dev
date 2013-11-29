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

#include "Bridges/General/DX9Context.h"
#include "Direct3D9Headers.h"
using namespace std;
#include <cstddef>
#include <iostream>
#include <math.h>


#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Universal_System/backgroundstruct.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
extern int room_width, room_height/*, sprite_idmax*/;
}

#include "../General/GSprimitives.h"
#include "../General/GSsurface.h"
#include "DX9SurfaceStruct.h"
#include "DX9TextureStruct.h"

namespace enigma
{
  vector<Surface*> Surfaces(0);
  
  D3DCOLOR get_currentcolor();
}


namespace enigma_user
{

bool surface_is_supported()
{
	//TODO: Implement with IDirect3D9::CheckDeviceFormat
	return true;
}

int surface_create(int width, int height)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	d3dmgr->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);			 
	enigma::Surface* surface = new enigma::Surface();	 
	TextureStruct* gmTexture = new TextureStruct(texture);
	gmTexture->isFont = false;
    textureStructs.push_back(gmTexture);
	//d3dmgr->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
	texture->GetSurfaceLevel(0,&surface->surf);
    surface->tex = textureStructs.size() - 1;
	surface->width = width; surface->height = height;
	enigma::Surfaces.push_back(surface);
	return enigma::Surfaces.size() - 1;
}

int surface_create_msaa(int width, int height, int levels)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	d3dmgr->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);			 
	enigma::Surface* surface = new enigma::Surface();	 
	TextureStruct* gmTexture = new TextureStruct(texture);
	gmTexture->isFont = false;
    textureStructs.push_back(gmTexture);
	d3dmgr->CreateRenderTarget(width, height, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_2_SAMPLES, 2, false, &surface->surf, NULL);
    surface->tex = textureStructs.size() - 1;
	surface->width = width; surface->height = height;
	enigma::Surfaces.push_back(surface);
	return enigma::Surfaces.size() - 1;
}

LPDIRECT3DSURFACE9 pBackBuffer;

void surface_set_target(int id)
{
	get_surface(surface,id);
d3dmgr->device->GetRenderTarget(0, &pBackBuffer);
	d3dmgr->device->SetRenderTarget(0, surface->surf);
	
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,D3DX_PI / 4.0f,1,1,100);
	//set projection matrix
	d3dmgr->SetTransform(D3DTS_PROJECTION,&matProjection);
}

void surface_reset_target()
{
	//d3dmgr->ResetRenderTarget();
	d3dmgr->EndShapesBatching();
	d3dmgr->device->SetRenderTarget(0, pBackBuffer);
}

void surface_free(int id)
{
	get_surface(surf, id);
	delete surf;
}

bool surface_exists(int id)
{
    return !((id < 0) or (id > enigma::Surfaces.size()) or (enigma::Surfaces[id] == NULL));
}

void draw_surface(int id, gs_scalar x, gs_scalar y)
{
    get_surface(surf,id);

	int w=surf->width;
	int h=surf->height;
  
	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture(x,y,0,0);
	draw_vertex_texture(x+w,y,1,0);
	draw_vertex_texture(x,y+h,0,1);
	draw_vertex_texture(x+w,y+h,1,1);
	draw_primitive_end();
}

void draw_surface_ext(int id,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale,double rot,int color,double alpha)
{
    get_surface(surf,id);

    const gs_scalar w=surf->width*xscale, h=surf->height*yscale;
    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture_color(ulcx,ulcy,0,0,color,alpha);
	draw_vertex_texture_color(ulcx + w*cos(rot), ulcy - w*sin(rot),1,0,color,alpha);
	draw_vertex_texture_color(ulcx,ulcy,0,1,color,alpha);
	draw_vertex_texture_color(ulcx + w*cos(rot), ulcy - w*sin(rot),1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, float w, float h)
{
    get_surface(surf,id);
  
	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture(x,y,0,0);
	draw_vertex_texture(x+w,y,1,0);
	draw_vertex_texture(x,y+h,0,1);
	draw_vertex_texture(x+w,y+h,1,1);
	draw_primitive_end();
}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, int color, double alpha)
{
    get_surface(surf,id);
  
	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture_color(x,y,0,0,color,alpha);
	draw_vertex_texture_color(x+w,y,1,0,color,alpha);
	draw_vertex_texture_color(x,y+h,0,1,color,alpha);
	draw_vertex_texture_color(x+w,y+h,1,1,color,alpha);
	draw_primitive_end();
}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar w, gs_scalar h, gs_scalar x, gs_scalar y)
{
	get_surface(surf,id);
  
	const gs_scalar tbw=surf->width,tbh=surf->height;
	
	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture(x,y,left/tbw,top/tbh);
	draw_vertex_texture(x+w,y,(left+w)/tbw,top/tbh);
	draw_vertex_texture(x,y+h,left/tbw,(top+h)/tbh);
	draw_vertex_texture(x+w,y+h,(left+w)/tbw,(top+h)/tbh);
	draw_primitive_end();
}

void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar w, gs_scalar h, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale,int color, double alpha)
{
	get_surface(surf,id);

	const gs_scalar tbw = surf->width, tbh = surf->height;
	draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture_color(x,y,left/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x+w*xscale,y,(left+w)/tbw,top/tbh,color,alpha);
	draw_vertex_texture_color(x,y+h*yscale,left/tbw,(top+h)/tbh,color,alpha);
	draw_vertex_texture_color(x+w*xscale,y+h*yscale,(left+w)/tbw,(top+h)/tbh,color,alpha);
	draw_primitive_end();
}

void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{
	get_surface(surf,id);
	const gs_scalar tbw = surf->width, tbh = surf->height,
	  w = width*xscale, h = height*yscale;

    rot *= M_PI/180;

    gs_scalar ulcx = x + xscale * cos(M_PI+rot) + yscale * cos(M_PI/2+rot),
          ulcy = y - yscale * sin(M_PI+rot) - yscale * sin(M_PI/2+rot);

    draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
	draw_vertex_texture_color(ulcx,ulcy,left/tbw,top/tbh,c1,a1);
	draw_vertex_texture_color((ulcx + w*cos(rot)), (ulcy - w*sin(rot)), (left+width)/tbw,top/tbh, c2, a2);
		
      ulcx += h * cos(3*M_PI/2 + rot);
      ulcy -= h * sin(3*M_PI/2 + rot);
	  
	draw_vertex_texture_color((ulcx + w*cos(rot)), (ulcy - w*sin(rot)), (left+width)/tbw,(top+height)/tbh, c3, a3);
	draw_vertex_texture_color(ulcx, ulcy, left/tbw, (top+height)/tbh, c4, a4);
    draw_primitive_end();
}

void draw_surface_tiled(int id, gs_scalar x, gs_scalar y)
{
	get_surface(surf,id);

	x=surf->width-fmod(x,surf->width);
	y=surf->height-fmod(y,surf->height);
	const int hortil= int (ceil(room_width/(surf->width))),
			  vertil= int (ceil(room_height/(surf->height)));

    
	for (int i=0; i<hortil; i++)
	{
		for (int c=0; c<vertil; c++)
		{
			draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
			draw_vertex_texture(i*surf->width-x,c*surf->height-y,0,0);
			draw_vertex_texture((i+1)*surf->width-x,c*surf->height-y,1,0);
			draw_vertex_texture(i*surf->width-x,(c+1)*surf->height-y,0,1);
			draw_vertex_texture((i+1)*surf->width-x,(c+1)*surf->height-y,1,1);
			draw_primitive_end();
		}
	}
}

void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{
	get_surface(surf,id);

    const gs_scalar w=surf->width*xscale, h=surf->height*yscale;
    const int hortil= int (ceil(room_width/(surf->width))),
        vertil= int (ceil(room_height/(surf->height)));
    x=w-fmod(x,w);
    y=h-fmod(y,h);
 
    for (int i=0; i<hortil; i++)
    {
      for (int c=0; c<vertil; c++)
      {		  
		draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
		draw_vertex_texture_color(i*w-x,c*h-y,0,0,color,alpha);
		draw_vertex_texture_color((i+1)*w-x,c*h-y,1,0,color,alpha);
		draw_vertex_texture_color(i*w-x,(c+1)*h-y,0,1,color,alpha);
		draw_vertex_texture_color((i+1)*w-x,(c+1)*h-y,1,1,color,alpha);
		draw_primitive_end();
      }
    }
}

void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2)
{
	get_surface(surf,id);
	
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width;
    sh = surf->height;

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
		  
		draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
		draw_vertex_texture(X,Y,left/sw,top/sh);
		draw_vertex_texture(X+width,Y,(left+width)/sw,top/sh);
		draw_vertex_texture(X,Y+height,left/sw,(top+height)/sh);
		draw_vertex_texture(X+width,Y+height,(left+width)/sw,(top+height)/sh);
		draw_primitive_end();
      }
      j = jj;
    }
}

void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{
	get_surface(surf,id);
  
    gs_scalar sw,sh,i,j,jj,left,top,width,height,X,Y;
    sw = surf->width*xscale;
    sh = surf->height*yscale;

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
		  
		draw_primitive_begin_texture(pr_trianglestrip, surf->tex);
		draw_vertex_texture_color(X,Y,left/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y,(left+width)/sw,top/sh,color,alpha);
		draw_vertex_texture_color(X,Y+height,left/sw,(top+height)/sh,color,alpha);
		draw_vertex_texture_color(X+width,Y+height,(left+width)/sw,(top+height)/sh,color,alpha);
		draw_primitive_end();
      }
      j = jj;
    }
}

int surface_get_texture(int id)
{
	get_surfacev(surf,id,-1);
	return (surf->tex);
}

int surface_get_width(int id)
{
    get_surfacev(surf,id,-1);
    return (surf->width);
}

int surface_get_height(int id)
{
    get_surfacev(surf,id,-1);
    return (surf->height);
}

int surface_getpixel(int id, int x, int y)
{

}

int surface_getpixel_ext(int id, int x, int y)
{

}

int surface_getpixel_alpha(int id, int x, int y)
{

}

int surface_get_bound()
{

}

}

//////////////////////////////////////SAVE TO FILE AND CTEATE SPRITE FUNCTIONS/////////
//Fuck whoever did this to the spec
#ifndef DX_BGR
  #define DX_BGR 0x80E0
#endif

#include "Universal_System/estring.h"

namespace enigma_user
{

int surface_save(int id, string filename)
{

}

int surface_save_part(int id, string filename, unsigned x, unsigned y, unsigned w, unsigned h)
{

}

int background_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload)
{

}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, bool preload, int xorig, int yorig)
{

}

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{
	return sprite_create_from_surface(id, x, y, w, h, removeback, smooth, true, xorig, yorig);
}

void sprite_add_from_surface(int ind, int id, int x, int y, int w, int h, bool removeback, bool smooth)
{

}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{

}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{

}

}

