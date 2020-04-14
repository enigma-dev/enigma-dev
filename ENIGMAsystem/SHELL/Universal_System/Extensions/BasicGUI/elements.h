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

#ifndef BGUI_ELEMENTS_H
#define BGUI_ELEMENTS_H

#include "Widget_Systems/widgets_mandatory.h"
#include "buttons.h"
#include "groups.h"
#include "labels.h"
#include "scrollbars.h"
#include "skins.h"
#include "sliders.h"
#include "styles.h"
#include "textboxes.h"
#include "toggles.h"
#include "windows.h"

#include <map>
#include <new>    //Placement new
#include <tuple>  // std::forward_as_tuple
#include <unordered_map>
#include <utility>  // std::pair, std::piecewise_construct

#ifdef DEBUG_MODE
#include <string>
#include "libEGMstd.h"
//This checks and returns an element
#define get_elementv(element, clastype, entype, id, ret)                                                       \
  if (guiElements.gui_elements.find(id) == guiElements.gui_elements.end() ||                                   \
      guiElements.gui_elements[id].type != entype) {                                                           \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }                                                                                                            \
  clastype &element = guiElements.gui_elements[id];
#define get_element(element, type, entype, id) get_elementv(element, type, entype, id, )

//This checks and returns an element but uses the type already assigned
/*#define get_element_smartv(element,id,ret)\
    if (guiElements.gui_elements.find(id) == guiElements.gui_elements.end()) {\
      DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR);\
      return ret;\
    }\
    switch (guiElements.gui_elements[id].type){ \
      case GUI_TYPE::BUTTON: Button &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::TOGGLE: Toggle &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::LABEL:  Label &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::SCROLLBAR: Scrollbar &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::SLIDER: Slider &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::WINDOW: Window &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::SKIN: Skin &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::STYLE: Style &element = guiElements.gui_elements[id]; break; \
      case GUI_TYPE::GROUP: Group &element = guiElements.gui_elements[id]; break; \
      default: DEBUG_MESSAGE("Cannot determine type of element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); return ret; \
    }
  #define get_element_smart(element,id) get_element_smartv(element,id,)*/

//This only checks an element if it exists
#define check_elementv(entype, id, ret)                                                                        \
  if (guiElements.gui_elements.find(id) == guiElements.gui_elements.end() ||                                   \
      guiElements.gui_elements[id].type != entype) {                                                           \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }
#define check_element(entype, id) check_elementv(entype, id, )

//This only checks if an element id exists (so it doesn't care about type)
#define check_element_existsv(id, ret)                                                                         \
  if (guiElements.gui_elements.find(id) == guiElements.gui_elements.end()) {                                   \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }
#define check_element_exists(id) check_element_existsv(id, )

///THIS IS FOR DATA ELEMENTS
//This checks and returns an element
#define get_data_elementv(element, clastype, entype, id, ret)                                                  \
  if (guiElements.gui_data_elements.find(id) == guiElements.gui_data_elements.end() ||                         \
      guiElements.gui_data_elements[id].type != entype) {                                                      \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }                                                                                                            \
  clastype &element = guiElements.gui_data_elements[id];
#define get_data_element(element, type, entype, id) get_data_elementv(element, type, entype, id, )

//This only checks an element if it exists
#define check_data_elementv(entype, id, ret)                                                                   \
  if (guiElements.gui_data_elements.find(id) == guiElements.gui_data_elements.end() ||                         \
      guiElements.gui_data_elements[id].type != entype) {                                                      \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }
#define check_data_element(entype, id) check_data_elementv(entype, id, )

