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

#include "parents.h"
#include "elements.h"

#include <algorithm>  //For std::find
#include <string>

using namespace enigma::gui;

namespace enigma {
namespace gui {

void Parent::update_children(gs_scalar ox, gs_scalar oy) {
  //Update children
  for (const auto &x : child_elements) {
    //get_element_smart(ele, x);
    switch (guiElements.gui_elements[x].type) {
      case GUI_TYPE::BUTTON: {
        Button &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::TEXTBOX: {
        Textbox &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::TOGGLE: {
        Toggle &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::SCROLLBAR: {
        Scrollbar &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::SLIDER: {
        Slider &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::WINDOW: {
        Window &element = guiElements.gui_elements[x];
        if (element.visible == true) element.update(ox, oy);
        break;
      }
      case GUI_TYPE::LABEL:
      case GUI_TYPE::SKIN:
      case GUI_TYPE::STYLE:
      case GUI_TYPE::GROUP:
      case GUI_TYPE::ERROR: {
        break;
      }
    }
  }
}

void Parent::draw_children(gs_scalar ox, gs_scalar oy) {
  //Draw all children
  for (const auto &x : child_elements) {
    switch (guiElements.gui_elements[x].type) {
      case GUI_TYPE::BUTTON: {
        Button &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::TEXTBOX: {
        Textbox &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::TOGGLE: {
        Toggle &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::LABEL: {
        Label &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::SCROLLBAR: {
        Scrollbar &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::SLIDER: {
        Slider &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::WINDOW: {
        Window &element = guiElements.gui_elements[x];
        if (element.visible == true) element.draw(ox, oy);
        break;
      }
      case GUI_TYPE::SKIN:
      case GUI_TYPE::STYLE:
      case GUI_TYPE::GROUP:
      case GUI_TYPE::ERROR: {
        break;
      }
    }
  }
}

void Parent::button_add(int id) {
  get_element(but, Button, GUI_TYPE::BUTTON, id);
  child_elements.push_back(id);
  but.parent_id = element_id;
}

void Parent::toggle_add(int id) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  child_elements.push_back(id);
  tog.parent_id = element_id;
}

void Parent::scrollbar_add(int id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  child_elements.push_back(id);
  scr.parent_id = element_id;
}

void Parent::slider_add(int id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  child_elements.push_back(id);
  sli.parent_id = element_id;
}

void Parent::window_add(int id) {
  get_element(win, Window, GUI_TYPE::WINDOW, id);
  child_elements.push_back(id);
  win.parent_id = element_id;
}

void Parent::label_add(int id) {
  get_element(lab, Label, GUI_TYPE::LABEL, id);
  child_elements.push_back(id);
  lab.parent_id = element_id;
}

void Parent::textbox_add(int id) {
  get_element(tex, Textbox, GUI_TYPE::TEXTBOX, id);
  if (tex.parent_id != -1) {
    DEBUG_MESSAGE("BasicGUI: Textbox with ID " + std::to_string(id) + " already has a parent with ID " +
                      std::to_string(tex.parent_id) + "! Parenting not done!",
                  MESSAGE_TYPE::M_ERROR);
  }
  child_elements.push_back(id);
  tex.parent_id = element_id;
}

void Parent::button_remove(int id) {
  check_element(GUI_TYPE::BUTTON, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(but, Button, GUI_TYPE::BUTTON, id);
    child_elements.erase(it);
    but.parent_id = -1;
  }
}

void Parent::toggle_remove(int id) {
  check_element(GUI_TYPE::TOGGLE, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
    child_elements.erase(it);
    tog.parent_id = -1;
  }
}

void Parent::scrollbar_remove(int id) {
  check_element(GUI_TYPE::SCROLLBAR, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
    child_elements.erase(it);
    scr.parent_id = -1;
  }
}

void Parent::slider_remove(int id) {
  check_element(GUI_TYPE::SLIDER, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(sli, Slider, GUI_TYPE::SLIDER, id);
    child_elements.erase(it);
    sli.parent_id = -1;
  }
}

void Parent::window_remove(int id) {
  check_element(GUI_TYPE::WINDOW, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(win, Window, GUI_TYPE::WINDOW, id);
    child_elements.erase(it);
    win.parent_id = -1;
  }
}

void Parent::label_remove(int id) {
  check_element(GUI_TYPE::LABEL, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(lab, Label, GUI_TYPE::LABEL, id);
    child_elements.erase(it);
    lab.parent_id = -1;
  }
}

void Parent::textbox_remove(int id) {
  check_element(GUI_TYPE::TEXTBOX, id);
  auto it = find(child_elements.begin(), child_elements.end(), id);
  if (it != child_elements.end()) {
    get_element(tex, Textbox, GUI_TYPE::TEXTBOX, id);
    child_elements.erase(it);
    tex.parent_id = -1;
  }
}

int Parent::button_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::BUTTON) c++;
  }
  return c;
}

int Parent::toggle_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::TOGGLE) c++;
  }
  return c;
}

int Parent::scrollbar_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::SCROLLBAR) c++;
  }
  return c;
}

int Parent::slider_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::SLIDER) c++;
  }
  return c;
}

int Parent::window_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::WINDOW) c++;
  }
  return c;
}

int Parent::label_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::LABEL) c++;
  }
  return c;
}

int Parent::textbox_count() {
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::TEXTBOX) c++;
  }
  return c;
}

int Parent::button(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::BUTTON) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::toggle(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::TOGGLE) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::scrollbar(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::SCROLLBAR) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::slider(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::SLIDER) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::window(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::WINDOW) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::label(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::LABEL) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::textbox(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  int c = 0;
  for (const unsigned int &e : child_elements) {
    check_element_existsv(e, -1);
    if (guiElements.gui_elements[e].type == GUI_TYPE::TEXTBOX) {
      if (c == id) {
        return e;
      }
      c++;
    }
  }
  return -1;
}

int Parent::element_count() { return child_elements.size(); }

int Parent::element(int id) {
  if (id < 0 || unsigned(id) >= child_elements.size()) return -1;
  check_element_existsv(child_elements[id], -1);
  return child_elements[id];
}
}  //namespace gui
}  //namespace enigma
