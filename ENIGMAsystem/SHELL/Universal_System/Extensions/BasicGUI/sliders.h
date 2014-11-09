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

#ifndef BGUI_SLIDER_H
#define BGUI_SLIDER_H

#include "common.h"

namespace enigma_user
{
	extern double mouse_x, mouse_y;
}

namespace gui
{
	class gui_slider{
		public:
			unsigned int id;
			rect box;
			string text;
			int state;
			bool visible;
			bool active; //Is slider pressed
			int callback; //Script to run when clicked

      double value; //Slider return value
      double minValue;
      double maxValue;
      double incValue;

			int parent_id; //ID of some kind of parent (probably a window). It won't render with gui_draw_sliders() if it is not -1.

			font_style font_styles[6]; //0 - default, 1 - hover, 2 - active, 3 - on, 4 - on hover, 5 - on active (this is based on enums)

			int sprite;
			int sprite_hover;
			int sprite_active;
			int sprite_on;
			int sprite_on_hover;
      int sprite_on_active;

			rect_offset border;
			rect_offset padding;

			void reset();
			gui_slider();
			//Update all possible slider states (hover, click etc.)
			void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
			void draw(gs_scalar ox = 0, gs_scalar oy = 0);
			void update_text_pos(int state = -1);
	};
}

#endif
