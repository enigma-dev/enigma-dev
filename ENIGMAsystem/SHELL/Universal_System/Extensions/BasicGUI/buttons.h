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

#ifndef BGUI_BUTTONS_H
#define BGUI_BUTTONS_H

#include "common.h"

namespace enigma_user
{
	extern double mouse_x, mouse_y;
}

namespace gui
{
	class gui_button{
		public:
			unsigned int id;
			rect box;
			string text;
			int state;
			bool visible;
			bool active; //Is button pressed
			bool togglable; //Is button a toggle button
			int callback; //Script to run when clicked

			int parent_id; //ID of the parent of some kind (probably a window). It won't render with gui_draw_buttons() if it is.

			font_style font_styles[5]; //0 - default, 1 - hover, 2 - active, 3 - on, 4 - on hover (this is based on enums)

			int sprite;
			int sprite_hover;
			int sprite_active;
			int sprite_on;
			int sprite_on_hover;

			rect_offset border;
			rect_offset padding;

			void reset();
			gui_button();
			//Update all possible button states (hover, click, toggle etc.)
			void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
			void draw(gs_scalar ox = 0, gs_scalar oy = 0);
			void update_text_pos(int state = -1);
	};
}

#endif
