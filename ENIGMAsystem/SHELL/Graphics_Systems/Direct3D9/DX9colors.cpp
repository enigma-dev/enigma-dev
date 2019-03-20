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
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSprimitives.h"

#include <math.h>

using namespace enigma::dx9;

namespace enigma {

extern unsigned char currentcolor[4];

} // namespace enigma

namespace enigma_user {

void draw_clear_alpha(int col, float alpha)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(COL_GET_R(col), COL_GET_G(col), COL_GET_B(col), CLAMP_ALPHA(alpha)), 1.0f, 0);
}

void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	d3dmgr->device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(COL_GET_R(col), COL_GET_G(col), COL_GET_B(col)), 1.0f, 0);
}

void draw_set_color(int col)
{
	enigma::currentcolor[0] = COL_GET_R(col);
	enigma::currentcolor[1] = COL_GET_G(col);
	enigma::currentcolor[2] = COL_GET_B(col);
}

void draw_set_color_rgb(unsigned char red,unsigned char green,unsigned char blue)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
}

void draw_set_alpha(float alpha)
{
	enigma::currentcolor[3] = CLAMP_ALPHA(alpha);
}

void draw_set_color_rgba(unsigned char red,unsigned char green,unsigned char blue,float alpha)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	enigma::currentcolor[3] = CLAMP_ALPHA(alpha);
}

} // namespace enigma_user
