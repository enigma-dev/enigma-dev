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

#include <new> //Placement new
#include <unordered_map>
using std::unordered_map;
using std::pair;

#include "sliders.h"
#include "buttons.h"
#include "toggles.h"
#include "groups.h"

#ifdef DEBUG_MODE
  #include <string>
  #include "libEGMstd.h"
  #include "Widget_Systems/widgets_mandatory.h"
  #define get_elementv(element,clastype,entype,id,ret)\
    if (gui::gui_elements.find(id) == gui::gui_elements.end() || gui::gui_elements[id].type != entype) {\
      show_error("Attempting to use non-existing element " + std::to_string(id), false);\
      return ret;\
    }\
    clastype &element = gui::gui_elements[id];
  #define get_element(element,type,entype,id) get_elementv(element,type,entype,id,)
#else
  #define get_elementv(element,clastype,entype,id,ret)\
    clastype &element = gui::gui_elements[id];
  #define get_element(element,type,entype,id) get_elementv(element,type,entype,id,)
#endif

namespace gui
{
  enum GUI_TYPE{
    ERROR = -1,
    BUTTON,
    TOGGLE,
    LABEL,
    SCROLLBAR,
    SLIDER,
    WINDOW,
    SKIN,
    STYLE,
    GROUP
  };

  class Element{
    public:
      GUI_TYPE type = GUI_TYPE::ERROR;
      unsigned int id = 0;

      //Unrestricted union is C++14
      union Data{
        Button button;
        Slider slider;
        Toggle toggle;
        Group group;

        Data() {}
        ~Data() {}
      };
      Data data;

      //Constructors
      inline Element(){ }

      inline Element(const Button &but){
        type = GUI_TYPE::BUTTON;
        new (&(data.button)) Button(but);
      }

      inline Element(const Slider &sli){
        type = GUI_TYPE::SLIDER;
        new (&(data.button)) Slider(sli);
      }

      inline Element(const Toggle &tog){
        type = GUI_TYPE::TOGGLE;
        new (&(data.toggle)) Toggle(tog);
      }

      inline Element(const Group &gro){
        type = GUI_TYPE::GROUP;
        new (&(data.group)) Group(gro);
      }

      //Destructor
      inline ~Element(){
        switch (type){
          case GUI_TYPE::BUTTON:
            data.button.~Button();
            break;
          case GUI_TYPE::SLIDER:
            data.slider.~Slider();
            break;
          case GUI_TYPE::TOGGLE:
            data.toggle.~Toggle();
            break;
          case GUI_TYPE::GROUP:
            data.group.~Group();
            break;
        }
      }

      //Accessor
      inline operator Button&(){
        if (type == GUI_TYPE::BUTTON){
          return data.button;
        }
        printf("Type is not a button! This is going to crash now!\n");
        return data.button;
      }

      inline operator Slider&(){
        if (type == GUI_TYPE::SLIDER){
          return data.slider;
        }
        printf("Type is not a slider! This is going to crash now!\n");
        return data.slider;
      }

      inline operator Toggle&(){
        if (type == GUI_TYPE::TOGGLE){
          return data.toggle;
        }
        printf("Type is not a toggle! This is going to crash now!\n");
        return data.toggle;
      }

      inline operator Group&(){
        if (type == GUI_TYPE::GROUP){
          return data.group;
        }
        printf("Type is not a group! This is going to crash now!\n");
        return data.group;
      }

      //This is only needed if we have non-trivial copy constructors, but I leave it here for now
      inline Element(const Element & rhs){
        type = rhs.type;
        switch(type){
          case GUI_TYPE::BUTTON:
              new (&(data.button)) Button(rhs.data.button);
              break;
          case GUI_TYPE::SLIDER:
              new (&(data.slider)) Slider(rhs.data.slider);
              break;
          case GUI_TYPE::TOGGLE:
              new (&(data.toggle)) Toggle(rhs.data.toggle);
              break;
          case GUI_TYPE::GROUP:
              new (&(data.group)) Group(rhs.data.group);
              break;
        }
      }
	};
}
#endif
