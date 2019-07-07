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
#include "Platforms/General/PFwindow.h"                //For mouse_check_button
#include "Universal_System/Resources/resource_data.h"  //For script_execute
#include "Universal_System/var4.h"

#include "common.h"
#include "elements.h"
#include "include.h"
#include "scrollbars.h"
#include "skins.h"
#include "styles.h"

#include <string>
#include <unordered_map>

using namespace enigma::gui;
using std::string;
using std::unordered_map;

namespace enigma {
namespace gui {

//Implements scrollbar class
Scrollbar::Scrollbar() {
  style_id = guiElements.gui_style_scrollbar;  //Default style
  callback.fill(-1);                           //Default callbacks don't exist (so it doesn't call any script)
}

void Scrollbar::callback_execute(int event) {
  if (callback[event] != -1) {
    enigma_user::script_execute(callback[event], id, active, state, event);
  }
}

void Scrollbar::update_spos() {
  if (direction == false) {
    if (indicator_box.w != box.w) {
      scroll_offset = fmin(scroll_offset, box.w - indicator_box.w);
      value =
          round((minValue + scroll_offset / (box.w - indicator_box.w) * (maxValue - minValue)) / incValue) * incValue;
      scroll_offset = (box.w - indicator_box.w) * ((value - minValue) / (maxValue - minValue));
    }
  } else {
    if (indicator_box.h != box.h) {
      scroll_offset = fmin(scroll_offset, box.h - indicator_box.h);
      value =
          round((minValue + scroll_offset / (box.h - indicator_box.h) * (maxValue - minValue)) / incValue) * incValue;
      scroll_offset = (box.h - indicator_box.h) * ((value - minValue) / (maxValue - minValue));
    }
  }
}

//Update all possible scrollbar states (hover, click etc.)
void Scrollbar::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty) {
  if ((box.point_inside(tx - ox, ty - oy) ||
       indicator_box.point_inside(tx - ox - box.x - scroll_offset - indicator_box.x,
                                  ty - box.y - oy - indicator_box.y)) &&
      windowStopPropagation == false) {
    callback_execute(enigma_user::gui_event_hover);
    windowStopPropagation = true;
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) {
      state = enigma_user::gui_state_active;
      active = true;
      drag = true;

      if (direction == false) {
        if (indicator_box.point_inside(tx - ox - box.x - indicator_box.x - scroll_offset,
                                       ty - oy - box.y - indicator_box.y)) {  //We press on the indicator itself
          drag_offset = tx - ox - indicator_box.x - box.x - scroll_offset;
        } else {                                //We press on the whole widget box, so the indicator must jump
          drag_offset = indicator_box.w / 2.0;  //Center it on mouse otherwise
        }
      } else {
        if (indicator_box.point_inside(
                tx - ox - box.x - indicator_box.x,
                ty - oy - box.y - indicator_box.y - scroll_offset)) {  //We press on the indicator itself
          drag_offset = ty - oy - indicator_box.y - box.y - scroll_offset;
        } else {                                //We press on the whole widget box, so the indicator must jump
          drag_offset = indicator_box.h / 2.0;  //Center it on mouse otherwise
        }
      }
      callback_execute(enigma_user::gui_event_pressed);
    } else {
      if (state != enigma_user::gui_state_active) {
        state = enigma_user::gui_state_hover;
      } else {
        if (enigma_user::mouse_check_button_released(enigma_user::mb_left)) {
          active = false;
          state = enigma_user::gui_state_hover;
        }
      }
    }
  } else {
    state = enigma_user::gui_state_default;
  }
  if (drag == true) {
    windowStopPropagation = true;

    if (direction == false) {
      if (indicator_box.w != box.w) {
        scroll_offset = fmin(fmax(0, tx - box.x - ox - drag_offset), box.w - indicator_box.w);
        value =
            round((minValue + scroll_offset / (box.w - indicator_box.w) * (maxValue - minValue)) / incValue) * incValue;
        scroll_offset = (box.w - indicator_box.w) * ((value - minValue) / (maxValue - minValue));
      } else {
        value = 1.0;
      }
    } else {
      if (indicator_box.h != box.h) {
        scroll_offset = fmin(fmax(0, ty - box.y - oy - drag_offset), box.h - indicator_box.h);
        value =
            round((minValue + scroll_offset / (box.h - indicator_box.h) * (maxValue - minValue)) / incValue) * incValue;
        scroll_offset = (box.h - indicator_box.h) * ((value - minValue) / (maxValue - minValue));
      } else {
        value = 1.0;
      }
    }
    callback_execute(enigma_user::gui_event_drag);

    if (enigma_user::mouse_check_button_released(enigma_user::mb_left)) {
      drag = false;
      callback_execute(enigma_user::gui_event_released);
    }
  }
}

