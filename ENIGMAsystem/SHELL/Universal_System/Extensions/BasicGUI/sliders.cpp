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
#include "skins.h"
#include "sliders.h"
#include "styles.h"

#include <string>
#include <unordered_map>

using namespace enigma::gui;
using std::string;
using std::unordered_map;

namespace enigma {
namespace gui {

//Implements slider class
Slider::Slider() {
  style_id = guiElements.gui_style_slider;  //Default style
  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
  enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
  callback.fill(-1);  //Default callbacks don't exist (so it doesn't call any script)
}

void Slider::callback_execute(int event) {
  if (callback[event] != -1) {
    enigma_user::script_execute(callback[event], id, active, state, event);
  }
}

void Slider::update_spos() {
  slider_offset = box.w * ((value - minValue) / (maxValue - minValue));
  rangeValue = fabs(minValue) + fabs(maxValue);
  segments = (maxValue - minValue) / incValue;
}

//Update all possible slider states (hover, click etc.)
void Slider::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty) {
  if ((box.point_inside(tx - ox, ty - oy) ||
       indicator_box.point_inside(tx - ox - box.x - slider_offset - indicator_box.x,
                                  ty - box.y - oy - indicator_box.y)) &&
      windowStopPropagation == false) {
    callback_execute(enigma_user::gui_event_hover);
    windowStopPropagation = true;
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) {
      state = enigma_user::gui_state_active;
      active = true;
      drag = true;
      if (indicator_box.point_inside(tx - ox - box.x - slider_offset - indicator_box.x,
                                     ty - box.y - oy - indicator_box.y)) {
        drag_xoffset = tx - ox - indicator_box.x - box.x - slider_offset;
      } else {
        drag_xoffset = 0.0;
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
    slider_offset = fmin(fmax(0, tx - box.x - ox - drag_xoffset), box.w);
    value = round((minValue + slider_offset / box.w * rangeValue) / incValue) * incValue;
    slider_offset = box.w * ((value - minValue) / (maxValue - minValue));
    callback_execute(enigma_user::gui_event_drag);
    if (enigma_user::mouse_check_button_released(enigma_user::mb_left)) {
      drag = false;
      callback_execute(enigma_user::gui_event_released);
    }
  }
}

void Slider::draw(gs_scalar ox, gs_scalar oy) {
  //Draw slider and indicator
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

  get_data_element(sty_ind, Style, GUI_TYPE::STYLE, indicator_style_id);
  if (sty_ind.sprites[state] != -1) {
    if (sty_ind.border.zero == true) {
      enigma_user::draw_sprite_stretched(
          sty_ind.sprites[state], -1, sty_ind.image_offset.x + ox + box.x + slider_offset + indicator_box.x,
          sty_ind.image_offset.y + oy + box.y + indicator_box.y, indicator_box.w, indicator_box.h,
          sty_ind.sprite_styles[state].color, sty_ind.sprite_styles[state].alpha);
    } else {
      enigma_user::draw_sprite_padded(
          sty_ind.sprites[state], -1, sty_ind.border.left, sty_ind.border.top, sty_ind.border.right,
          sty_ind.border.bottom, sty_ind.image_offset.x + ox + box.x + slider_offset + indicator_box.x,
          sty_ind.image_offset.y + oy + box.y + indicator_box.y,
          sty_ind.image_offset.x + ox + box.x + indicator_box.w + slider_offset + indicator_box.x,
          sty_ind.image_offset.y + oy + box.y + indicator_box.h + indicator_box.y, sty_ind.sprite_styles[state].color,
          sty_ind.sprite_styles[state].alpha);
    }
  }

  if (text.empty() == false) {
    //Draw text
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
}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_slider_create() {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Slider(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(sli, Slider, GUI_TYPE::SLIDER, ski.slider_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(sli, guiElements.gui_elements_maxid));
  }
  Slider &s = guiElements.gui_elements[guiElements.gui_elements_maxid];
  s.visible = true;
  s.id = guiElements.gui_elements_maxid;
  return (guiElements.gui_elements_maxid++);
}

int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                      gs_scalar ind_w, gs_scalar ind_h, string text) {
  return gui_slider_create(x, y, w, h, ind_x, ind_y, ind_w, ind_h, 0, 0, 1, 0, text);
}

int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                      gs_scalar ind_w, gs_scalar ind_h, double val, double minVal, double maxVal, double incrVal,
                      string text) {
  if (gui_bound_skin == -1) {  //Add default one
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(Slider(), guiElements.gui_elements_maxid));
  } else {
    get_data_elementv(ski, Skin, GUI_TYPE::SKIN, gui_bound_skin, -1);
    get_elementv(sli, Slider, GUI_TYPE::SLIDER, ski.slider_style, -1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                     std::forward_as_tuple(sli, guiElements.gui_elements_maxid));
  }
  Slider &s = guiElements.gui_elements[guiElements.gui_elements_maxid];
  s.visible = true;
  s.id = guiElements.gui_elements_maxid;
  s.box.set(x, y, w, h);
  s.indicator_box.set(ind_x, ind_y, ind_w, ind_h);
  s.minValue = minVal;
  s.maxValue = maxVal;
  s.value = val;
  s.rangeValue = fabs(minVal) + fabs(maxVal);

  if (incrVal == 0) {
    s.incValue = 1.0 / w;
  } else {
    s.incValue = incrVal;
  }

  s.text = text;
  s.update_spos();
  return (guiElements.gui_elements_maxid++);
}

