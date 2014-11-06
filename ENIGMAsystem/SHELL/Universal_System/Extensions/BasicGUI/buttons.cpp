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

#include "skins.h"
#include "buttons.h"
#include "include.h"
#include "common.h"

namespace gui
{
  unordered_map<unsigned int, gui_button> gui_buttons;
	unsigned int gui_buttons_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
	extern unsigned int gui_skins_maxid;

	extern bool windowStopPropagation;

	//Implements button class
	void gui_button::reset(){
		text = "", state = 0, sprite = sprite_hover = sprite_active = sprite_on = sprite_on_hover = -1;
		active = false;
		togglable = false;
		callback = -1;
		visible = true;
		font_styles[0].halign = font_styles[1].halign = font_styles[2].halign = font_styles[3].halign = font_styles[4].halign = enigma_user::fa_center;
		font_styles[0].valign = font_styles[1].valign = font_styles[2].valign = font_styles[3].valign = font_styles[4].valign = enigma_user::fa_middle;
	}

	gui_button::gui_button(){
	  parent_id = -1;
		reset();
	}

	//Update all possible button states (hover, click, toggle etc.)
	void gui_button::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
		if (box.point_inside(tx-ox,ty-oy)){
      gui::windowStopPropagation = true;
			if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
				state = enigma_user::gui_state_active;
			}else{
				if (state != enigma_user::gui_state_active){
					if (active == false){
						state = enigma_user::gui_state_hover;
					}else{
						state = enigma_user::gui_state_on_hover;
					}
				}else{
					if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
						if (togglable == true){
							active = !active;
						}else{
							active = true;
						}
						if (callback != -1){
							enigma_user::script_execute(callback, id, active);
						}
						if (togglable == false){
							active = false;
						}

						if (active == false){
							state = enigma_user::gui_state_hover;
						}else{
							state = enigma_user::gui_state_on_hover;
						}
					}
				}
			}
		}else{
			if (togglable == false) active = false;
			if (active == false){
				state = enigma_user::gui_state_default;
			}else{
				state = enigma_user::gui_state_on;
			}
		}
	}

	void gui_button::draw(gs_scalar ox, gs_scalar oy){
		//Draw button
		switch (state){
			case enigma_user::gui_state_default: //Default off
				if (sprite!=-1){
					enigma_user::draw_sprite_padded(sprite,-1,border.left,border.top,border.right,border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
				}
			break;
			case enigma_user::gui_state_hover: //Default off hover
				if (sprite_hover!=-1){
					enigma_user::draw_sprite_padded(sprite_hover,-1,border.left,border.top,border.right,border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
				}
			break;
			case enigma_user::gui_state_active: //Default active
				if (sprite_active!=-1){
					enigma_user::draw_sprite_padded(sprite_active,-1,border.left,border.top,border.right,border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
				}
			break;
			case enigma_user::gui_state_on: //Default on
				if (sprite_on!=-1){
					enigma_user::draw_sprite_padded(sprite_on,-1,border.left,border.top,border.right,border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
				}
			break;
			case enigma_user::gui_state_on_hover: //Default on hover
				if (sprite_on_hover!=-1){
					enigma_user::draw_sprite_padded(sprite_on_hover,-1,border.left,border.top,border.right,border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
				}
			break;
		}

		//Draw text
		font_styles[state].use();
		enigma_user::draw_text(ox + font_styles[state].textx,oy + font_styles[state].texty,text);
	}

	void gui_button::update_text_pos(int state){
		if (state == -1){
			update_text_pos(enigma_user::gui_state_default);
			update_text_pos(enigma_user::gui_state_hover);
			update_text_pos(enigma_user::gui_state_active);
			update_text_pos(enigma_user::gui_state_on);
			update_text_pos(enigma_user::gui_state_on_hover);
		}

		font_style* style = &font_styles[state];

		if (style->halign == enigma_user::fa_left){
			style->textx = box.x+padding.left;
		}else if (style->halign == enigma_user::fa_center){
			style->textx = box.x+box.w/2.0;
		}else if (style->halign == enigma_user::fa_right){
			style->textx = box.x+box.w-padding.right;
		}

		if (style->valign == enigma_user::fa_top){
			style->texty = box.y+padding.top;
		}else if (style->valign == enigma_user::fa_middle){
			style->texty = box.y+box.h/2.0;
		}else if (style->valign == enigma_user::fa_bottom){
			style->texty = box.y+box.h-padding.bottom;
		}
	}
}

namespace enigma_user
{
	int gui_button_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_buttons.insert(pair<unsigned int, gui::gui_button >(gui::gui_buttons_maxid, gui::gui_button()));
		}else{
			gui::gui_buttons.insert(pair<unsigned int, gui::gui_button >(gui::gui_buttons_maxid, gui::gui_buttons[gui::gui_skins[gui::gui_bound_skin].button_style]));
		}
		gui::gui_buttons[gui::gui_buttons_maxid].visible = true;
		gui::gui_buttons[gui::gui_buttons_maxid].id = gui::gui_buttons_maxid;
		return (gui::gui_buttons_maxid++);
	}

	int gui_button_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_buttons.insert(pair<unsigned int, gui::gui_button >(gui::gui_buttons_maxid, gui::gui_button()));
		}else{
			gui::gui_buttons.insert(pair<unsigned int, gui::gui_button >(gui::gui_buttons_maxid, gui::gui_buttons[gui::gui_skins[gui::gui_bound_skin].button_style]));
		}
		gui::gui_buttons[gui::gui_buttons_maxid].visible = true;
		gui::gui_buttons[gui::gui_buttons_maxid].id = gui::gui_buttons_maxid;
		gui::gui_buttons[gui::gui_buttons_maxid].box.x = x;
		gui::gui_buttons[gui::gui_buttons_maxid].box.y = y;
		gui::gui_buttons[gui::gui_buttons_maxid].box.w = w;
		gui::gui_buttons[gui::gui_buttons_maxid].box.h = h;
		gui::gui_buttons[gui::gui_buttons_maxid].text = text;
		gui::gui_buttons[gui::gui_buttons_maxid].update_text_pos();
		return (gui::gui_buttons_maxid++);
	}

	void gui_button_destroy(int id){
		gui::gui_buttons.erase(gui::gui_buttons.find(id));
	}

	void gui_button_set_text(int id, string text){
		gui::gui_buttons[id].text = text;
	}

	void gui_button_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_buttons[id].box.x = x;
		gui::gui_buttons[id].box.y = y;
	}

	void gui_button_set_font(int id, int state, int font){
		if (state == enigma_user::gui_state_all){
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_default].font = font;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_hover].font = font;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_active].font = font;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on].font = font;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on_hover].font = font;
		}else{
			gui::gui_buttons[id].font_styles[state].font = font;
		}
	}

	void gui_button_set_font_halign(int id, int state, unsigned int halign){
		if (state == enigma_user::gui_state_all){
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_default].halign = halign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_hover].halign = halign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_active].halign = halign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on].halign = halign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on_hover].halign = halign;
			gui::gui_buttons[id].update_text_pos();
		}else{
			gui::gui_buttons[id].font_styles[state].halign = halign;
			gui::gui_buttons[id].update_text_pos(state);
		}
	}

	void gui_button_set_font_valign(int id, int state, unsigned int valign){
		if (state == enigma_user::gui_state_all){
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_default].valign = valign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_hover].valign = valign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_active].valign = valign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on].valign = valign;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on_hover].valign = valign;
			gui::gui_buttons[id].update_text_pos();
		}else{
			gui::gui_buttons[id].font_styles[state].valign = valign;
			gui::gui_buttons[id].update_text_pos(state);
		}
	}

	void gui_button_set_font_color(int id, int state, int color){
		if (state == enigma_user::gui_state_all){
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_default].color = color;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_hover].color = color;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_active].color = color;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on].color = color;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on_hover].color = color;
		}else{
			gui::gui_buttons[id].font_styles[state].color = color;
		}
	}

	void gui_button_set_font_alpha(int id, int state, gs_scalar alpha){
		if (state == enigma_user::gui_state_all){
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_default].alpha = alpha;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_hover].alpha = alpha;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_active].alpha = alpha;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on].alpha = alpha;
			gui::gui_buttons[id].font_styles[enigma_user::gui_state_on_hover].alpha = alpha;
		}else{
			gui::gui_buttons[id].font_styles[state].alpha = alpha;
		}
	}

	void gui_button_set_sprite(int id, int state, int sprid){
		switch (state){
			case enigma_user::gui_state_default: gui::gui_buttons[id].sprite = sprid; break;
			case enigma_user::gui_state_hover: gui::gui_buttons[id].sprite_hover = sprid; break;
			case enigma_user::gui_state_active: gui::gui_buttons[id].sprite_active = sprid; break;
			case enigma_user::gui_state_on: gui::gui_buttons[id].sprite_on = sprid; break;
			case enigma_user::gui_state_on_hover: gui::gui_buttons[id].sprite_on_hover = sprid; break;
		}
	}

	void gui_button_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_buttons[id].box.w = w;
		gui::gui_buttons[id].box.h = h;
		gui::gui_buttons[id].update_text_pos();
	}

	void gui_button_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_buttons[id].padding.set(left,top,right,bottom);
		gui::gui_buttons[id].update_text_pos();
	}

	void gui_button_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_buttons[id].border.set(left,top,right,bottom);
	}

	void gui_button_set_callback(int id, int script_id){
		gui::gui_buttons[id].callback = script_id;
	}

	int gui_button_get_state(int id){
		return gui::gui_buttons[id].state;
	}

	bool gui_button_get_active(int id){
		return gui::gui_buttons[id].active;
	}

	void gui_button_set_togglable(int id, bool togglable){
		gui::gui_buttons[id].togglable = togglable;
	}

	void gui_button_set_visible(int id, bool visible){
		gui::gui_buttons[id].visible = visible;
	}

	void gui_button_draw(int id){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    gui::gui_buttons[id].update();
		gui::gui_buttons[id].draw();
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_buttons_draw(){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		for (unsigned int i=0; i<gui::gui_buttons_maxid; ++i){
			//if (gui::gui_buttons.find(i) !=  gui::gui_buttons.end()){
			if (gui::gui_buttons[i].visible == true && gui::gui_buttons[i].parent_id == -1){
      	gui::gui_buttons[i].update();
				gui::gui_buttons[i].draw();
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}
}

