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

#include "Bridges/General/DX11Context.h"
#include "Direct3D11Headers.h"
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
#include "DX11SurfaceStruct.h"
#include "DX11TextureStruct.h"

namespace enigma
{
  vector<Surface*> Surfaces(0);
  
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

}

int surface_create_msaa(int width, int height, int levels)
{

}

void surface_set_target(int id)
{

}

void surface_reset_target()
{

}

void surface_free(int id)
{

}

bool surface_exists(int id)
{

}

int surface_get_texture(int id)
{

}

int surface_get_width(int id)
{

}

int surface_get_height(int id)
{

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

