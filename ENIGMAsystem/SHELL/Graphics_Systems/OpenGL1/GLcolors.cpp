/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
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

#include "Graphics_Systems/OpenGL/OpenGLHeaders.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include "Graphics_Systems/General/GSprimitives.h"
#include <math.h>

namespace enigma_user
{

void draw_clear_alpha(int col,float alpha)
{
	draw_batch_flush(batch_flush_deferred);
  //Unfortunately, we lack a 255-based method for setting ClearColor.
	glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}
void draw_clear(int col)
{
	draw_batch_flush(batch_flush_deferred);
	glClearColor(COL_GET_Rf(col),COL_GET_Gf(col),COL_GET_Bf(col),1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void draw_set_color(int color)
{
	enigma::currentcolor[0] = COL_GET_R(color);
	enigma::currentcolor[1] = COL_GET_G(color);
	enigma::currentcolor[2] = COL_GET_B(color);
	glColor4ubv(enigma::currentcolor);
}

void draw_set_color_rgb(unsigned char red,unsigned char green,unsigned char blue)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	glColor4ubv(enigma::currentcolor);
}

void draw_set_alpha(float alpha)
{
	enigma::currentcolor[3] = CLAMP_ALPHA(alpha);
	glColor4ubv(enigma::currentcolor);
}

void draw_set_color_rgba(unsigned char red,unsigned char green,unsigned char blue,float alpha)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	enigma::currentcolor[3] = CLAMP_ALPHA(alpha);
	glColor4ubv(enigma::currentcolor);
}

void draw_set_color_write_enable(bool red, bool green, bool blue, bool alpha)
{
	draw_batch_flush(batch_flush_deferred);
	glColorMask(red, green, blue, alpha);
}
}
