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
#include <array>
using std::array;

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
      rect indicator_box;
			string text = "";
			int state = 0;
			bool visible = true;
			bool active = false; //Is slider pressed
      bool drag = false;
			int callback = -1; //Script to run when clicked

			gs_scalar drag_xoffset = 0.0;
			gs_scalar drag_yoffset = 0.0;

      double value = 0.0; //Slider return value
      double minValue = 0.0;
      double maxValue = 1.0;
      double incValue = 0.1;

      gs_scalar slider_offset = 0.0;

			int parent_id = -1; //ID of some kind of parent (probably a window). It won't render with gui_draw_sliders() if it is not -1.

			array<font_style,6> font_styles; //0 - default, 1 - hover, 2 - active, 3 - on, 4 - on hover, 5 - on active (this is based on enums)

      array<int,6> sprites;
      array<int,6> sprites_indicator;
			/*int sprite = -1;
			int sprite_hover = -1;
			int sprite_active = -1;
			int sprite_on = -1;
			int sprite_on_hover = -1;
      int sprite_on_active = -1;*/

     	/*int sprite_indicator = -1;
			int sprite_indicator_hover = -1;
			int sprite_indicator_active = -1;
			int sprite_indicator_on = -1;
			int sprite_indicator_on_hover = -1;
      int sprite_indicator_on_active = -1;*/

			rect_offset border;
      rect_offset indicator_border;
			rect_offset padding;

			gui_slider();
			//Update all possible slider states (hover, click etc.)
			void update(gs_scalar ox = 0, gs_scalar oy = 0, gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
			void draw(gs_scalar ox = 0, gs_scalar oy = 0);
			void update_text_pos(int state = -1);
	};
}

#endif
