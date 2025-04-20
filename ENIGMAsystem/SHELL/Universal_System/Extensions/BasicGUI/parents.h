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

#ifndef BGUI_PARENT_H
#define BGUI_PARENT_H

#include "common.h"  //For gs_scalar

#include <vector>

namespace enigma {
namespace gui {
class Parent {
 public:
  int element_id = -1;  //The element id for which this is a parenter

  std::vector<unsigned int> child_elements;

  void update_children(gs_scalar ox, gs_scalar oy);
  void draw_children(gs_scalar ox, gs_scalar oy);

  void button_add(int id);
  void toggle_add(int id);
  void scrollbar_add(int id);
  void slider_add(int id);
  void window_add(int id);
  void label_add(int id);
  void textbox_add(int id);

  void button_remove(int id);
  void toggle_remove(int id);
  void scrollbar_remove(int id);
  void slider_remove(int id);
  void window_remove(int id);
  void label_remove(int id);
  void textbox_remove(int id);

  int button_count();
  int toggle_count();
  int scrollbar_count();
  int slider_count();
  int window_count();
  int label_count();
  int textbox_count();

  int button(int id);
  int toggle(int id);
  int scrollbar(int id);
  int slider(int id);
  int window(int id);
  int label(int id);
  int textbox(int id);

  int element_count();
  int element(int id);
};
}  //namespace gui
}  //namespace enigma

#endif
