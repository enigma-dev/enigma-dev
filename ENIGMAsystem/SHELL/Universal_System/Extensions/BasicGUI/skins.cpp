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

#include "Universal_System/var4.h"
#include "Platforms/General/PFmain.h" //For mouse_check_button
#include "Universal_System/Resources/resource_data.h" //For script_execute
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "elements.h"
#include "include.h"
#include "skins.h"

namespace gui{
  extern unsigned int gui_data_elements_maxid;
  extern unordered_map<unsigned int, DataElement> gui_data_elements;

	Skin::Skin(){
		//Create invisible dummy elements to use as styles for them all
		button_style = (enigma_user::gui_button_create());
		enigma_user::gui_button_set_visible(button_style,false);

		window_style = (enigma_user::gui_window_create());
		enigma_user::gui_window_set_visible(window_style,false);

		toggle_style = (enigma_user::gui_toggle_create());
		enigma_user::gui_toggle_set_visible(toggle_style,false);

		slider_style = (enigma_user::gui_slider_create());
		enigma_user::gui_slider_set_visible(slider_style,false);

		scrollbar_style = (enigma_user::gui_scrollbar_create());
		enigma_user::gui_scrollbar_set_visible(scrollbar_style,false);

    label_style = (enigma_user::gui_label_create());
		enigma_user::gui_label_set_visible(label_style,false);

    textbox_style = (enigma_user::gui_textbox_create());
    enigma_user::gui_textbox_set_visible(textbox_style,false);
	}

	int gui_bound_skin = -1;
}

namespace enigma_user
{
	int gui_skin_create(){
		gui::gui_data_elements.emplace(gui::gui_data_elements_maxid, gui::Skin());
    gui::Skin &ski = gui::gui_data_elements[gui::gui_data_elements_maxid];
		ski.id = gui::gui_data_elements_maxid;
    //printf("Creating skin with size %i\n", sizeof(gui::gui_data_elements[gui::gui_data_elements_maxid]));
		return gui::gui_data_elements_maxid++;
	}

  int gui_skin_duplicate(int skin){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,skin,-1);
    gui::gui_data_elements.emplace(gui::gui_data_elements_maxid, ski);
    gui::gui_data_elements[gui::gui_data_elements_maxid].id = gui::gui_data_elements_maxid;
    return gui::gui_data_elements_maxid++;
  }

	void gui_skin_destroy(int id){
    check_data_element(gui::GUI_TYPE::SKIN,id);
		gui::gui_data_elements.erase(gui::gui_data_elements.find(id));
	}

	void gui_skin_set(int id){
		gui::gui_bound_skin = id;
	}

	int gui_skin_get_button(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
		return (ski.button_style);
	}

  int gui_skin_get_window(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
		return (ski.window_style);
	}

  int gui_skin_get_toggle(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
		return (ski.toggle_style);
	}

	int gui_skin_get_slider(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
    return (ski.slider_style);
	}

  int gui_skin_get_scrollbar(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
    return (ski.scrollbar_style);
	}

  int gui_skin_get_label(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
    return (ski.label_style);
	}

  int gui_skin_get_textbox(int id){
    get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,id,-1);
    return (ski.textbox_style);
  }
}
