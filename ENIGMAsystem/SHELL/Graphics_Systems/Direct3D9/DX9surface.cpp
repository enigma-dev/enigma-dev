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

#include "Direct3D9Headers.h"
using namespace std;
#include <cstddef>
#include <iostream>
#include <math.h>

#include "DX9binding.h"
#include <stdio.h> //for file writing (surface_save)
#include "Universal_System/nlpo2.h"
#include "Universal_System/spritestruct.h"
#include "Collision_Systems/collision_types.h"

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00) >> 8)
#define __GETB(x) ((x & 0xFF0000) >> 16)

namespace enigma_user {
extern int room_width, room_height/*, sprite_idmax*/;
}
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
	LPDIRECT3DTEXTURE9 texture;
	d3ddev->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &texture, NULL);			 
	enigma::Surface* surface = new enigma::Surface();	 
	GmTexture* gmTexture = new GmTexture(texture);
	gmTexture->isFont = false;
    GmTextures.push_back(gmTexture);
    surface->tex = GmTextures.size() - 1;
	surface->width = width; surface->height = height;
	texture->GetSurfaceLevel(0, &surface->surf);
	
	enigma::Surfaces.push_back(surface);
	return enigma::Surfaces.size() - 1;
}

int surface_msaa_create(int width, int height, int levels)
{

}

LPDIRECT3DSURFACE9 pBackBuffer;

void surface_set_target(int id)
{
	get_surface(surface,id);
	
	d3ddev->GetRenderTarget(0, &pBackBuffer);
		// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
	DWORD wrapu, wrapv, wrapw;
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSU, &wrapu );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSV, &wrapv );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSW, &wrapw );
	
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );
	// The D3D sprite batcher uses clockwise face culling which is default but can't tell if 
	// this here should memorize it and force it to CW all the time and then reset what the user had
	// or not.
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dsprite->End();
	// And now reset the texture repetition.
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, wrapu );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, wrapv );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, wrapw );
	
	// reset the culling
	//d3d_set_culling(culling);


	d3ddev->SetRenderTarget(0, surface->surf);
	  
	dsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
}

void surface_reset_target(void)
{

		// Textures should be clamped when rendering 2D sprites and stuff, so memorize it.
	DWORD wrapu, wrapv, wrapw;
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSU, &wrapu );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSV, &wrapv );
	d3ddev->GetSamplerState( 0, D3DSAMP_ADDRESSW, &wrapw );
	
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP );
	// The D3D sprite batcher uses clockwise face culling which is default but can't tell if 
	// this here should memorize it and force it to CW all the time and then reset what the user had
	// or not.
	d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dsprite->End();
	// And now reset the texture repetition.
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSU, wrapu );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSV, wrapv );
	d3ddev->SetSamplerState( 0, D3DSAMP_ADDRESSW, wrapw );
	
	// reset the culling
	//d3d_set_culling(culling);

	d3ddev->SetRenderTarget(0, pBackBuffer);

	dsprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DO_NOT_ADDREF_TEXTURE);
}

void surface_free(int id)
{
	get_surface(surface, id);
	delete surface;
}

bool surface_exists(int id)
{
    return !((id < 0) or (id > enigma::Surfaces.size()) or (enigma::Surfaces[id] == NULL));
}

void draw_surface(int id, gs_scalar x, gs_scalar y)
{
    get_surface(surface,id);
	
	D3DXVECTOR3 offset(0, 0, 0);
	D3DXVECTOR3 pos(x, y, 0);
	dsprite->Draw(GmTextures[surface->tex]->gTexture, NULL, &offset, &pos, enigma::get_currentcolor());
}

void draw_surface_stretched(int id, gs_scalar x, gs_scalar y, float w, float h)
{

}

void draw_surface_part(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y)
{

}

void draw_surface_tiled(int id, gs_scalar x, gs_scalar y)
{

}

void draw_surface_tiled_area(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2)
{

}

void draw_surface_ext(int id,gs_scalar x, gs_scalar y,gs_scalar xscale, gs_scalar yscale,double rot,int color,double alpha)
{

}

void draw_surface_stretched_ext(int id, gs_scalar x, gs_scalar y, float w, float h, int color, double alpha)
{

}

void draw_surface_part_ext(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale,int color, double alpha)
{

}

void draw_surface_tiled_ext(int id, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

void draw_surface_tiled_area_ext(int id, gs_scalar x, gs_scalar y, float x1, float y1, float x2, float y2, gs_scalar xscale, gs_scalar yscale, int color, double alpha)
{

}

void draw_surface_general(int id, gs_scalar left, gs_scalar top, gs_scalar width, gs_scalar height, gs_scalar x, gs_scalar y, gs_scalar xscale, gs_scalar yscale, double rot, int c1, int c2, int c3, int c4, double a1, double a2, double a3, double a4)
{

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

int sprite_create_from_surface(int id, int x, int y, int w, int h, bool removeback, bool smooth, int xorig, int yorig)
{

}

void surface_copy_part(int destination, gs_scalar x, gs_scalar y, int source, int xs, int ys, int ws, int hs)
{

}

void surface_copy(int destination, gs_scalar x, gs_scalar y, int source)
{

}

}

