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
#include "windows.h"
#include "include.h"
#include "common.h"

//Children
#include "buttons.h"
#include "toggles.h"

namespace gui
{
  bool windowStopPropagation = false; //Stop event propagation in windows and between
	unordered_map<unsigned int, gui_window> gui_windows;
	unsigned int gui_windows_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
	extern unordered_map<unsigned int, gui_button> gui_buttons;
	extern unordered_map<unsigned int, gui_toggle> gui_toggles;
	extern unsigned int gui_skins_maxid;

	//Implements button class
	void gui_window::reset(){
		text = "", state = 0, sprite = sprite_on = -1;
		callback = -1;
		visible = true;
    drag = false;
		font_styles[0].halign = font_styles[1].halign = enigma_user::fa_center;
		font_styles[0].valign = font_styles[1].valign = enigma_user::fa_top;
	}

	gui_window::gui_window(){
    drag_xoffset = 0;
    drag_yoffset = 0;
		reset();
	}

	//Update all possible button states (hover, click, toggle etc.)
	void gui_window::update(gs_scalar tx, gs_scalar ty){
    if (box.point_inside(tx,ty)){ //Hover
        windowStopPropagation = true;
    }

    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){ //Press
      if(box.point_inside(tx,ty)){
        state = enigma_user::gui_state_on;
        drag = true;
        drag_xoffset = tx-box.x;
        drag_yoffset = ty-box.y;
      }else{
      state = enigma_user::gui_state_default;
      }
    }

		if (drag == true){
      windowStopPropagation = true;
			box.x = tx-drag_xoffset;
			box.y = ty-drag_yoffset;
			if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				drag = false;
			}
			update_text_pos();
		}
	}

	void gui_window::draw(){
		//Draw button
		switch (state){
			case enigma_user::gui_state_default: //Default off
				if (sprite!=-1){
					enigma_user::draw_sprite_padded(sprite,-1,border.left,border.top,border.right,border.bottom,box.x,box.y,box.x+box.w,box.y+box.h);
				}
			break;
			case enigma_user::gui_state_on: //Default on
				if (sprite_on!=-1){
					enigma_user::draw_sprite_padded(sprite_on,-1,border.left,border.top,border.right,border.bottom,box.x,box.y,box.x+box.w,box.y+box.h);
				}
			break;
		}

		//Draw text
		font_styles[state].use();
		enigma_user::draw_text(font_styles[state].textx,font_styles[state].texty,text);
	}

	void gui_window::update_text_pos(int state){
		if (state == -1){
			update_text_pos(enigma_user::gui_state_default);
			update_text_pos(enigma_user::gui_state_on);
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
			style->texty = box.y+padding.top+padding.top/2.0;
		}else if (style->valign == enigma_user::fa_bottom){
			style->texty = box.y+padding.top*2.0;
		}
	}
}

