/** Copyright (C) 2008-2013 Josh Ventura, Alasdair Morrison
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

#include "OpenGLESHeaders.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include <math.h>

namespace enigma {
  extern unsigned char currentcolor[4];
}

namespace enigma_user {

void draw_clear_alpha(int col,float alpha)
{
  //Unfortunately, we lack a 255-based method for setting ClearColor.
	glClearColor(__GETR(col)/255.0,__GETG(col)/255.0,__GETB(col)/255.0,alpha);
	glClear(GL_COLOR_BUFFER_BIT);
}
void draw_clear(int col)
{
	glClearColor(__GETR(col)/255.0,__GETG(col)/255.0,__GETB(col)/255.0,1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void draw_set_color(int color)
{
	enigma::currentcolor[0] = __GETR(color);
	enigma::currentcolor[1] = __GETG(color);
	enigma::currentcolor[2] = __GETB(color);
	glColor4ub(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
}
void draw_set_color_rgb(unsigned char red,unsigned char green,unsigned char blue)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	glColor4ub(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);

}
void draw_set_alpha(float alpha)
{
	enigma::currentcolor[3] = bind_alpha(alpha);
	glColor4ub(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);

}
void draw_set_color_rgba(unsigned char red,unsigned char green,unsigned char blue,float alpha)
{
	enigma::currentcolor[0] = red;
	enigma::currentcolor[1] = green;
	enigma::currentcolor[2] = blue;
	enigma::currentcolor[3] = bind_alpha(alpha);
	glColor4ub(enigma::currentcolor[0],enigma::currentcolor[1],enigma::currentcolor[2],enigma::currentcolor[3]);
}
}
