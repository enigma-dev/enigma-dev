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
#include "windows.h"
#include "include.h"
#include "common.h"

//Children
#include "buttons.h"
#include "toggles.h"
#include "sliders.h"
#include "labels.h"

namespace gui
{
  bool windowStopPropagation = false; //Stop event propagation in windows and between
	unordered_map<unsigned int, gui_window> gui_windows;
	unsigned int gui_windows_maxid = 0;
	extern unsigned int gui_style_window;

  extern unordered_map<unsigned int, gui_style> gui_styles;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
	extern unordered_map<unsigned int, gui_button> gui_buttons;
	extern unordered_map<unsigned int, gui_toggle> gui_toggles;
  extern unordered_map<unsigned int, gui_slider> gui_sliders;
  extern unordered_map<unsigned int, gui_label> gui_labels;
	extern unsigned int gui_skins_maxid;

	//Implements button class
	gui_window::gui_window(){
    style_id = gui_style_window; //Default style
    enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
	}

	//Update all possible button states (hover, click, toggle etc.)
	void gui_window::update(gs_scalar tx, gs_scalar ty){
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left) && gui::windowStopPropagation == false){ //Press
      if(box.point_inside(tx,ty)){
        state = enigma_user::gui_state_on;
        if (draggable == true){
          drag = true;
          drag_xoffset = tx-box.x;
          drag_yoffset = ty-box.y;
        }
      }else{
        state = enigma_user::gui_state_default;
      }
    }

    if (box.point_inside(tx,ty)){ //Hover
        windowStopPropagation = true;
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
	  //Draw window
    if (gui::gui_styles[style_id].sprites[state] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[state],-1,gui::gui_styles[style_id].border.left,gui::gui_styles[style_id].border.top,gui::gui_styles[style_id].border.right,gui::gui_styles[style_id].border.bottom,box.x,box.y,box.x+box.w,box.y+box.h);
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

		enigma_user::draw_text(textx,texty,text);
	}

	void gui_window::update_text_pos(int state){
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
		gui::gui_windows[gui::gui_windows_maxid].box.set(x, y, w, h);
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

	void gui_window_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_windows[id].box.w = w;
		gui::gui_windows[id].box.h = h;
		gui::gui_windows[id].update_text_pos();
	}

	void gui_window_set_callback(int id, int script_id){
		gui::gui_windows[id].callback = script_id;
	}

  void gui_window_set_style(int id, int style_id){
    gui::gui_windows[id].style_id = (style_id != -1? style_id : gui::gui_style_window);
  }

  int gui_window_get_style(int id){
    return gui::gui_windows[id].style_id;
  }

	int gui_window_get_state(int id){
		return gui::gui_windows[id].state;
	}

	void gui_window_set_visible(int id, bool visible){
		gui::gui_windows[id].visible = visible;
	}

  void gui_window_set_draggable(int id, bool draggable){
		gui::gui_windows[id].draggable = draggable;
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
      for (const auto &b : gui::gui_windows[id].child_buttons){
        gui::gui_buttons[b].update();
        gui::gui_buttons[b].draw();
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
            gui::gui_buttons[gui::gui_windows[i].child_buttons[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
        if (gui::gui_windows[i].child_toggles.empty() == false){
          for (int b=gui::gui_windows[i].child_toggles.size()-1; b>=0; --b){
            if (gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].visible == false) continue; //Skip invisible objects
            gui::gui_toggles[gui::gui_windows[i].child_toggles[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
        if (gui::gui_windows[i].child_sliders.empty() == false){
          for (int b=gui::gui_windows[i].child_sliders.size()-1; b>=0; --b){
            if (gui::gui_sliders[gui::gui_windows[i].child_sliders[b]].visible == false) continue;
            gui::gui_sliders[gui::gui_windows[i].child_sliders[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
        gui::gui_windows[i].update();
			}
		}

    //Draw loop
    for (auto &wi : gui::gui_windows){
      auto &w = wi.second;
      if (w.visible == true){
				w.draw();
        //Draw children
        if (w.child_buttons.empty() == false){
          for (const auto &b : w.child_buttons){
            if (gui::gui_buttons[b].visible == true) gui::gui_buttons[b].draw(w.box.x,w.box.y);
          }
        }
        if (w.child_toggles.empty() == false){
          for (const auto &t : w.child_toggles){
            if (gui::gui_toggles[t].visible == true) gui::gui_toggles[t].draw(w.box.x,w.box.y);
          }
        }
        if (w.child_sliders.empty() == false){
          for (const auto &s : w.child_sliders){
            if (gui::gui_sliders[s].visible == true) gui::gui_sliders[s].draw(w.box.x,w.box.y);
          }
        }
        if (w.child_labels.empty() == false){
          for (const auto &l : w.child_labels){
            if (gui::gui_labels[l].visible == true) gui::gui_labels[l].draw(w.box.x,w.box.y);
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

  void gui_window_add_slider(int id, int sid){
    gui::gui_windows[id].child_sliders.push_back(sid);
    gui::gui_sliders[sid].parent_id = id;
  }

  void gui_window_add_label(int id, int sid){
    gui::gui_windows[id].child_labels.push_back(sid);
    gui::gui_labels[sid].parent_id = id;
  }
}
