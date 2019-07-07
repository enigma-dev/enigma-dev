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
#include "Platforms/General/PFwindow.h"                //For mouse_check_button
#include "Universal_System/Resources/resource_data.h"  //For script_execute
#include "Universal_System/var4.h"

#include "buttons.h"
#include "common.h"
#include "elements.h"
#include "groups.h"
#include "include.h"
#include "skins.h"
#include "styles.h"
#include "toggles.h"

#include <string>
#include <unordered_map>

using namespace enigma::gui;
using std::string;
using std::unordered_map;

namespace enigma {
namespace gui {
//Implements toggle class
Toggle::Toggle() {
  style_id = guiElements.gui_style_toggle;  //Default style
  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
  enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
  callback.fill(-1);  //Default callbacks don't exist (so it doesn't call any script)
}

void Toggle::callback_execute(int event) {
  if (callback[event] != -1) {
    enigma_user::script_execute(callback[event], id, active, state, event);
  }
}

//Update all possible toggle states (hover, click, toggle etc.)
void Toggle::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty) {
  if (box.point_inside(tx - ox, ty - oy) && windowStopPropagation == false) {
    callback_execute(enigma_user::gui_event_hover);
    windowStopPropagation = true;
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) {
      if (active == false) {
        state = enigma_user::gui_state_active;
      } else {
        state = enigma_user::gui_state_on_active;
      }
      callback_execute(enigma_user::gui_event_pressed);
    } else {
      if (state != enigma_user::gui_state_active && state != enigma_user::gui_state_on_active) {
        if (active == false) {
          state = enigma_user::gui_state_hover;
        } else {
          state = enigma_user::gui_state_on_hover;
        }
      } else {
        if (enigma_user::mouse_check_button_released(enigma_user::mb_left)) {
          if (group_id !=
              -1) {  //Toggles in groups cannot be disabled by user clicking again. At least one toggle must be active
            active = true;
          } else {
            active = !active;
          }

          callback_execute(enigma_user::gui_event_released);

          if (active == false) {
            state = enigma_user::gui_state_hover;
          } else {
            state = enigma_user::gui_state_on_hover;
            if (group_id != -1) {  //Groups disable any other active element
              get_data_element(gro, Group, GUI_TYPE::GROUP, group_id);
              for (const auto &b : gro.group_buttons) {
                get_element(but, Button, GUI_TYPE::BUTTON, b);
                but.active = false;
              }
              for (const auto &t : gro.group_toggles) {
                if (t != id) {
                  get_element(tog, Toggle, GUI_TYPE::TOGGLE, t);
                  tog.active = false;
                }
              }
            }
          }
        }
      }
    }
  } else {
    if (active == false) {
      state = enigma_user::gui_state_default;
    } else {
      state = enigma_user::gui_state_on;
    }
  }
}

void Toggle::draw(gs_scalar ox, gs_scalar oy) {
  //Draw toggle
  get_data_element(sty, Style, GUI_TYPE::STYLE, style_id);
  if (sty.sprites[state] != -1) {
    if (sty.border.zero == true) {
      enigma_user::draw_sprite_stretched(sty.sprites[state], -1, ox + box.x, oy + box.y, box.w, box.h,
                                         sty.sprite_styles[state].color, sty.sprite_styles[state].alpha);
    } else {
      enigma_user::draw_sprite_padded(sty.sprites[state], -1, sty.border.left, sty.border.top, sty.border.right,
                                      sty.border.bottom, ox + box.x, oy + box.y, ox + box.x + box.w, oy + box.y + box.h,
                                      sty.sprite_styles[state].color, sty.sprite_styles[state].alpha);
    }
  }

  //Draw text
  if (text.empty() == false) {
    sty.font_styles[state].use();

    gs_scalar textx = 0.0, texty = 0.0;
    switch (sty.font_styles[state].halign) {
      case enigma_user::fa_left:
        textx = box.x + sty.padding.left;
        break;
      case enigma_user::fa_center:
        textx = box.x + box.w / 2.0;
        break;
      case enigma_user::fa_right:
        textx = box.x + box.w - sty.padding.right;
        break;
    }

    switch (sty.font_styles[state].valign) {
      case enigma_user::fa_top:
        texty = box.y + sty.padding.top;
        break;
      case enigma_user::fa_middle:
        texty = box.y + box.h / 2.0;
        break;
      case enigma_user::fa_bottom:
        texty = box.y + box.h - sty.padding.bottom;
        break;
    }

    enigma_user::draw_text(ox + textx, oy + texty, text);
  }
}

void Toggle::update_text_pos(int state) {}
}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_toggle_create() {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Toggle(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, ski.toggle_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(tog, guiElements.gui_elements_maxid));
  }
  Toggle &t = guiElements.gui_elements[guiElements.gui_elements_maxid];
  t.visible = true;
  t.id = guiElements.gui_elements_maxid;
  return (guiElements.gui_elements_maxid++);
}

