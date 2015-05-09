/** Copyright (C) 2014-2015 Harijs Grinbergs
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

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "elements.h"
//#include "include.h"
#include "groups.h"

//Groups
#include "buttons.h"
#include "toggles.h"

namespace gui{
	extern unsigned int gui_elements_maxid;
  extern unsigned int gui_data_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;
  extern unordered_map<unsigned int, DataElement> gui_data_elements;
}

namespace enigma_user
{
	int gui_group_create(){
		gui::gui_data_elements.emplace(gui::gui_data_elements_maxid, gui::Group());
		gui::Group &g = gui::gui_data_elements[gui::gui_data_elements_maxid];
    printf("Creating group with size %i\n", sizeof(gui::gui_data_elements[gui::gui_data_elements_maxid]));
		g.id = gui::gui_data_elements_maxid;
		return gui::gui_data_elements_maxid++;
	}

	void gui_group_destroy(int id){
    check_data_element(gui::GUI_TYPE::GROUP,id);
		gui::gui_data_elements.erase(gui::gui_data_elements.find(id));
	}

  void gui_group_add_button(int id, int bid){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    gro.group_buttons.push_back(bid);
    but.group_id = id;
  }

  void gui_group_add_toggle(int id, int tid){
    get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    gro.group_toggles.push_back(tid);
    tog.group_id = id;
  }
}
