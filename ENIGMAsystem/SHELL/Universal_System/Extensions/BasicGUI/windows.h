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
#include <array>
#include <vector>

using std::array;
using std::vector;

#include "common.h"

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
      rect min_box = rect(0,0,50,50); //If resizable then this is the smallest it can be
			string text = "";
			int state = 0;
			bool visible = true;
			bool drag = false;
      bool draggable = true; //Specifies if the window can be moved
      bool resize = false;
      bool resizable = false; //Specifies if the window can be resized
      int resize_side = 0; //0 - resize left, 1 - top-left, 2 - top, 3 - top-right, 4 - right, 5 bottom-right, 6 - bottom, 7 - bottom-left
      gs_scalar resize_width = 0.0;
      gs_scalar resize_height = 0.0;
      gs_scalar resize_xoffset = 0.0;
      gs_scalar resize_yoffset = 0.0;

			gs_scalar drag_xoffset = 0.0;
			gs_scalar drag_yoffset = 0.0;

			array<int,5> callback; //Script to run on event

      int style_id = -1;

			gui_window();
			//Update all possible window states (focus and unfocused)
			void update(gs_scalar tx = enigma_user::mouse_x, gs_scalar ty = enigma_user::mouse_y);
			void draw();
			void update_text_pos(int state = -1);
      void callback_execute(int event);

      vector<unsigned int> child_buttons;
      vector<unsigned int> child_toggles;
      vector<unsigned int> child_sliders;
      vector<unsigned int> child_labels;
	};
}

#endif
