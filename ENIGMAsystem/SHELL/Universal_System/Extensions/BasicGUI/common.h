/** Copyright (C) 2014 Harijs Grinbergs
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

#ifndef BGUI_COMMON_H
#define BGUI_COMMON_H

namespace gui
{
	struct rect_offset{
		gs_scalar top, left, bottom, right;
		rect_offset(){ top = 0, left = 0, bottom = 0, right = 0; }
		void set(gs_scalar l, gs_scalar t, gs_scalar r, gs_scalar b) { left = l, top = t, right = r, bottom = b; }
	};
	struct rect{
		gs_scalar x, y, w, h;
		rect(){ x = 0, y = 0, w = 0, h = 0; }
		bool point_inside(gs_scalar tx, gs_scalar ty){ return (tx>x && tx<x+w && ty>y && ty<y+h); }
	};
	struct font_style{
		int font;
		unsigned int halign;
		unsigned int valign;
		int color;
		gs_scalar alpha;
		gs_scalar textx, texty;
		font_style(){ font = -1, halign = enigma_user::fa_left, valign = enigma_user::fa_top, color = enigma_user::c_white, alpha = 1.0;  }
		void use(){
			enigma_user::draw_set_halign(halign);
			enigma_user::draw_set_valign(valign);
			enigma_user::draw_set_font(font);
			enigma_user::draw_set_color(color);
			enigma_user::draw_set_alpha(alpha);
		}
	};
}

#endif