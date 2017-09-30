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
#include "Graphics_Systems/General/GScolors.h"
#include <math.h>

#define __GETR(x) ((x & 0x0000FF))
#define __GETG(x) ((x & 0x00FF00)>>8)
#define __GETB(x) ((x & 0xFF0000)>>16)
/*#define __GETRf(x) fmod(x,256)
#define __GETGf(x) fmod(x/256,256)
#define __GETBf(x) fmod(x/65536,256)*/

#define bind_alpha(alpha) (alpha>1?255:(alpha<0?0:(unsigned char)(alpha*255)))

namespace enigma {
  extern unsigned char currentcolor[4];
  D3DCOLOR get_currentcolor() {
	return D3DCOLOR_RGBA(currentcolor[0], currentcolor[1], currentcolor[2], currentcolor[3]);
  }
}

namespace enigma_user
{

void draw_clear_alpha(int col, float alpha)
{
	d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_COLORVALUE(__GETR(col), __GETG(col), __GETB(col), alpha), 1.0f, 0);
}

void draw_clear(int col)
{
	d3dmgr->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(__GETR(col), __GETG(col), __GETB(col)), 1.0f, 0);
}

void draw_set_color(int col)
{
	enigma::currentcolor[0] = __GETR(col);
	enigma::currentcolor[1] = __GETG(col);
	enigma::currentcolor[2] = __GETB(col);
}

void draw_set_color_rgb(unsigned char red,unsigned char green,unsigned char blue)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
}

void draw_set_alpha(float alpha)
{
	enigma::currentcolor[3] = bind_alpha(alpha);
}

void draw_set_color_rgba(unsigned char red,unsigned char green,unsigned char blue,float alpha)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	enigma::currentcolor[3] = bind_alpha(alpha);
	D3DCOLOR D3DColor = D3DCOLOR_RGBA(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2], enigma::currentcolor[3]);
}

void draw_set_color_write_enable(bool red, bool green, bool blue, bool alpha)
{
	DWORD flags = NULL;
	if (red) { flags |= D3DCOLORWRITEENABLE_RED; }
	if (green) { flags |= D3DCOLORWRITEENABLE_GREEN; }
	if (blue) { flags |= D3DCOLORWRITEENABLE_BLUE; }
	if (alpha) { flags |= D3DCOLORWRITEENABLE_ALPHA; }
	d3dmgr->SetRenderState(D3DRS_COLORWRITEENABLE, flags);
}
}
