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

#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Platforms/General/PFmain.h"                  //For mouse_check_button
#include "Universal_System/Resources/resource_data.h"  //For script_execute
#include "Universal_System/var4.h"

#include "elements.h"
#include "include.h"
#include "skins.h"

#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;

using namespace enigma::gui;

namespace enigma {
namespace gui {
int gui_bound_skin = -1;
Skin::Skin() {
  //Create invisible dummy elements to use as styles for them all
  button_style = (enigma_user::gui_button_create());
  enigma_user::gui_button_set_visible(button_style, false);

  window_style = (enigma_user::gui_window_create());
  enigma_user::gui_window_set_visible(window_style, false);

  toggle_style = (enigma_user::gui_toggle_create());
  enigma_user::gui_toggle_set_visible(toggle_style, false);

  slider_style = (enigma_user::gui_slider_create());
  enigma_user::gui_slider_set_visible(slider_style, false);

  scrollbar_style = (enigma_user::gui_scrollbar_create());
  enigma_user::gui_scrollbar_set_visible(scrollbar_style, false);

  label_style = (enigma_user::gui_label_create());
  enigma_user::gui_label_set_visible(label_style, false);

  textbox_style = (enigma_user::gui_textbox_create());
  enigma_user::gui_textbox_set_visible(textbox_style, false);
}

}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_skin_create() {
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, Skin());
  Skin &ski = guiElements.gui_data_elements[guiElements.gui_data_elements_maxid];
  ski.id = guiElements.gui_data_elements_maxid;
  //printf("Creating skin with size %i\n", sizeof(guiElements.gui_data_elements[guiElements.gui_data_elements_maxid]));
  return guiElements.gui_data_elements_maxid++;
}

int gui_skin_duplicate(int skin) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, skin, -1);
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, ski);
  guiElements.gui_data_elements[guiElements.gui_data_elements_maxid].id = guiElements.gui_data_elements_maxid;
  return guiElements.gui_data_elements_maxid++;
}

void gui_skin_destroy(int id) {
  check_data_element(GUI_TYPE::SKIN, id);
  guiElements.gui_data_elements.erase(guiElements.gui_data_elements.find(id));
}

void gui_skin_set(int id) { gui_bound_skin = id; }

int gui_skin_get_button(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.button_style);
}

int gui_skin_get_window(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.window_style);
}

int gui_skin_get_toggle(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.toggle_style);
}

int gui_skin_get_slider(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.slider_style);
}

int gui_skin_get_scrollbar(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.scrollbar_style);
}

int gui_skin_get_label(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.label_style);
}

int gui_skin_get_textbox(int id) {
  get_data_elementv(ski, Skin, GUI_TYPE::SKIN, id, -1);
  return (ski.textbox_style);
}
}  //namespace enigma_user
