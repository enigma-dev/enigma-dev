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

#include <algorithm> //For std::remove
#include <unordered_map>
#include <string>
#include <deque>
using std::string;
using std::unordered_map;
using std::pair;
using std::deque;

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
#include "scrollbars.h"
#include "labels.h"

namespace gui
{
  bool windowStopPropagation = false; //Stop event propagation in windows and between
	unordered_map<unsigned int, gui_window> gui_windows;
	deque<unsigned int> gui_window_order; //This allows changing rendering order (like depth)

	unsigned int gui_windows_maxid = 0;
	extern unsigned int gui_style_window;

  extern unordered_map<unsigned int, gui_style> gui_styles;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
	extern unordered_map<unsigned int, gui_button> gui_buttons;
	extern unordered_map<unsigned int, gui_toggle> gui_toggles;
  extern unordered_map<unsigned int, gui_slider> gui_sliders;
  extern unordered_map<unsigned int, gui_scrollbar> gui_scrollbars;
  extern unordered_map<unsigned int, gui_label> gui_labels;
	extern unsigned int gui_skins_maxid;

	//Implements button class
	gui_window::gui_window(){
    style_id = gui_style_window; //Default style
    enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
	}

  void gui_window::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, true, state, event);
    }
	}

	//Update all possible button states (hover, click, toggle etc.)
	void gui_window::update(gs_scalar tx, gs_scalar ty){
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){ //Press
      if (windowStopPropagation == false && box.point_inside(tx,ty)){
        state = enigma_user::gui_state_on;
        callback_execute(enigma_user::gui_event_pressed);
        if (draggable == true){
          drag = true;
          drag_xoffset = tx-box.x;
          drag_yoffset = ty-box.y;
        }
      }else{
        state = enigma_user::gui_state_default;
      }
    }

    if (windowStopPropagation == false && box.point_inside(tx,ty)){ //Hover or resize
      if (resizable == true){
        rect box_left(box.x,box.y,20,box.h);
        rect box_top(box.x,box.y,box.w,20);
        rect box_right(box.w+box.x-20,box.y,20,box.h);
        rect box_bottom(box.x,box.y+box.h-20,box.w,20);

        //Resize
        if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left) && (box_left.point_inside(tx,ty) || box_top.point_inside(tx,ty) || box_right.point_inside(tx,ty) || box_bottom.point_inside(tx,ty))){ //Press
          if (box_left.point_inside(tx,ty)) { if (!box_top.point_inside(tx,ty)) { resize_side = 0; } else { resize_side = 1; } }
          else if (box_top.point_inside(tx,ty)) { if (!box_right.point_inside(tx,ty)) { resize_side = 2; } else { resize_side = 3; } }
          else if (box_right.point_inside(tx,ty)) { if (!box_bottom.point_inside(tx,ty)) { resize_side = 4; } else { resize_side = 5; } }
          else if (box_bottom.point_inside(tx,ty)) { if (!box_left.point_inside(tx,ty)) { resize_side = 6; } else { resize_side = 7; } }
          drag_xoffset = tx-box.x;
          drag_yoffset = ty-box.y;
          resize_xoffset = tx;
          resize_yoffset = ty;
          resize_width = box.w;
          resize_height = box.h;
          resize = true;
          drag = false;
        }
      }else{
        callback_execute(enigma_user::gui_event_hover);
        windowStopPropagation = true;
      }
    }

    if (resize == true){
      windowStopPropagation = true;
      switch (resize_side){
        case 0: { //Resizing left side
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.x = tx-drag_xoffset;
        } break;
        case 1: { //Resizing top-left
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.x = tx-drag_xoffset;
          box.y = ty-drag_yoffset;
        } break;
        case 2: { //Resizing top
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.y = ty-drag_yoffset;
        } break;
        case 3: { //Resizing top-right
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (ty - resize_yoffset), min_box.h);
          box.y = ty-drag_yoffset;
        } break;
        case 4: { //Resizing right side
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
        } break;
        case 5: { //Resizing bottom-right
          box.w = fmax(resize_width + (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
        } break;
        case 6: { //Resizing top
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
        } break;
        case 7: { //Resizing top-left
          box.w = fmax(resize_width - (tx - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (ty - resize_yoffset), min_box.h);
          box.x = tx-drag_xoffset;
        } break;
      }
      callback_execute(enigma_user::gui_event_resize);
      if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				resize = false;
				callback_execute(enigma_user::gui_event_released);
			}
    }else if (drag == true){
      windowStopPropagation = true;
			box.x = tx-drag_xoffset;
			box.y = ty-drag_yoffset;
			callback_execute(enigma_user::gui_event_drag);
			if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				drag = false;
				callback_execute(enigma_user::gui_event_released);
			}
		}
	}

	void gui_window::draw(){
	  //Draw window
    if (gui::gui_styles[style_id].sprites[state] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[state],-1,
                                      gui::gui_styles[style_id].border.left,
                                      gui::gui_styles[style_id].border.top,
                                      gui::gui_styles[style_id].border.right,
                                      gui::gui_styles[style_id].border.bottom,
                                      box.x,
                                      box.y,
                                      box.x+box.w,
                                      box.y+box.h,
                                      gui::gui_styles[style_id].sprite_styles[state].color,
                                      gui::gui_styles[style_id].sprite_styles[state].alpha);
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
		gui::gui_window_order.push_back(gui::gui_windows_maxid);
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
		gui::gui_window_order.push_back(gui::gui_windows_maxid);
		return gui::gui_windows_maxid++;
	}

	void gui_window_destroy(int id){
		gui::gui_windows.erase(gui::gui_windows.find(id));
		//This is the fancy remove/erase idiom, which is the fastest way I know how to delete an element by value from vector
		gui::gui_window_order.erase(std::remove(gui::gui_window_order.begin(), gui::gui_window_order.end(), id), gui::gui_window_order.end());
	}

	void gui_window_set_state(int id, int state){
    gui::gui_windows[id].state = state;
	}

  ///Setters
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

	void gui_window_set_callback(int id, int event, int script_id){
    if (event == enigma_user::gui_event_all){
      gui::gui_windows[id].callback.fill(script_id);
	  }else{
      gui::gui_windows[id].callback[event] = script_id;
	  }
  }

  void gui_window_set_style(int id, int style_id){
    gui::gui_windows[id].style_id = (style_id != -1? style_id : gui::gui_style_window);
  }

	void gui_window_set_visible(int id, bool visible){
		gui::gui_windows[id].visible = visible;
	}

  void gui_window_set_draggable(int id, bool draggable){
		gui::gui_windows[id].draggable = draggable;
	}

  void gui_window_set_resizable(int id, bool resizable){
		gui::gui_windows[id].resizable = resizable;
	}

	///Getters
  int gui_window_get_style(int id){
    return gui::gui_windows[id].style_id;
  }

	int gui_window_get_state(int id){
		return gui::gui_windows[id].state;
	}

	int gui_window_get_callback(int id, int event){
		return gui::gui_windows[id].callback[event];
	}

  bool gui_window_get_draggable(int id){
    return gui::gui_windows[id].draggable;
  }

  bool gui_window_get_resizable(int id){
    return gui::gui_windows[id].resizable;
  }

	bool gui_window_get_visible(int id){
	  return gui::gui_windows[id].visible;
	}

  gs_scalar gui_window_get_width(int id){
    return gui::gui_windows[id].box.w;
  }

	gs_scalar gui_window_get_height(int id){
    return gui::gui_windows[id].box.h;
	}

	gs_scalar gui_window_get_x(int id){
    return gui::gui_windows[id].box.x;
	}

	gs_scalar gui_window_get_y(int id){
	  return gui::gui_windows[id].box.y;
	}

	string gui_window_get_text(int id){
    return gui::gui_windows[id].text;
	}

	///Depth changers
	void gui_window_push_to_front(int id){
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_back(id);
    }
	}

  void gui_window_push_to_back(int id){
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_front(id);
    }
	}

  ///Drawers
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
	  if (gui::gui_window_order.size() == 0) return;

		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		gui::windowStopPropagation = false;

    //Update loop in reverse direction
    bool window_click = false; //Something clicked in the window (or the window itself)?
    int window_swap_id = -1;
    for (int ind = gui::gui_window_order.size()-1; ind >= 0; --ind){
      int i = gui::gui_window_order[ind];
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
        if (gui::gui_windows[i].child_scrollbars.empty() == false){
          for (int b=gui::gui_windows[i].child_scrollbars.size()-1; b>=0; --b){
            if (gui::gui_scrollbars[gui::gui_windows[i].child_scrollbars[b]].visible == false) continue;
            gui::gui_scrollbars[gui::gui_windows[i].child_scrollbars[b]].update(gui::gui_windows[i].box.x,gui::gui_windows[i].box.y);
          }
        }
        //This checks if any of the inside elements are pressed
        if (gui::windowStopPropagation == true && window_click == false) { window_click = true; window_swap_id = ind; }
        gui::gui_windows[i].update();
        //This checks for the click on the window itself. I cannot just check window propagation, because hover also stops propagation, but should not change the draw order
        if (gui::windowStopPropagation == true && window_click == false) { window_click = true; if (gui::gui_windows[i].state == enigma_user::gui_state_on) { window_swap_id = ind; } }
      }
		}
		//printf("Window selected = %i and click = %i and size = %i && mouse_check_button_pressed %i\n", window_swap_id, window_click, gui::gui_window_order.size(), enigma_user::mouse_check_button_pressed(enigma_user::mb_left));
		//I REALLY HATE THE MOUSE CHECK HERE :( - Harijs
		if (window_click == true && enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) { //Push to front
        int t = gui::gui_window_order[window_swap_id]; //Get the id of the clicked window
        gui::gui_window_order.erase(gui::gui_window_order.begin()+window_swap_id); //Delete the id from it's current place
        gui::gui_window_order.push_back(t); //put on top
    }

    //Draw loop
    for (auto &wi : gui::gui_window_order){
      auto &w = gui::gui_windows[wi];
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
        if (w.child_scrollbars.empty() == false){
          for (const auto &s : w.child_scrollbars){
            if (gui::gui_scrollbars[s].visible == true) gui::gui_scrollbars[s].draw(w.box.x,w.box.y);
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

  void gui_window_add_scrollbar(int id, int sid){
    gui::gui_windows[id].child_scrollbars.push_back(sid);
    gui::gui_scrollbars[sid].parent_id = id;
  }

  void gui_window_add_label(int id, int lid){
    gui::gui_windows[id].child_labels.push_back(lid);
    gui::gui_labels[lid].parent_id = id;
  }

  void gui_window_remove_button(int id, int bid){
    auto it = find(gui::gui_windows[id].child_buttons.begin(), gui::gui_windows[id].child_buttons.end(), bid);
    if (it != gui::gui_windows[id].child_buttons.end()){
      gui::gui_windows[id].child_buttons.erase(it);
      gui::gui_buttons[bid].parent_id = -1;
    }
  }

  void gui_window_remove_toggle(int id, int tid){
    auto it = find(gui::gui_windows[id].child_toggles.begin(), gui::gui_windows[id].child_toggles.end(), tid);
    if (it != gui::gui_windows[id].child_toggles.end()){
      gui::gui_windows[id].child_toggles.erase(it);
      gui::gui_toggles[tid].parent_id = -1;
    }
  }

  void gui_window_remove_slider(int id, int sid){
    auto it = find(gui::gui_windows[id].child_sliders.begin(), gui::gui_windows[id].child_sliders.end(), sid);
    if (it != gui::gui_windows[id].child_sliders.end()){
      gui::gui_windows[id].child_sliders.erase(it);
      gui::gui_sliders[sid].parent_id = -1;
    }
  }

  void gui_window_remove_scrollbar(int id, int sid){
    auto it = find(gui::gui_windows[id].child_scrollbars.begin(), gui::gui_windows[id].child_scrollbars.end(), sid);
    if (it != gui::gui_windows[id].child_scrollbars.end()){
      gui::gui_windows[id].child_scrollbars.erase(it);
      gui::gui_scrollbars[sid].parent_id = -1;
    }
  }

  void gui_window_remove_label(int id, int lid){
    auto it = find(gui::gui_windows[id].child_labels.begin(), gui::gui_windows[id].child_labels.end(), lid);
    if (it != gui::gui_windows[id].child_labels.end()){
      gui::gui_windows[id].child_labels.erase(it);
      gui::gui_labels[lid].parent_id = -1;
    }
  }

  int gui_window_get_button_count(int id){
    return gui::gui_windows[id].child_buttons.size();
  }

  int gui_window_get_toggle_count(int id){
    return gui::gui_windows[id].child_toggles.size();
  }

  int gui_window_get_slider_count(int id){
    return gui::gui_windows[id].child_sliders.size();
  }

  int gui_window_get_label_count(int id){
    return gui::gui_windows[id].child_labels.size();
  }

  int gui_window_get_button(int id, int but){
    return gui::gui_windows[id].child_buttons[but];
  }

  int gui_window_get_toggle(int id, int tog){
    return gui::gui_windows[id].child_toggles[tog];
  }

  int gui_window_get_slider(int id, int sli){
    return gui::gui_windows[id].child_sliders[sli];
  }

  int gui_window_get_scrollbar(int id, int scr){
    return gui::gui_windows[id].child_scrollbars[scr];
  }

  int gui_window_get_label(int id, int lab){
    return gui::gui_windows[id].child_labels[lab];
  }
}