//This only checks if an element id exists (so it doesn't care about type)
#define check_data_element_existsv(id, ret)                                                                    \
  if (guiElements.gui_data_elements.find(id) == guiElements.gui_data_elements.end()) {                         \
    DEBUG_MESSAGE("Attempting to use non-existing element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); \
    return ret;                                                                                                \
  }
#define check_data_element_exists(id) check_data_element_existsv(id, )
#else
#define get_elementv(element, clastype, entype, id, ret) clastype &element = guiElements.gui_elements[id];
#define get_element(element, type, entype, id) get_elementv(element, type, entype, id, )
#define check_elementv(entype, id, ret)
#define check_element(entype, id) check_elementv(entype, id, )
#define check_element_existsv(id, ret)
#define check_element_exists(id) check_element_existsv(id, )
/*#define get_element_smartv(element,id,ret) \
    Button *b, Toggle *t, Label *l, Scrollbar *s, Slider *sl, Window *w, Skin *sk, Style *st, Group *g; \
    switch (guiElements.gui_elements[id].type){ \
      case GUI_TYPE::BUTTON:    element = Button element; break; \
      case GUI_TYPE::TOGGLE:    Toggle element; break; \
      case GUI_TYPE::LABEL:     Label element; break; \
      case GUI_TYPE::SCROLLBAR: Scrollbar element; break; \
      case GUI_TYPE::SLIDER:    Slider element; break; \
      case GUI_TYPE::WINDOW:    Window element; break; \
      case GUI_TYPE::SKIN:      Skin element; break; \
      case GUI_TYPE::STYLE:     Style element; break; \
      case GUI_TYPE::GROUP:     Group element; break; \
      default: Button element = Button(); DEBUG_MESSAGE("Cannot determine type of element " + std::to_string(id), MESSAGE_TYPE::M_USER_ERROR); return ret; \
    } \
    auto element = &b;
  #define get_element_smart(element,id) get_element_smartv(element,id,)*/
#define get_data_elementv(element, clastype, entype, id, ret) clastype &element = guiElements.gui_data_elements[id];
#define get_data_element(element, type, entype, id) get_data_elementv(element, type, entype, id, )
#define check_data_elementv(entype, id, ret)
#define check_data_element(entype, id) check_data_elementv(entype, id, )
#define check_data_element_existsv(id, ret)
#define check_data_element_exists(id) check_data_element_existsv(id, )
#endif

namespace enigma {
namespace gui {
#undef ERROR  //Windows has ERROR of its own
enum GUI_TYPE { ERROR = -1, BUTTON, TOGGLE, LABEL, SCROLLBAR, SLIDER, WINDOW, SKIN, STYLE, GROUP, TEXTBOX };

class Element {
 public:
  GUI_TYPE type = GUI_TYPE::ERROR;
  unsigned int id = 0;

  //Unrestricted union is C++14
  union Data {
    Button button;
    Slider slider;
    Toggle toggle;
    Label label;
    Scrollbar scrollbar;
    Window window;
    Textbox textbox;

    Data() {}
    ~Data() {}
  };
  Data data;

  //Constructors
  inline Element() {}

  inline Element(const Button &but, int id) : id(id) {
    type = GUI_TYPE::BUTTON;
    new (&(data.button)) Button(but);
  }

  inline Element(const Slider &sli, int id) : id(id) {
    type = GUI_TYPE::SLIDER;
    new (&(data.slider)) Slider(sli);
  }

  inline Element(const Toggle &tog, int id) : id(id) {
    type = GUI_TYPE::TOGGLE;
    new (&(data.toggle)) Toggle(tog);
  }

  inline Element(const Label &lab, int id) : id(id) {
    type = GUI_TYPE::LABEL;
    new (&(data.label)) Label(lab);
  }

  inline Element(const Scrollbar &scr, int id) : id(id) {
    type = GUI_TYPE::SCROLLBAR;
    new (&(data.scrollbar)) Scrollbar(scr);
  }

  inline Element(const Window &win, int id) : id(id) {
    type = GUI_TYPE::WINDOW;
    new (&(data.window)) Window(win);
  }

  inline Element(const Textbox &tex, int id) : id(id) {
    type = GUI_TYPE::TEXTBOX;
    new (&(data.textbox)) Textbox(tex);
  }

  //Destructor
  inline ~Element() {
    switch (type) {
      case GUI_TYPE::BUTTON:
        data.button.~Button();
        break;
      case GUI_TYPE::SLIDER:
        data.slider.~Slider();
        break;
      case GUI_TYPE::TOGGLE:
        data.toggle.~Toggle();
        break;
      case GUI_TYPE::LABEL:
        data.label.~Label();
        break;
      case GUI_TYPE::SCROLLBAR:
        data.scrollbar.~Scrollbar();
        break;
      case GUI_TYPE::WINDOW:
        data.window.~Window();
        break;
      case GUI_TYPE::TEXTBOX:
        data.textbox.~Textbox();
        break;
      default:
        DEBUG_MESSAGE("BasicGUI: Unknown element type or element type == ERROR and id = " + std::to_string(id) + "!",
                      MESSAGE_TYPE::M_USER_ERROR);
        break;
    }
  }

  //Accessor
  inline operator Button &() {
    if (type == GUI_TYPE::BUTTON) {
      return data.button;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a button! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.button;
  }

  inline operator Slider &() {
    if (type == GUI_TYPE::SLIDER) {
      return data.slider;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a slider! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.slider;
  }

  inline operator Toggle &() {
    if (type == GUI_TYPE::TOGGLE) {
      return data.toggle;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a toggle! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.toggle;
  }

  inline operator Label &() {
    if (type == GUI_TYPE::LABEL) {
      return data.label;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a label! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.label;
  }

  inline operator Scrollbar &() {
    if (type == GUI_TYPE::SCROLLBAR) {
      return data.scrollbar;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a scrollbar! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.scrollbar;
  }

  inline operator Window &() {
    if (type == GUI_TYPE::WINDOW) {
      return data.window;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a window! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.window;
  }

  inline operator Textbox &() {
    if (type == GUI_TYPE::TEXTBOX) {
      return data.textbox;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a textbox! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.textbox;
  }

  //This is only needed if we have non-trivial copy constructors, but I leave it here for now
  inline Element(const Element &rhs) {
    type = rhs.type;
    switch (type) {
      case GUI_TYPE::BUTTON:
        new (&(data.button)) Button(rhs.data.button);
        break;
      case GUI_TYPE::SLIDER:
        new (&(data.slider)) Slider(rhs.data.slider);
        break;
      case GUI_TYPE::TOGGLE:
        new (&(data.toggle)) Toggle(rhs.data.toggle);
        break;
      case GUI_TYPE::LABEL:
        new (&(data.label)) Label(rhs.data.label);
        break;
      case GUI_TYPE::SCROLLBAR:
        new (&(data.scrollbar)) Scrollbar(rhs.data.scrollbar);
        break;
      case GUI_TYPE::WINDOW:
        new (&(data.window)) Window(rhs.data.window);
        break;
      case GUI_TYPE::TEXTBOX:
        new (&(data.textbox)) Textbox(rhs.data.textbox);
        break;
      default:
        DEBUG_MESSAGE("BasicGUI: Unknown element type or element type == ERROR!", MESSAGE_TYPE::M_USER_ERROR);
        break;
    }
  }
};

///These are data elements, like skins, styles and groups. They are separate from widgets, so we can iterate faster. This should also bring down memory consumption
class DataElement {
 public:
  GUI_TYPE type = GUI_TYPE::ERROR;
  unsigned int id = 0;

  //Unrestricted union is C++14
  union Data {
    Group group;
    Style style;
    Skin skin;

    Data() {}
    ~Data() {}
  };
  Data data;

  //Constructors
  inline DataElement() {}

  inline DataElement(const Group &gro) {
    type = GUI_TYPE::GROUP;
    new (&(data.group)) Group(gro);
  }

  inline DataElement(const Style &sty) {
    type = GUI_TYPE::STYLE;
    new (&(data.style)) Style(sty);
  }

  inline DataElement(const Skin &ski) {
    type = GUI_TYPE::SKIN;
    new (&(data.skin)) Skin(ski);
  }

  //Destructor
  inline ~DataElement() {
    switch (type) {
      case GUI_TYPE::GROUP:
        data.group.~Group();
        break;
      case GUI_TYPE::STYLE:
        data.style.~Style();
        break;
      case GUI_TYPE::SKIN:
        data.skin.~Skin();
        break;
      default:
        DEBUG_MESSAGE("BasicGUI: Unknown element type or element type == ERROR!", MESSAGE_TYPE::M_FATAL_ERROR);
        break;
    }
  }

  //Accessor
  inline operator Group &() {
    if (type == GUI_TYPE::GROUP) {
      return data.group;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a group! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.group;
  }

  inline operator Style &() {
    if (type == GUI_TYPE::STYLE) {
      return data.style;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a style! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.style;
  }

  inline operator Skin &() {
    if (type == GUI_TYPE::SKIN) {
      return data.skin;
    }
    DEBUG_MESSAGE("BasicGUI: Type is not a skin! This is going to crash now!", MESSAGE_TYPE::M_FATAL_ERROR);
    return data.skin;
  }

  //This is only needed if we have non-trivial copy constructors, but I leave it here for now
  inline DataElement(const DataElement &rhs) {
    type = rhs.type;
    switch (type) {
      case GUI_TYPE::GROUP:
        new (&(data.group)) Group(rhs.data.group);
        break;
      case GUI_TYPE::STYLE:
        new (&(data.style)) Style(rhs.data.style);
        break;
      case GUI_TYPE::SKIN:
        new (&(data.skin)) Skin(rhs.data.skin);
        break;
      default:
        DEBUG_MESSAGE("BasicGUI: Unknown element type or element type == ERROR!", MESSAGE_TYPE::M_USER_ERROR);
        break;
    }
  }
};

struct GUIElements {
  GUIElements();
  unsigned int gui_elements_maxid = 0;
  unsigned int gui_data_elements_maxid = 0;
  std::unordered_map<unsigned int, Element> gui_elements;
  std::unordered_map<unsigned int, DataElement> gui_data_elements;
  std::map<unsigned int, unsigned int> gui_element_order;  //This allows changing rendering order (like depth)
  unsigned int gui_style_button;
  unsigned int gui_style_toggle;
  unsigned int gui_style_window;
  unsigned int gui_style_slider;
  unsigned int gui_style_label;
  unsigned int gui_style_scrollbar;
  unsigned int gui_style_textbox;
};

extern GUIElements guiElements;
extern bool windowStopPropagation;
extern int gui_bound_skin;

}  //namespace gui
}  //namespace enigma

#endif
