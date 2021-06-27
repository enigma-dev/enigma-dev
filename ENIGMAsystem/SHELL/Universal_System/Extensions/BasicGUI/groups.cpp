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

#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Platforms/General/PFmain.h"                  //For mouse_check_button
#include "Universal_System/Resources/resource_data.h"  //For script_execute
#include "Universal_System/var4.h"

#include "buttons.h"
#include "elements.h"
#include "groups.h"
#include "toggles.h"

#include <algorithm>
#include <string>
#include <unordered_map>

using namespace enigma::gui;
using std::string;
using std::unordered_map;

namespace enigma_user {
int gui_group_create() {
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, Group());
  Group &g = guiElements.gui_data_elements[guiElements.gui_data_elements_maxid];
  g.id = guiElements.gui_data_elements_maxid;
  return guiElements.gui_data_elements_maxid++;
}

int gui_group_duplicate(int group) {
  get_data_elementv(gro, Group, GUI_TYPE::GROUP, group, -1);
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, gro);
  guiElements.gui_data_elements[guiElements.gui_data_elements_maxid].id = guiElements.gui_data_elements_maxid;
  return guiElements.gui_data_elements_maxid++;
}

void gui_group_destroy(int id) {
  check_data_element(GUI_TYPE::GROUP, id);
  guiElements.gui_data_elements.erase(guiElements.gui_data_elements.find(id));
}

void gui_group_add_button(int id, int bid) {
  get_element(but, Button, GUI_TYPE::BUTTON, bid);
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  gro.group_buttons.push_back(bid);
  but.group_id = id;
}

void gui_group_add_toggle(int id, int tid) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, tid);
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  gro.group_toggles.push_back(tid);
  tog.group_id = id;
}

void gui_group_add_window(int id, int wid) {
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  gro.group_windows.push_back(wid);
}

void gui_group_remove_button(int id, int bid) {
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  auto it = find(gro.group_buttons.begin(), gro.group_buttons.end(), bid);
  if (it != gro.group_buttons.end()) {
    get_element(but, Button, GUI_TYPE::BUTTON, bid);
    gro.group_buttons.erase(it);
    but.parent_id = -1;
  }
}

void gui_group_remove_toggle(int id, int tid) {
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  auto it = find(gro.group_toggles.begin(), gro.group_toggles.end(), tid);
  if (it != gro.group_toggles.end()) {
    get_element(tog, Toggle, GUI_TYPE::TOGGLE, tid);
    gro.group_toggles.erase(it);
    tog.parent_id = -1;
  }
}

void gui_group_remove_window(int id, int wid) {
  get_data_element(gro, Group, GUI_TYPE::GROUP, id);
  auto it = find(gro.group_windows.begin(), gro.group_windows.end(), wid);
  if (it != gro.group_windows.end()) {
    get_element(win, Window, GUI_TYPE::WINDOW, wid);
    gro.group_windows.erase(it);
    win.parent_id = -1;
  }
}
}  // namespace enigma_user
