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
#include "skins.h"

namespace gui{
	gui_skin::gui_skin(){
		//Create invisible dummy button to use as style for them all
		button_style = (enigma_user::gui_button_create());
		enigma_user::gui_button_set_visible(button_style,false);

		//Create invisible dummy window to use as style for them all
		window_style = (enigma_user::gui_window_create());
		enigma_user::gui_window_set_visible(window_style,false);

    //Create invisible dummy toggle to use as style for them all
		toggle_style = (enigma_user::gui_toggle_create());
		enigma_user::gui_toggle_set_visible(toggle_style,false);
	}

	int gui_bound_skin = -1;
	unordered_map<unsigned int, gui_skin> gui_skins;
	unsigned int gui_skins_maxid = 0;
}

namespace enigma_user
{
	int gui_skin_create(){
		gui::gui_skins.insert(pair<unsigned int, gui::gui_skin >(gui::gui_skins_maxid, gui::gui_skin()));
		gui::gui_skins[gui::gui_skins_maxid].id = gui::gui_skins_maxid;
		return gui::gui_skins_maxid++;
	}

	void gui_skin_destroy(int id){
		gui::gui_skins.erase(gui::gui_skins.find(id));
	}

	void gui_skin_set(int id){
		gui::gui_bound_skin = id;
	}

	int gui_skin_get_button(int id){
		return (gui::gui_skins[id].button_style);
	}

  int gui_skin_get_window(int id){
		return (gui::gui_skins[id].window_style);
	}

  int gui_skin_get_toggle(int id){
		return (gui::gui_skins[id].toggle_style);
	}
}
