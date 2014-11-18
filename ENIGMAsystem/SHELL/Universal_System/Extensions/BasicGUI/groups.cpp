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

#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;
using std::pair;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "include.h"
#include "groups.h"

//Groups
#include "buttons.h"
#include "toggles.h"

namespace gui{

	unordered_map<unsigned int, gui_group> gui_groups;
	unsigned int gui_groups_maxid = 0;

  extern unordered_map<unsigned int, gui_button> gui_buttons;
	extern unordered_map<unsigned int, gui_toggle> gui_toggles;
}

namespace enigma_user
{
	int gui_group_create(){
		gui::gui_groups.insert(pair<unsigned int, gui::gui_group >(gui::gui_groups_maxid, gui::gui_group()));
		gui::gui_groups[gui::gui_groups_maxid].id = gui::gui_groups_maxid;
		return gui::gui_groups_maxid++;
	}

	void gui_group_destroy(int id){
		gui::gui_groups.erase(gui::gui_groups.find(id));
	}

  void gui_group_add_button(int id, int bid){
    gui::gui_groups[id].group_buttons.push_back(bid);
    gui::gui_buttons[bid].group_id = id;
  }

  void gui_group_add_toggle(int id, int tid){
    gui::gui_groups[id].group_toggles.push_back(tid);
    gui::gui_toggles[tid].group_id = id;
  }
}