void Scrollbar::draw(gs_scalar ox, gs_scalar oy) {
  //Draw scrollbar and indicator
  get_data_element(sty, Style, GUI_TYPE::STYLE, style_id);
  if (sty.sprites[state] != -1) {
    if (sty.border.zero == true) {
      enigma_user::draw_sprite_stretched(sty.sprites[state], -1, box.x, box.y, box.w, box.h,
                                         sty.sprite_styles[state].color, sty.sprite_styles[state].alpha);
    } else {
      enigma_user::draw_sprite_padded(sty.sprites[state], -1, sty.border.left, sty.border.top, sty.border.right,
                                      sty.border.bottom, ox + box.x, oy + box.y, ox + box.x + box.w, oy + box.y + box.h,
                                      sty.sprite_styles[state].color, sty.sprite_styles[state].alpha);
    }
  }

  get_data_element(sty_ind, Style, GUI_TYPE::STYLE, style_id);
  if (sty_ind.sprites[state] != -1) {
    if (direction == false) {
      if (sty_ind.border.zero == true) {
        enigma_user::draw_sprite_stretched(
            sty_ind.sprites[state], -1, sty_ind.image_offset.x + ox + box.x + scroll_offset + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + indicator_box.y, indicator_box.w, indicator_box.h,
            sty_ind.sprite_styles[state].color, sty_ind.sprite_styles[state].alpha);
      } else {
        enigma_user::draw_sprite_padded(
            sty_ind.sprites[state], -1, sty_ind.border.left, sty_ind.border.top, sty_ind.border.right,
            sty_ind.border.bottom, sty_ind.image_offset.x + ox + box.x + scroll_offset + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + indicator_box.y,
            sty_ind.image_offset.x + ox + box.x + indicator_box.w + scroll_offset + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + indicator_box.h + indicator_box.y, sty_ind.sprite_styles[state].color,
            sty_ind.sprite_styles[state].alpha);
      }
    } else {
      if (sty_ind.border.zero == true) {
        enigma_user::draw_sprite_stretched(
            sty_ind.sprites[state], -1, sty_ind.image_offset.x + ox + box.x + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + scroll_offset + indicator_box.y, indicator_box.w, indicator_box.h,
            sty_ind.sprite_styles[state].color, sty_ind.sprite_styles[state].alpha);
      } else {
        enigma_user::draw_sprite_padded(
            sty_ind.sprites[state], -1, sty_ind.border.left, sty_ind.border.top, sty_ind.border.right,
            sty_ind.border.bottom, sty_ind.image_offset.x + ox + box.x + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + scroll_offset + indicator_box.y,
            sty_ind.image_offset.x + ox + box.x + indicator_box.w + indicator_box.x,
            sty_ind.image_offset.y + oy + box.y + indicator_box.h + scroll_offset + indicator_box.y,
            sty_ind.sprite_styles[state].color, sty_ind.sprite_styles[state].alpha);
      }
    }
  }
}
}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_scrollbar_create() {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Scrollbar(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, ski.scrollbar_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(scr, guiElements.gui_elements_maxid));
  }
  Scrollbar &scr = guiElements.gui_elements[guiElements.gui_elements_maxid];
  scr.visible = true;
  scr.id = guiElements.gui_elements_maxid;
  return (guiElements.gui_elements_maxid++);
}

int gui_scrollbar_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                         bool direction, double val, double minVal, double maxVal, double incrVal, gs_scalar size) {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Scrollbar(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, ski.scrollbar_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(scr, guiElements.gui_elements_maxid));
  }
  Scrollbar &scr = guiElements.gui_elements[guiElements.gui_elements_maxid];
  scr.visible = true;
  scr.id = guiElements.gui_elements_maxid;
  scr.box.set(x, y, w, h);
  scr.minValue = minVal;
  scr.maxValue = maxVal;
  scr.size = size;
  scr.value = val;
  scr.direction = direction;

  if (direction == false) {
    scr.indicator_box.set(ind_x, ind_y, size * w, h);
  } else {
    scr.indicator_box.set(ind_x, ind_y, w, size * h);
  }

  if (incrVal == 0) {
    scr.incValue = 1.0 / (direction == false ? w : h);
  } else {
    scr.incValue = incrVal;
  }

  scr.update_spos();
  return (guiElements.gui_elements_maxid++);
}

int gui_scrollbar_duplicate(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                   std::forward_as_tuple(scr, guiElements.gui_elements_maxid));
  guiElements.gui_elements[guiElements.gui_elements_maxid].id = guiElements.gui_elements_maxid;
  Scrollbar &sc = guiElements.gui_elements[guiElements.gui_elements_maxid];
  sc.id = guiElements.gui_elements_maxid;
  sc.parent_id = -1;  //We cannot duplicate parenting for now
  return guiElements.gui_elements_maxid++;
}