int gui_slider_duplicate(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid),
                                   std::forward_as_tuple(sli, guiElements.gui_elements_maxid));
  guiElements.gui_elements[guiElements.gui_elements_maxid].id = guiElements.gui_elements_maxid;
  Slider &s = guiElements.gui_elements[guiElements.gui_elements_maxid];
  s.id = guiElements.gui_elements_maxid;
  s.parent_id = -1;  //We cannot duplicate parenting for now
  return guiElements.gui_elements_maxid++;
}

void gui_slider_destroy(int id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  if (sli.parent_id != -1) {
    gui_window_remove_slider(sli.parent_id, id);
  }
  guiElements.gui_elements.erase(guiElements.gui_elements.find(id));
}

///Setters
void gui_slider_set_text(int id, string text) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.text = text;
}

void gui_slider_set_position(int id, gs_scalar x, gs_scalar y) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.box.x = x;
  sli.box.y = y;
}

void gui_slider_set_size(int id, gs_scalar w, gs_scalar h) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.box.w = w;
  sli.box.h = h;
  gui_slider_set_incvalue(id, gui_slider_get_incvalue(id));
}

void gui_slider_set_callback(int id, int event, int script_id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  if (event == enigma_user::gui_event_all) {
    sli.callback.fill(script_id);
  } else {
    sli.callback[event] = script_id;
  }
}

void gui_slider_set_style(int id, int style_id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  check_data_element(GUI_TYPE::STYLE, style_id);
  sli.style_id = (style_id != -1 ? style_id : guiElements.gui_style_slider);
}

void gui_slider_set_indicator_style(int id, int style_id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  check_data_element(GUI_TYPE::STYLE, style_id);
  sli.indicator_style_id = (style_id != -1 ? style_id : guiElements.gui_style_slider);
}

void gui_slider_set_visible(int id, bool visible) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.visible = visible;
}

void gui_slider_set_active(int id, bool active) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.active = active;
}

void gui_slider_set_value(int id, double value) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.value = fmin(fmax(value, sli.minValue), sli.maxValue);
  sli.update_spos();
}

void gui_slider_set_minvalue(int id, double minvalue) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.minValue = minvalue;
  sli.update_spos();
}

void gui_slider_set_maxvalue(int id, double maxvalue) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  sli.maxValue = maxvalue;
  sli.update_spos();
}

void gui_slider_set_incvalue(int id, double incvalue) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  if (incvalue == 0) {
    sli.incValue = 1.0 / sli.box.w;
  } else {
    sli.incValue = incvalue;
  }
  sli.update_spos();
}

///Getters
int gui_slider_get_style(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.style_id;
}

int gui_slider_get_indicator_style(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.indicator_style_id;
}

int gui_slider_get_state(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.state;
}

int gui_slider_get_callback(int id, int event) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.callback[event];
}

bool gui_slider_get_active(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, false);
  return sli.active;
}

bool gui_slider_get_visible(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, false);
  return sli.visible;
}

gs_scalar gui_slider_get_width(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.box.w;
}

gs_scalar gui_slider_get_height(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.box.h;
}

gs_scalar gui_slider_get_x(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.box.x;
}

gs_scalar gui_slider_get_y(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.box.y;
}

gs_scalar gui_slider_get_indicator_width(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.indicator_box.w;
}

gs_scalar gui_slider_get_indicator_height(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.indicator_box.h;
}

gs_scalar gui_slider_get_indicator_x(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.indicator_box.x;
}

gs_scalar gui_slider_get_indicator_y(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.indicator_box.y;
}

string gui_slider_get_text(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, "");
  return sli.text;
}

double gui_slider_get_value(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.value;
}

double gui_slider_get_minvalue(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.minValue;
}

double gui_slider_get_maxvalue(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.maxValue;
}

double gui_slider_get_incvalue(int id) {
  get_elementv(sli, Slider, GUI_TYPE::SLIDER, id, -1);
  return sli.incValue;
}

///Drawers
void gui_slider_draw(int id) {
  get_element(sli, Slider, GUI_TYPE::SLIDER, id);
  int pfont = enigma_user::draw_get_font();
  unsigned int phalign = enigma_user::draw_get_halign();
  unsigned int pvalign = enigma_user::draw_get_valign();
  int pcolor = enigma_user::draw_get_color();
  gs_scalar palpha = enigma_user::draw_get_alpha();
  sli.update();
  sli.draw();
  enigma_user::draw_set_halign(phalign);
  enigma_user::draw_set_valign(pvalign);
  enigma_user::draw_set_color(pcolor);
  enigma_user::draw_set_alpha(palpha);
  enigma_user::draw_set_font(pfont);
}

void gui_sliders_draw() {
  int pfont = enigma_user::draw_get_font();
  unsigned int phalign = enigma_user::draw_get_halign();
  unsigned int pvalign = enigma_user::draw_get_valign();
  int pcolor = enigma_user::draw_get_color();
  gs_scalar palpha = enigma_user::draw_get_alpha();
  for (auto &b : guiElements.gui_elements) {
    ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
    if (b.second.type == GUI_TYPE::SLIDER) {
      get_element(but, Slider, GUI_TYPE::SLIDER, b.first);
      if (but.visible == true && but.parent_id == -1) {
        but.update();
        but.draw();
      }
    }
  }
  enigma_user::draw_set_halign(phalign);
  enigma_user::draw_set_valign(pvalign);
  enigma_user::draw_set_color(pcolor);
  enigma_user::draw_set_alpha(palpha);
  enigma_user::draw_set_font(pfont);
}
}  // namespace enigma_user
