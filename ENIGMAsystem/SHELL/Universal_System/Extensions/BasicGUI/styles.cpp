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
#include "Universal_System/var4.h"

#include "elements.h"
#include "include.h"
#include "styles.h"

#include <string>
#include <unordered_map>

using namespace enigma::gui;
using std::string;
using std::unordered_map;

namespace enigma {
namespace gui {

Style::Style() {
  font_styles[0].halign = font_styles[1].halign = font_styles[2].halign = font_styles[3].halign =
      font_styles[4].halign = font_styles[5].halign = enigma_user::fa_left;
  font_styles[0].valign = font_styles[1].valign = font_styles[2].valign = font_styles[3].valign =
      font_styles[4].valign = font_styles[5].valign = enigma_user::fa_middle;
  sprites.fill(-1);
}
}  //namespace gui
}  //namespace enigma

namespace enigma_user {
int gui_style_create() {
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, Style());
  Style &sty = guiElements.gui_data_elements[guiElements.gui_data_elements_maxid];
  sty.id = guiElements.gui_data_elements_maxid;
  return guiElements.gui_data_elements_maxid++;
}

int gui_style_duplicate(int style) {
  get_data_elementv(sty, Style, GUI_TYPE::STYLE, style, -1);
  guiElements.gui_data_elements.emplace(guiElements.gui_data_elements_maxid, sty);
  guiElements.gui_data_elements[guiElements.gui_data_elements_maxid].id = guiElements.gui_data_elements_maxid;
  return guiElements.gui_data_elements_maxid++;
}

void gui_style_destroy(int id) { guiElements.gui_data_elements.erase(guiElements.gui_data_elements.find(id)); }

void gui_style_set_font(int id, int state, int font) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.font_styles) {
      it.font = font;
    }
  } else {
    sty.font_styles[state].font = font;
  }
}

void gui_style_set_font_halign(int id, int state, unsigned int halign) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.font_styles) {
      it.halign = halign;
    }
  } else {
    sty.font_styles[state].halign = halign;
  }
}

void gui_style_set_font_valign(int id, int state, unsigned int valign) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.font_styles) {
      it.valign = valign;
    }
  } else {
    sty.font_styles[state].valign = valign;
  }
}

void gui_style_set_font_color(int id, int state, int color) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.font_styles) {
      it.color = color;
    }
  } else {
    sty.font_styles[state].color = color;
  }
}

void gui_style_set_font_alpha(int id, int state, gs_scalar alpha) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.font_styles) {
      it.alpha = alpha;
    }
  } else {
    sty.font_styles[state].alpha = alpha;
  }
}

void gui_style_set_sprite(int id, int state, int sprid) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    sty.sprites.fill(sprid);
  } else {
    sty.sprites[state] = sprid;
  }
}

void gui_style_set_sprite_color(int id, int state, int color) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.sprite_styles) {
      it.color = color;
    }
  } else {
    sty.sprite_styles[state].color = color;
  }
}

void gui_style_set_sprite_alpha(int id, int state, gs_scalar alpha) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.sprite_styles) {
      it.alpha = alpha;
    }
  } else {
    sty.sprite_styles[state].alpha = alpha;
  }
}

void gui_style_set_sprite_direction(int id, int state, gs_scalar direction) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.sprite_styles) {
      it.rotation = direction;
    }
  } else {
    sty.sprite_styles[state].rotation = direction;
  }
}

void gui_style_set_sprite_scale(int id, int state, gs_scalar scale_x, gs_scalar scale_y) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  if (state == enigma_user::gui_state_all) {
    for (auto &it : sty.sprite_styles) {
      it.scale_x = scale_x;
      it.scale_y = scale_y;
    }
  } else {
    sty.sprite_styles[state].scale_x = scale_x;
    sty.sprite_styles[state].scale_y = scale_y;
  }
}

void gui_style_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  sty.padding.set(left, top, right, bottom);
}

void gui_style_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  sty.border.set(left, top, right, bottom);
}

void gui_style_set_image_offset(int id, gs_scalar x, gs_scalar y) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  sty.image_offset.set(x, y);
}

void gui_style_set_box(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h) {
  get_data_element(sty, Style, GUI_TYPE::STYLE, id);
  sty.box.set(x, y, w, h);
}
}  // namespace enigma_user