void gui_scrollbar_destroy(int id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  if (scr.parent_id != -1) {
    gui_window_remove_scrollbar(scr.parent_id, id);
  }
  guiElements.gui_elements.erase(guiElements.gui_elements.find(id));
}

///Setters
void gui_scrollbar_set_position(int id, gs_scalar x, gs_scalar y) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.box.x = x;
  scr.box.y = y;
}

void gui_scrollbar_set_size(int id, gs_scalar w, gs_scalar h) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.box.w = w;
  scr.box.h = h;
  if (scr.direction == false) {
    scr.indicator_box.w = scr.size * w;
  } else {
    scr.indicator_box.h = scr.size * h;
  }
  gui_scrollbar_set_incvalue(id, gui_scrollbar_get_incvalue(id));
}

void gui_scrollbar_set_callback(int id, int event, int script_id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  if (event == enigma_user::gui_event_all) {
    scr.callback.fill(script_id);
  } else {
    scr.callback[event] = script_id;
  }
}

void gui_scrollbar_set_style(int id, int style_id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  check_data_element(GUI_TYPE::STYLE, style_id);
  scr.style_id = (style_id != -1 ? style_id : guiElements.gui_style_scrollbar);
}

void gui_scrollbar_set_indicator_style(int id, int style_id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  check_data_element(GUI_TYPE::STYLE, style_id);
  scr.indicator_style_id = (style_id != -1 ? style_id : guiElements.gui_style_scrollbar);
}

void gui_scrollbar_set_visible(int id, bool visible) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.visible = visible;
}

void gui_scrollbar_set_active(int id, bool active) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.active = active;
}

void gui_scrollbar_set_value(int id, double value) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.value = value;
  scr.update_spos();
}

void gui_scrollbar_set_minvalue(int id, double minvalue) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.minValue = minvalue;
  scr.update_spos();
}

void gui_scrollbar_set_maxvalue(int id, double maxvalue) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.maxValue = maxvalue;
  scr.update_spos();
}

void gui_scrollbar_set_incvalue(int id, double incvalue) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  if (incvalue == 0) {
    scr.incValue = 1.0 / (scr.direction == false ? scr.box.w : scr.box.h);
  } else {
    scr.incValue = incvalue;
  }
  scr.update_spos();
}

void gui_scrollbar_set_indicator_size(int id, gs_scalar size) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  size = (size < 0.0 ? 0.0 : (size > 1.0 ? 1.0 : size));
  scr.size = size;
  if (scr.direction == false) {
    scr.indicator_box.w = scr.size * scr.box.w;
  } else {
    scr.indicator_box.h = scr.size * scr.box.h;
  }
  scr.update_spos();
}

void gui_scrollbar_set_direction(int id, bool direction) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.direction = direction;
}

///Getters
int gui_scrollbar_get_style(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.style_id;
}

int gui_scrollbar_get_indicator_style(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.indicator_style_id;
}

int gui_scrollbar_get_state(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.state;
}

int gui_scrollbar_get_callback(int id, int event) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.callback[event];
}

bool gui_scrollbar_get_active(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, false);
  return scr.active;
}

bool gui_scrollbar_get_visible(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, false);
  return scr.visible;
}

gs_scalar gui_scrollbar_get_width(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.box.w;
}

gs_scalar gui_scrollbar_get_height(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.box.h;
}

gs_scalar gui_scrollbar_get_x(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.box.x;
}

gs_scalar gui_scrollbar_get_y(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.box.y;
}

gs_scalar gui_scrollbar_get_indicator_width(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.indicator_box.w;
}

gs_scalar gui_scrollbar_get_indicator_height(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.indicator_box.h;
}

gs_scalar gui_scrollbar_get_indicator_x(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.indicator_box.x;
}

gs_scalar gui_scrollbar_get_indicator_y(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.indicator_box.y;
}

double gui_scrollbar_get_value(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.value;
}

double gui_scrollbar_get_minvalue(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.minValue;
}

double gui_scrollbar_get_maxvalue(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.maxValue;
}

double gui_scrollbar_get_incvalue(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.incValue;
}

gs_scalar gui_scrollbar_get_indicator_size(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, -1);
  return scr.size;
}

bool gui_scrollbar_get_direction(int id) {
  get_elementv(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id, false);
  return scr.direction;
}

///Drawers
void gui_scrollbar_draw(int id) {
  get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, id);
  scr.update();
  scr.draw();
}

void gui_scrollbars_draw() {
  for (auto &s : guiElements.gui_elements) {
    ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
    if (s.second.type == GUI_TYPE::SCROLLBAR) {
      get_element(scr, Scrollbar, GUI_TYPE::SCROLLBAR, s.first);
      if (scr.visible == true && scr.parent_id == -1) {
        scr.update();
        scr.draw();
      }
    }
  }
}
}  //namespace enigma_user