namespace enigma_user
{
	int gui_window_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_windows.insert(pair<unsigned int, gui::gui_window >(gui::gui_windows_maxid, gui::gui_window()));
		}else{
			gui::gui_windows.insert(pair<unsigned int, gui::gui_window >(gui::gui_windows_maxid, gui::gui_windows[gui::gui_skins[gui::gui_bound_skin].window_style]));
		}
		gui::gui_windows[gui::gui_windows_maxid].visible = true;
		gui::gui_windows[gui::gui_windows_maxid].id = gui::gui_windows_maxid;
		return gui::gui_windows_maxid++;
	}

	int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_windows.insert(pair<unsigned int, gui::gui_window >(gui::gui_windows_maxid, gui::gui_window()));
		}else{
			gui::gui_windows.insert(pair<unsigned int, gui::gui_window >(gui::gui_windows_maxid, gui::gui_windows[gui::gui_skins[gui::gui_bound_skin].window_style]));
		}
		gui::gui_windows[gui::gui_windows_maxid].visible = true;
		gui::gui_windows[gui::gui_windows_maxid].id = gui::gui_windows_maxid;
		gui::gui_windows[gui::gui_windows_maxid].box.x = x;
		gui::gui_windows[gui::gui_windows_maxid].box.y = y;
		gui::gui_windows[gui::gui_windows_maxid].box.w = w;
		gui::gui_windows[gui::gui_windows_maxid].box.h = h;
		gui::gui_windows[gui::gui_windows_maxid].text = text;
		gui::gui_windows[gui::gui_windows_maxid].update_text_pos();
		return gui::gui_windows_maxid++;
	}

	void gui_window_destroy(int id){
		gui::gui_windows.erase(gui::gui_windows.find(id));
	}

	void gui_window_set_text(int id, string text){
		gui::gui_windows[id].text = text;
	}

	void gui_window_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_windows[id].box.x = x;
		gui::gui_windows[id].box.y = y;
	}

	void gui_window_set_font(int id, int state, int font){
		if (state == enigma_user::gui_state_all){
			gui::gui_windows[id].font_styles[enigma_user::gui_state_default].font = font;
			gui::gui_windows[id].font_styles[enigma_user::gui_state_on].font = font;
		}else{
			gui::gui_windows[id].font_styles[state].font = font;
		}
	}

	void gui_window_set_font_halign(int id, int state, unsigned int halign){
		if (state == enigma_user::gui_state_all){
			gui::gui_windows[id].font_styles[enigma_user::gui_state_default].halign = halign;
			gui::gui_windows[id].font_styles[enigma_user::gui_state_on].halign = halign;
			gui::gui_windows[id].update_text_pos();
		}else{
			gui::gui_windows[id].font_styles[state].halign = halign;
			gui::gui_windows[id].update_text_pos(state);
		}
	}

	void gui_window_set_font_valign(int id, int state, unsigned int valign){
		if (state == enigma_user::gui_state_all){
			gui::gui_windows[id].font_styles[enigma_user::gui_state_default].valign = valign;
			gui::gui_windows[id].font_styles[enigma_user::gui_state_on].valign = valign;
			gui::gui_windows[id].update_text_pos();
		}else{
			gui::gui_windows[id].font_styles[state].valign = valign;
			gui::gui_windows[id].update_text_pos(state);
		}
	}

	void gui_window_set_font_color(int id, int state, int color){
		if (state == enigma_user::gui_state_all){
			gui::gui_windows[id].font_styles[enigma_user::gui_state_default].color = color;
			gui::gui_windows[id].font_styles[enigma_user::gui_state_on].color = color;
		}else{
			gui::gui_windows[id].font_styles[state].color = color;
		}
	}

	void gui_window_set_font_alpha(int id, int state, gs_scalar alpha){
		if (state == enigma_user::gui_state_all){
			gui::gui_windows[id].font_styles[enigma_user::gui_state_default].alpha = alpha;
			gui::gui_windows[id].font_styles[enigma_user::gui_state_on].alpha = alpha;
		}else{
			gui::gui_windows[id].font_styles[state].alpha = alpha;
		}
	}

	void gui_window_set_sprite(int id, int state, int sprid){
		switch (state){
			case enigma_user::gui_state_default: gui::gui_windows[id].sprite = sprid; break;
			case enigma_user::gui_state_on: gui::gui_windows[id].sprite_on = sprid; break;
		}
	}

	void gui_window_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_windows[id].box.w = w;
		gui::gui_windows[id].box.h = h;
		gui::gui_windows[id].update_text_pos();
	}

	void gui_window_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_windows[id].padding.set(left,top,right,bottom);
		gui::gui_windows[id].update_text_pos();
	}

	void gui_window_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_windows[id].border.set(left,top,right,bottom);
	}

	void gui_window_set_callback(int id, int script_id){
		gui::gui_windows[id].callback = script_id;
	}

	int gui_window_get_state(int id){
		return gui::gui_windows[id].state;
	}

	void gui_window_set_visible(int id, bool visible){
		gui::gui_windows[id].visible = visible;
	}

	void gui_window_draw(int id){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    gui::gui_windows[id].update();
		gui::gui_windows[id].draw();
		//Draw children
    if (gui::gui_windows[id].child_buttons.empty() == false){
      for (unsigned int b=0; b<gui::gui_windows[id].child_buttons.size(); ++b){
        gui::gui_buttons[gui::gui_windows[id].child_buttons[b]].update();
        gui::gui_buttons[gui::gui_windows[id].child_buttons[b]].draw();
      }
    }
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_windows_draw(){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		gui::windowStopPropagation = false;

    //Update loop in reverse direction
    for (int i=gui::gui_windows_maxid-1; i>=0; --i){
      if (gui::gui_windows[i].visible == true){
        //Update children
        if (gui::gui_windows[i].child_buttons.empty() == false){
          for (int b=gui::gui_windows[i].child_buttons.size()-1; b>=0; --b){
            if (gui::gui_buttons[gui::gui_windows[i].child_buttons[b]].visible == false) continue; //Skip invisible objects
            if (gui::windowStopPropagation == false){ gui::gui_buttons[gui::gui_windows[i].child_buttons[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y); } else { break; } //Stop propagation
          }
          for (int b=gui::gui_windows[i].child_toggles.size()-1; b>=0; --b){
            if (gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].visible == false) continue; //Skip invisible objects
            if (gui::windowStopPropagation == false){ gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y); } else { break; } //Stop propagation
          }
        }
        if (gui::windowStopPropagation == false){ gui::gui_windows[i].update(); } else { break; } //Stop propagation
			}
		}

    //Draw loop
    for (unsigned int i=0; i<gui::gui_windows_maxid; ++i){
      if (gui::gui_windows[i].visible == true){
				gui::gui_windows[i].draw();
        //Draw children
        if (gui::gui_windows[i].child_buttons.empty() == false){
          for (unsigned int b=0; b<gui::gui_windows[i].child_buttons.size(); ++b){
            if (gui::gui_buttons[gui::gui_windows[i].child_buttons[b]].visible == true) gui::gui_buttons[gui::gui_windows[i].child_buttons[b]].draw(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
        if (gui::gui_windows[i].child_toggles.empty() == false){
          for (unsigned int b=0; b<gui::gui_windows[i].child_toggles.size(); ++b){
            if (gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].visible == true) gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].draw(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

  void gui_window_add_button(int id, int bid){
    gui::gui_windows[id].child_buttons.push_back(bid);
    gui::gui_buttons[bid].parent_id = id;
  }

  void gui_window_add_toggle(int id, int tid){
    gui::gui_windows[id].child_toggles.push_back(tid);
    gui::gui_toggles[tid].parent_id = id;
  }
}
