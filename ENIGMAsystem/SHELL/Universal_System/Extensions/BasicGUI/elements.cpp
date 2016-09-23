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
#include <map>
using std::unordered_map;
using std::map;

#include "elements.h"

namespace gui
{
  unsigned int gui_elements_maxid = 0;
  unsigned int gui_data_elements_maxid = 0;

  unordered_map<unsigned int, Element> gui_elements;
  unordered_map<unsigned int, DataElement> gui_data_elements;
  map<unsigned int, unsigned int> gui_element_order; //This allows changing rendering order (like depth)
}

namespace enigma_user
{
  int gui_element_get_type(int ele){
    check_element_existsv(ele,-1);
    return static_cast<int>(gui::gui_elements[ele].type);
  }

 ///Depth changers
 ///TODO(harijs) - this still needs to be worked on
	/*void gui_element_push_to_front(int id){
    check_element_exists(id);
    auto it = gui::gui_element_order.find(id);
    if (it != gui::gui_element_order.end()){
      gui::gui_element_order.erase(it);
      gui::gui_element_order.emplace(gui::gui_element_order.size(),id);
    }
	}

  void gui_element_push_to_back(int id){
    check_element_exists(id);
    auto it = gui::gui_element_order.find(id);
    if (it != gui::gui_element_order.end()){
      gui::gui_element_order.erase(it);
      gui::gui_element_order.emplace(0,id);
    }
	}*/
}
