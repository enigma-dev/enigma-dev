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

#include <algorithm>
#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;

#include "Universal_System/var4.h"
#include "Platforms/General/PFmain.h" //For mouse_check_button
#include "Universal_System/Resources/resource_data.h" //For script_execute
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
		g.id = gui::gui_data_elements_maxid;
		return gui::gui_data_elements_maxid++;
	}

  int gui_group_duplicate(int group){
    get_data_elementv(gro,gui::Group,gui::GUI_TYPE::GROUP,group,-1);
    gui::gui_data_elements.emplace(gui::gui_data_elements_maxid, gro);
    gui::gui_data_elements[gui::gui_data_elements_maxid].id = gui::gui_data_elements_maxid;
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

  void gui_group_add_window(int id, int wid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    gro.group_windows.push_back(wid);
  }

  void gui_group_remove_button(int id, int bid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    auto it = find(gro.group_buttons.begin(), gro.group_buttons.end(), bid);
    if (it != gro.group_buttons.end()){
      get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
      gro.group_buttons.erase(it);
      but.parent_id = -1;
    }
  }

  void gui_group_remove_toggle(int id, int tid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    auto it = find(gro.group_toggles.begin(), gro.group_toggles.end(), tid);
    if (it != gro.group_toggles.end()){
      get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
      gro.group_toggles.erase(it);
      tog.parent_id = -1;
    }
  }

  void gui_group_remove_window(int id, int wid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,id);
    auto it = find(gro.group_windows.begin(), gro.group_windows.end(), wid);
    if (it != gro.group_windows.end()){
      get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,wid);
      gro.group_windows.erase(it);
      win.parent_id = -1;
    }
  }
}
