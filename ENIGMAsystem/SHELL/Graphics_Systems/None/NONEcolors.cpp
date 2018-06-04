/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2013 Robert B. Colton, Adriano Tumminelli
*** Copyright (C) 2014 Seth N. Hetu
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
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GScolor_macros.h"
#include <math.h>

namespace enigma
{
	extern unsigned char currentcolor[4];
}

namespace enigma_user
{
	void draw_set_color(int color)
	{
		enigma::currentcolor[0] = __GETR(color);
		enigma::currentcolor[1] = __GETG(color);
		enigma::currentcolor[2] = __GETB(color);
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
	}

	void draw_set_color_write_enable(bool red, bool green, bool blue, bool alpha){}
}
