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

#include "elements.h"
#include "include.h"

#include <map>
#include <unordered_map>

using namespace enigma::gui;
using std::map;
using std::unordered_map;

namespace enigma {
namespace gui {
GUIElements::GUIElements()
    : gui_style_button(enigma_user::gui_style_create()),
      gui_style_toggle(enigma_user::gui_style_create()),
      gui_style_window(enigma_user::gui_style_create()),
      gui_style_slider(enigma_user::gui_style_create()),
      gui_style_label(enigma_user::gui_style_create()),
      gui_style_scrollbar(enigma_user::gui_style_create()),
      gui_style_textbox(enigma_user::gui_style_create()) {}
GUIElements guiElements;
}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_element_get_type(int ele) {
  check_element_existsv(ele, -1);
  return static_cast<int>(guiElements.gui_elements[ele].type);
}

///Depth changers
///TODO(harijs) - this still needs to be worked on
/*void gui_element_push_to_front(int id){
    check_element_exists(id);
    auto it = guiElements.gui_element_order.find(id);
    if (it != guiElements.gui_element_order.end()){
      guiElements.gui_element_order.erase(it);
      guiElements.gui_element_order.emplace(guiElements.gui_element_order.size(),id);
    }
	}

  void gui_element_push_to_back(int id){
    check_element_exists(id);
    auto it = guiElements.gui_element_order.find(id);
    if (it != guiElements.gui_element_order.end()){
      guiElements.gui_element_order.erase(it);
      guiElements.gui_element_order.emplace(0,id);
    }
	}*/
}  //namespace enigma_user
