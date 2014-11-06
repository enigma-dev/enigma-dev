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

#ifndef BGUI_WINDOWS_H
#define BGUI_WINDOWS_H

#include "common.h"
#include <vector>
using std::vector;

namespace enigma_user
{
	extern double mouse_x, mouse_y;
}

namespace gui
{
	class gui_window{
		public:
			int id;
			
			rect box;
			string text;
			int state;
			bool visible;
			bool drag;
			gs_scalar drag_xoffset;
			gs_scalar drag_yoffset;
			int callback;
			
			font_style font_styles[2]; //0 - default, 1 - on (this is based based on enums)
			
			int sprite;
			int sprite_on;

			rect_offset border;
			rect_offset padding;
			
			void reset();
			gui_window();
			//Update all possible window states (focus and unfocused)
			void update(gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
			void draw();
			void update_text_pos(int state = -1);
      
      vector<unsigned int> child_buttons;
	};
}

#endif