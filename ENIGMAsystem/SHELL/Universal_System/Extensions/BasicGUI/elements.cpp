/** Copyright (C) 2015 Harijs Grinbergs
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

#include <unordered_map>
using std::unordered_map;
using std::pair;

#include "elements.h"
#include "windows.h"
#include "toggles.h"
#include "buttons.h"
#include "groups.h"
#include "labels.h"
#include "scrollbars.h"
#include "styles.h"
#include "skins.h"
#include "sliders.h"

namespace gui
{
  unsigned int gui_styles_maxid = 0;
	unsigned int gui_labels_maxid = 0;
  unsigned int gui_scrollbars_maxid = 0;
  unsigned int gui_skins_maxid = 0;
	unsigned int gui_windows_maxid = 0;
  unsigned int gui_elements_maxid = 0;

	unordered_map<unsigned int, gui_style> gui_styles;
  unordered_map<unsigned int, gui_label> gui_labels;
  unordered_map<unsigned int, gui_scrollbar> gui_scrollbars;
  unordered_map<unsigned int, gui_skin> gui_skins;
  unordered_map<unsigned int, gui_window> gui_windows;
  unordered_map<unsigned int, Element> gui_elements;
}
