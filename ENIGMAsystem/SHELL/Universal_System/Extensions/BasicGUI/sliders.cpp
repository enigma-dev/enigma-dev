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
#include "sliders.h"
#include "include.h"
#include "common.h"

namespace gui
{
  unordered_map<unsigned int, gui_slider> gui_sliders;
	unsigned int gui_sliders_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
  extern unordered_map<unsigned int, gui_style> gui_styles;
	extern unsigned int gui_skins_maxid;
  extern unsigned int gui_style_slider;

	extern bool windowStopPropagation; //This stops event propagation between window elements

	//Implements slider class
	gui_slider::gui_slider(){
    style_id = gui_style_slider; //Default style
	  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
	}

	//Update all possible slider states (hover, click etc.)
	void gui_slider::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
		if (box.point_inside(tx-ox,ty-oy)){
      gui::windowStopPropagation = true;
			if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        state = enigma_user::gui_state_active;
        drag = true;
        if (indicator_box.point_inside(tx-ox-box.x-slider_offset,ty-oy)){
          printf("CLicked on indicator!\n");
          drag_xoffset = tx-ox-indicator_box.x;
        }else{
          printf("CLicked on slider!\n");
          slider_offset = fmin(fmax(0,tx-ox), box.w);
          drag_xoffset = 0.0;
        }
      }else{
				if (state != enigma_user::gui_state_active){
					state = enigma_user::gui_state_hover;
				}else{
					if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
						active = false;
						if (callback != -1){
							enigma_user::script_execute(callback, id, active);
						}
						state = enigma_user::gui_state_hover;
					}
				}
			}
		}else{
      state = enigma_user::gui_state_default;
		}
		if (drag == true){
      windowStopPropagation = true;
      slider_offset = fmin(fmax(0,tx-ox-drag_xoffset), box.w);
      value = minValue + slider_offset/box.w * (maxValue-minValue);
      //printf("Box.x = %f\n", value);
			if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				drag = false;
			}
		}
	}

	void gui_slider::draw(gs_scalar ox, gs_scalar oy){
		//Draw slider and indicator
    if (gui::gui_styles[style_id].sprites[state] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[state],-1,gui::gui_styles[style_id].border.left,gui::gui_styles[style_id].border.top,gui::gui_styles[style_id].border.right,gui::gui_styles[style_id].border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
		}
    if (sprites_indicator[state] != -1){
      enigma_user::draw_sprite_padded(sprites_indicator[state],-1,indicator_border.left,indicator_border.top,indicator_border.right,indicator_border.bottom,ox + box.x + slider_offset,oy + box.y,ox + box.x + indicator_box.w + slider_offset,oy + box.y+indicator_box.h);
		}

		//Draw text
		font_styles[state].use();
		enigma_user::draw_text(ox + font_styles[state].textx,oy + font_styles[state].texty,text);
	}

	void gui_slider::update_text_pos(int state){
	}
}

namespace enigma_user
{
	int gui_slider_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_sliders.insert(pair<unsigned int, gui::gui_slider >(gui::gui_sliders_maxid, gui::gui_slider()));
		}else{
			gui::gui_sliders.insert(pair<unsigned int, gui::gui_slider >(gui::gui_sliders_maxid, gui::gui_sliders[gui::gui_skins[gui::gui_bound_skin].slider_style]));
		}
		gui::gui_sliders[gui::gui_sliders_maxid].visible = true;
		gui::gui_sliders[gui::gui_sliders_maxid].id = gui::gui_sliders_maxid;
		return (gui::gui_sliders_maxid++);
	}

	int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_w, gs_scalar ind_h, double val, double minVal, double maxVal, double incrVal, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_sliders.insert(pair<unsigned int, gui::gui_slider >(gui::gui_sliders_maxid, gui::gui_slider()));
		}else{
			gui::gui_sliders.insert(pair<unsigned int, gui::gui_slider >(gui::gui_sliders_maxid, gui::gui_sliders[gui::gui_skins[gui::gui_bound_skin].slider_style]));
		}
		gui::gui_sliders[gui::gui_sliders_maxid].visible = true;
		gui::gui_sliders[gui::gui_sliders_maxid].id = gui::gui_sliders_maxid;
		gui::gui_sliders[gui::gui_sliders_maxid].box.x = x;
		gui::gui_sliders[gui::gui_sliders_maxid].box.y = y;
		gui::gui_sliders[gui::gui_sliders_maxid].box.w = w;
		gui::gui_sliders[gui::gui_sliders_maxid].box.h = h;
		gui::gui_sliders[gui::gui_sliders_maxid].indicator_box.w = ind_w;
		gui::gui_sliders[gui::gui_sliders_maxid].indicator_box.h = ind_h;
    gui::gui_sliders[gui::gui_sliders_maxid].minValue = minVal;
		gui::gui_sliders[gui::gui_sliders_maxid].maxValue = maxVal;
		gui::gui_sliders[gui::gui_sliders_maxid].incValue = incrVal;
    gui::gui_sliders[gui::gui_sliders_maxid].value = val;

		gui::gui_sliders[gui::gui_sliders_maxid].text = text;
		gui::gui_sliders[gui::gui_sliders_maxid].update_text_pos();
		return (gui::gui_sliders_maxid++);
	}

	void gui_slider_destroy(int id){
		gui::gui_sliders.erase(gui::gui_sliders.find(id));
	}

	void gui_slider_set_text(int id, string text){
		gui::gui_sliders[id].text = text;
	}

	void gui_slider_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_sliders[id].box.x = x;
		gui::gui_sliders[id].box.y = y;
	}

	void gui_slider_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_sliders[id].box.w = w;
		gui::gui_sliders[id].box.h = h;
		gui::gui_sliders[id].update_text_pos();
	}

	void gui_slider_set_callback(int id, int script_id){
		gui::gui_sliders[id].callback = script_id;
	}

  void gui_toggle_set_style(int id, int style_id){
    gui::gui_toggles[id].style_id = (style_id != -1? style_id : gui::gui_style_slider);
  }

  void gui_toggle_set_indicator_style(int id, int style_id){
    gui::gui_toggles[id].indicator_style_id = (style_id != -1? style_id : gui::gui_style_slider);
  }

	int gui_slider_get_state(int id){
		return gui::gui_sliders[id].state;
	}

	bool gui_slider_get_active(int id){
		return gui::gui_sliders[id].active;
	}

  double gui_slider_get_value(int id){
		return gui::gui_sliders[id].value;
  }

	void gui_slider_set_visible(int id, bool visible){
		gui::gui_sliders[id].visible = visible;
	}

	void gui_slider_draw(int id){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    gui::gui_sliders[id].update();
		gui::gui_sliders[id].draw();
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_sliders_draw(){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		for (unsigned int i=0; i<gui::gui_sliders_maxid; ++i){
			if (gui::gui_sliders[i].visible == true && gui::gui_sliders[i].parent_id == -1){
      	gui::gui_sliders[i].update();
				gui::gui_sliders[i].draw();
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}
}

