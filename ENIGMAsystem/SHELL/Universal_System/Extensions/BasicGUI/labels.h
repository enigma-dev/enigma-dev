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

#ifndef BGUI_LABELS_H
#define BGUI_LABELS_H

#include "common.h"

#include <string>

namespace enigma {
namespace gui {
	class Label{
		public:
			unsigned int id;
			rect box;
			std::string text = "";

   		bool visible = true;

			int parent_id = -1; //ID of the parent of some kind (probably a window). It won't render with gui_draw_buttons() if it is.

      int style_id = -1; //The style we use

			Label();
			void draw(gs_scalar ox = 0, gs_scalar oy = 0);
	};
} //namespace gui
} //namespace enigma

#endif
