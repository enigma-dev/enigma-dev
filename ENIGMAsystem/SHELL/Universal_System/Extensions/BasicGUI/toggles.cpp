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

#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;
using std::pair;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
//#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "styles.h"
#include "skins.h"
#include "toggles.h"
#include "buttons.h"
#include "groups.h"
#include "include.h"
#include "common.h"

namespace gui
{
  unordered_map<unsigned int, gui_toggle> gui_toggles;
	unsigned int gui_toggles_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
  extern unordered_map<unsigned int, gui_style> gui_styles;
  extern unordered_map<unsigned int, gui_group> gui_groups;
  extern unordered_map<unsigned int, gui_button> gui_buttons;
	extern unsigned int gui_skins_maxid;
	extern unsigned int gui_style_toggle;

	extern bool windowStopPropagation;

	//Implements toggle class
	gui_toggle::gui_toggle(){
	  style_id = gui_style_toggle; //Default style
	  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
	}

	//Update all possible toggle states (hover, click, toggle etc.)
	void gui_toggle::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
		if (box.point_inside(tx-ox,ty-oy) && gui::windowStopPropagation == false){
      gui::windowStopPropagation = true;
			if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        if (active == false){
          state = enigma_user::gui_state_active;
        }else{
          state = enigma_user::gui_state_on_active;
        }
			}else{
				if (state != enigma_user::gui_state_active &&  state != enigma_user::gui_state_on_active){
					if (active == false){
						state = enigma_user::gui_state_hover;
					}else{
						state = enigma_user::gui_state_on_hover;
					}
				}else{
					if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
						active = !active;
						if (callback != -1){
							enigma_user::script_execute(callback, id, active);
						}

						if (active == false){
							state = enigma_user::gui_state_hover;
						}else{
							state = enigma_user::gui_state_on_hover;
							if (group_id != -1){ //Groups disable any other active element
                for (auto &b : gui::gui_groups[group_id].group_buttons){
                  gui_buttons[b].active = false;
                }
                for (auto &t : gui::gui_groups[group_id].group_toggles){
                  if (t != id) { gui_toggles[t].active = false; }
                }
							}
						}
					}
				}
			}
		}else{
			if (active == false){
				state = enigma_user::gui_state_default;
			}else{
				state = enigma_user::gui_state_on;
			}
		}
	}

	void gui_toggle::draw(gs_scalar ox, gs_scalar oy){
	  //Draw toggle
    if (gui::gui_styles[style_id].sprites[state] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[state],-1,gui::gui_styles[style_id].border.left,gui::gui_styles[style_id].border.top,gui::gui_styles[style_id].border.right,gui::gui_styles[style_id].border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
		}

		//Draw text
		gui::gui_styles[style_id].font_styles[state].use();

    gs_scalar textx = 0.0, texty = 0.0;
    switch (gui::gui_styles[style_id].font_styles[state].halign){
      case enigma_user::fa_left: textx = box.x+gui::gui_styles[style_id].padding.left; break;
      case enigma_user::fa_center: textx = box.x+box.w/2.0; break;
      case enigma_user::fa_right: textx = box.x+box.w-gui::gui_styles[style_id].padding.right; break;
    }

    switch (gui::gui_styles[style_id].font_styles[state].valign){
      case enigma_user::fa_top: texty = box.y+gui::gui_styles[style_id].padding.top; break;
      case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
      case enigma_user::fa_bottom: texty = box.y+box.h-gui::gui_styles[style_id].padding.bottom; break;
    }

		enigma_user::draw_text(ox + textx,oy + texty,text);
	}

	void gui_toggle::update_text_pos(int state){
	}
}

namespace enigma_user
{
	int gui_toggle_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_toggles.insert(pair<unsigned int, gui::gui_toggle >(gui::gui_toggles_maxid, gui::gui_toggle()));
		}else{
			gui::gui_toggles.insert(pair<unsigned int, gui::gui_toggle >(gui::gui_toggles_maxid, gui::gui_toggles[gui::gui_skins[gui::gui_bound_skin].toggle_style]));
		}
		gui::gui_toggles[gui::gui_toggles_maxid].visible = true;
		gui::gui_toggles[gui::gui_toggles_maxid].id = gui::gui_toggles_maxid;
		return (gui::gui_toggles_maxid++);
	}

	int gui_toggle_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_toggles.insert(pair<unsigned int, gui::gui_toggle >(gui::gui_toggles_maxid, gui::gui_toggle()));
		}else{
			gui::gui_toggles.insert(pair<unsigned int, gui::gui_toggle >(gui::gui_toggles_maxid, gui::gui_toggles[gui::gui_skins[gui::gui_bound_skin].toggle_style]));
		}
		gui::gui_toggles[gui::gui_toggles_maxid].visible = true;
		gui::gui_toggles[gui::gui_toggles_maxid].id = gui::gui_toggles_maxid;
		gui::gui_toggles[gui::gui_toggles_maxid].box.set(x, y, w, h);
		gui::gui_toggles[gui::gui_toggles_maxid].text = text;
		gui::gui_toggles[gui::gui_toggles_maxid].update_text_pos();
		return (gui::gui_toggles_maxid++);
	}

	void gui_toggle_destroy(int id){
		gui::gui_toggles.erase(gui::gui_toggles.find(id));
	}

	void gui_toggle_set_text(int id, string text){
		gui::gui_toggles[id].text = text;
	}

	void gui_toggle_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_toggles[id].box.x = x;
		gui::gui_toggles[id].box.y = y;
	}

	void gui_toggle_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_toggles[id].box.w = w;
		gui::gui_toggles[id].box.h = h;
		gui::gui_toggles[id].update_text_pos();
	}

	void gui_toggle_set_callback(int id, int script_id){
		gui::gui_toggles[id].callback = script_id;
	}

  void gui_toggle_set_style(int id, int style_id){
    gui::gui_toggles[id].style_id = (style_id != -1? style_id : gui::gui_style_toggle);
  }

  int gui_toggle_get_style(int id){
    return gui::gui_toggles[id].style_id;
  }


	int gui_toggle_get_state(int id){
		return gui::gui_toggles[id].state;
	}

	bool gui_toggle_get_active(int id){
		return gui::gui_toggles[id].active;
	}

  void gui_toggle_set_active(int id, bool active){
		gui::gui_toggles[id].active = active;
	}

	void gui_toggle_set_visible(int id, bool visible){
		gui::gui_toggles[id].visible = visible;
	}

	void gui_toggle_draw(int id){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    gui::gui_toggles[id].update();
		gui::gui_toggles[id].draw();
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_toggles_draw(){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		for (auto &t : gui::gui_toggles){
			if (t.second.visible == true && t.second.parent_id == -1){
      	t.second.update();
				t.second.draw();
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}
}