int gui_toggle_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text) {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Toggle(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, ski.toggle_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(tog, guiElements.gui_elements_maxid));
  }
  Toggle &t = guiElements.gui_elements[guiElements.gui_elements_maxid];
  t.visible = true;
  t.id = guiElements.gui_elements_maxid;
  t.box.set(x, y, w, h);
  t.text = text;
  t.update_text_pos();
  return (guiElements.gui_elements_maxid++);
}

int gui_toggle_duplicate(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                   std::forward_as_tuple(tog, guiElements.gui_elements_maxid));
  guiElements.gui_elements[guiElements.gui_elements_maxid].id = guiElements.gui_elements_maxid;
  Toggle &t = guiElements.gui_elements[guiElements.gui_elements_maxid];
  t.id = guiElements.gui_elements_maxid;
  t.parent_id = -1;  //We cannot duplicate parenting for now
  return guiElements.gui_elements_maxid++;
}

void gui_toggle_destroy(int id) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  if (tog.parent_id != -1) {
    gui_window_remove_toggle(tog.parent_id, id);
  }
  guiElements.gui_elements.erase(guiElements.gui_elements.find(id));
}

///Setters
void gui_toggle_set_text(int id, string text) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  tog.text = text;
}

void gui_toggle_set_position(int id, gs_scalar x, gs_scalar y) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  tog.box.x = x;
  tog.box.y = y;
}

void gui_toggle_set_size(int id, gs_scalar w, gs_scalar h) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  tog.box.w = w;
  tog.box.h = h;
  tog.update_text_pos();
}

void gui_toggle_set_callback(int id, int event, int script_id) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  if (event == enigma_user::gui_event_all) {
    tog.callback.fill(script_id);
  } else {
    tog.callback[event] = script_id;
  }
}

void gui_toggle_set_style(int id, int style_id) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  check_data_element(GUI_TYPE::STYLE, style_id);
  tog.style_id = (style_id != -1 ? style_id : guiElements.gui_style_toggle);
}

void gui_toggle_set_active(int id, bool active) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  tog.active = active;
}

void gui_toggle_set_visible(int id, bool visible) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  tog.visible = visible;
}

///Getters
int gui_toggle_get_style(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.style_id;
}

int gui_toggle_get_state(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.state;
}

int gui_toggle_get_callback(int id, int event) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.callback[event];
}

int gui_toggle_get_parent(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.parent_id;
}

bool gui_toggle_get_active(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, false);
  return tog.active;
}

bool gui_toggle_get_visible(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, false);
  return tog.visible;
}

gs_scalar gui_toggle_get_width(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.box.w;
}

gs_scalar gui_toggle_get_height(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.box.h;
}

gs_scalar gui_toggle_get_x(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.box.x;
}

gs_scalar gui_toggle_get_y(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, -1);
  return tog.box.y;
}

string gui_toggle_get_text(int id) {
  get_elementv(tog, Toggle, GUI_TYPE::TOGGLE, id, "");
  return tog.text;
}

///Drawers
void gui_toggle_draw(int id) {
  get_element(tog, Toggle, GUI_TYPE::TOGGLE, id);
  int pfont = enigma_user::draw_get_font();
  unsigned int phalign = enigma_user::draw_get_halign();
  unsigned int pvalign = enigma_user::draw_get_valign();
  int pcolor = enigma_user::draw_get_color();
  gs_scalar palpha = enigma_user::draw_get_alpha();
  tog.update();
  tog.draw();
  enigma_user::draw_set_halign(phalign);
  enigma_user::draw_set_valign(pvalign);
  enigma_user::draw_set_color(pcolor);
  enigma_user::draw_set_alpha(palpha);
  enigma_user::draw_set_font(pfont);
}

void gui_toggles_draw() {
  int pfont = enigma_user::draw_get_font();
  unsigned int phalign = enigma_user::draw_get_halign();
  unsigned int pvalign = enigma_user::draw_get_valign();
  int pcolor = enigma_user::draw_get_color();
  gs_scalar palpha = enigma_user::draw_get_alpha();
  for (auto &b : guiElements.gui_elements) {
    ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
    if (b.second.type == GUI_TYPE::TOGGLE) {
      get_element(tog, Toggle, GUI_TYPE::TOGGLE, b.first);
      if (tog.visible == true && tog.parent_id == -1) {
        tog.update();
        tog.draw();
      }
    }
  }
  enigma_user::draw_set_halign(phalign);
  enigma_user::draw_set_valign(pvalign);
  enigma_user::draw_set_color(pcolor);
  enigma_user::draw_set_alpha(palpha);
  enigma_user::draw_set_font(pfont);
}
}  //namespace enigma_user
