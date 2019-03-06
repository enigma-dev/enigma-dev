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

#include "Direct3D11Headers.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include <math.h>

using namespace enigma::dx11;

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user
{

void draw_clear_alpha(int col, float alpha)
{
	draw_batch_flush(batch_flush_deferred);
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = COL_GET_Rf(col);
	color[1] = COL_GET_Gf(col);
	color[2] = COL_GET_Bf(col);
	color[3] = alpha;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = COL_GET_Rf(col);
	color[1] = COL_GET_Gf(col);
	color[2] = COL_GET_Bf(col);
	color[3] = 1;

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
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

void draw_set_color_write_enable(bool red, bool green, bool blue, bool alpha)
{
	draw_batch_flush(batch_flush_deferred);
}

}
