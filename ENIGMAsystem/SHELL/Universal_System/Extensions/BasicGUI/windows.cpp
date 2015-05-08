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
#include <map>
#include <deque>
using std::string;
using std::unordered_map;
using std::map;
using std::deque;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
//#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Graphics_Systems/General/GSd3d.h" //For stencil stuff

#include "elements.h"
#include "styles.h"
#include "skins.h"
#include "windows.h"
#include "include.h"
#include "common.h"

//Children
/*
#include "buttons.h"
#include "toggles.h"
#include "sliders.h"
#include "scrollbars.h"
#include "labels.h"*/

namespace gui
{
  extern unsigned int gui_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;
  extern map<unsigned int, unsigned int> gui_element_order;

  bool windowStopPropagation = false; //Stop event propagation in windows and between
	deque<unsigned int> gui_window_order; //This allows changing rendering order (like depth)
  
	extern unsigned int gui_style_window;

	extern int gui_bound_skin;

	//Implements button class
	Window::Window(){
    style_id = gui_style_window; //Default style
    enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
	}

  void Window::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, true, state, event);
    }
	}

	//Update all possible button states (hover, click, toggle etc.)
	void Window::update(gs_scalar tx, gs_scalar ty){
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

	void Window::draw(gs_scalar ox, gs_scalar oy){
	  //Draw window
    get_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    if (sty.sprites[state] != -1){
      enigma_user::draw_sprite_padded(sty.sprites[state],-1,
                                      sty.border.left,
                                      sty.border.top,
                                      sty.border.right,
                                      sty.border.bottom,
                                      box.x,
                                      box.y,
                                      box.x+box.w,
                                      box.y+box.h,
                                      sty.sprite_styles[state].color,
                                      sty.sprite_styles[state].alpha);
		}

		//Draw text
		sty.font_styles[state].use();

		gs_scalar textx = 0.0, texty = 0.0;
    switch (sty.font_styles[state].halign){
      case enigma_user::fa_left: textx = box.x+sty.padding.left; break;
      case enigma_user::fa_center: textx = box.x+box.w/2.0; break;
      case enigma_user::fa_right: textx = box.x+box.w-sty.padding.right; break;
    }

    switch (sty.font_styles[state].valign){
      case enigma_user::fa_top: texty = box.y+sty.padding.top; break;
      case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
      case enigma_user::fa_bottom: texty = box.y+box.h-sty.padding.bottom; break;
    }

		enigma_user::draw_text(textx,texty,text);
	}

	void Window::update_text_pos(int state){
	}
}

namespace enigma_user
{
	int gui_window_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Window());
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.window_style]);
		}
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
		win.visible = true;
		win.id = gui::gui_elements_maxid;
    gui::gui_window_order.emplace_back(gui::gui_elements_maxid);
		gui::gui_element_order.emplace(gui::gui_elements_maxid, gui::gui_elements_maxid);
		return gui::gui_elements_maxid++;
	}

	int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Window());
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.window_style]);
      printf("Creating window with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
		}
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
		win.visible = true;
		win.id = gui::gui_elements_maxid;
		win.box.set(x, y, w, h);
		win.text = text;
		win.update_text_pos();
    gui::gui_window_order.emplace_back(gui::gui_elements_maxid);
		gui::gui_element_order.emplace(gui::gui_elements_maxid, gui::gui_elements_maxid);
		return gui::gui_elements_maxid++;
	}

	void gui_window_destroy(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
		gui::gui_elements.erase(gui::gui_elements.find(id));
		//This is the fancy remove/erase idiom, which is the fastest way I know how to delete an element by value from vector
		gui::gui_element_order.erase(id);
		gui::gui_window_order.erase(std::remove(gui::gui_window_order.begin(), gui::gui_window_order.end(), id), gui::gui_window_order.end());
	}

	void gui_window_set_state(int id, int state){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.state = state;
	}

  ///Setters
	void gui_window_set_text(int id, string text){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.text = text;
	}

	void gui_window_set_position(int id, gs_scalar x, gs_scalar y){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.box.x = x;
		win.box.y = y;
	}

	void gui_window_set_size(int id, gs_scalar w, gs_scalar h){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.box.w = w;
		win.box.h = h;
		win.update_text_pos();
	}

	void gui_window_set_callback(int id, int event, int script_id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    if (event == enigma_user::gui_event_all){
      win.callback.fill(script_id);
	  }else{
      win.callback[event] = script_id;
	  }
  }

  void gui_window_set_style(int id, int style_id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.style_id = (style_id != -1? style_id : gui::gui_style_window);
  }

	void gui_window_set_visible(int id, bool visible){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.visible = visible;
	}

  void gui_window_set_draggable(int id, bool draggable){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.draggable = draggable;
	}

  void gui_window_set_resizable(int id, bool resizable){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.resizable = resizable;
	}

  void gui_window_set_stencil_mask(int id, bool stencil){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		win.stencil_mask = stencil;
	}

	///Getters
  int gui_window_get_style(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.style_id;
  }

	int gui_window_get_state(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
		return win.state;
	}

	int gui_window_get_callback(int id, int event){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
		return win.callback[event];
	}

  bool gui_window_get_draggable(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
    return win.draggable;
  }

  bool gui_window_get_resizable(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
    return win.resizable;
  }

	bool gui_window_get_visible(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
	  return win.visible;
	}

  gs_scalar gui_window_get_width(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.w;
  }

	gs_scalar gui_window_get_height(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.h;
	}

	gs_scalar gui_window_get_x(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.box.x;
	}

	gs_scalar gui_window_get_y(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
	  return win.box.y;
	}

	string gui_window_get_text(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,"");
    return win.text;
	}

  bool gui_window_get_stencil_mask(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,false);
		return win.stencil_mask;
	}

	///Depth changers
	void gui_window_push_to_front(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_back(id);
    }
	}

  void gui_window_push_to_back(int id){
    check_element(gui::GUI_TYPE::WINDOW,id);
    auto it = find(gui::gui_window_order.begin(), gui::gui_window_order.end(), id);
    if (it != gui::gui_window_order.end()){
      gui::gui_window_order.erase(it);
      gui::gui_window_order.push_front(id);
    }
	}

  ///Drawers
	void gui_window_draw(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    win.update();
		win.draw(0,0);
		//Draw children
		///TODO(harijs) - This needs to implemented!
    /*if (gui::gui_windows[id].child_buttons.empty() == false){
      for (const auto &b : gui::gui_windows[id].child_buttons){
        gui::gui_buttons[b].update();
        gui::gui_buttons[b].draw();
      }
    }*/
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
      get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,i);
      if (win.visible == true){
        //Update children
        for (const auto& x: win.child_elements){
          //get_element_smart(ele, x);
          switch (gui::gui_elements[x].type){
            case gui::GUI_TYPE::BUTTON: { gui::Button &element = gui::gui_elements[x]; if (element.visible == true) element.update(win.box.x,win.box.y); break; }
            case gui::GUI_TYPE::TOGGLE: { gui::Toggle &element = gui::gui_elements[x]; if (element.visible == true) element.update(win.box.x,win.box.y); break; }
            case gui::GUI_TYPE::SCROLLBAR: { gui::Scrollbar &element = gui::gui_elements[x]; if (element.visible == true) element.update(win.box.x,win.box.y); break; }
            case gui::GUI_TYPE::SLIDER: { gui::Slider &element = gui::gui_elements[x]; if (element.visible == true) element.update(win.box.x,win.box.y); break; }
            case gui::GUI_TYPE::WINDOW: { gui::Window &element = gui::gui_elements[x]; if (element.visible == true) element.update(win.box.x,win.box.y); break; }
            case gui::GUI_TYPE::LABEL:
            case gui::GUI_TYPE::SKIN:
            case gui::GUI_TYPE::STYLE:
            case gui::GUI_TYPE::GROUP:
            case gui::GUI_TYPE::ERROR: { break; }
          }
        }
        
        //This checks if any of the inside elements are pressed
        if (gui::windowStopPropagation == true && window_click == false) { window_click = true; window_swap_id = ind; }
        win.update();
        //This checks for the click on the window itself. I cannot just check window propagation, because hover also stops propagation, but should not change the draw order
        if (gui::windowStopPropagation == true && window_click == false) { 
          window_click = true; if (win.state == enigma_user::gui_state_on) { window_swap_id = ind; }
        }
      }
		}
		//printf("Window selected = %i and click = %i and size = %i && mouse_check_button_pressed %i\n", window_swap_id, window_click, gui::gui_window_order.size(), enigma_user::mouse_check_button_pressed(enigma_user::mb_left));
		//I REALLY HATE THE MOUSE CHECK HERE :( - Harijs
		if (window_click == true && enigma_user::mouse_check_button_pressed(enigma_user::mb_left)) { //Push to front
        int t = gui::gui_window_order[window_swap_id]; //Get the id of the clicked window
        gui::gui_window_order.erase(gui::gui_window_order.begin()+window_swap_id); //Delete the id from its current place
        gui::gui_window_order.push_back(t); //put on top
    }

    //Draw loop
    for (const auto &wi : gui::gui_window_order){
      get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,wi);
      if (win.visible == true){
        //Stencil test
        win.draw(0,0);
        if (win.stencil_mask == true){
          enigma_user::d3d_stencil_start_mask();
          win.draw(0,0);
          enigma_user::d3d_stencil_use_mask();
        }

        //Draw children
        for (const auto& x: win.child_elements){
            switch (gui::gui_elements[x].type){
              case gui::GUI_TYPE::BUTTON: { gui::Button &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::TOGGLE: { gui::Toggle &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::LABEL:  { gui::Label &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::SCROLLBAR: { gui::Scrollbar &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::SLIDER: { gui::Slider &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::WINDOW: { gui::Window &element = gui::gui_elements[x]; if (element.visible == true) element.draw(win.box.x,win.box.y); break; }
              case gui::GUI_TYPE::SKIN:
              case gui::GUI_TYPE::STYLE:
              case gui::GUI_TYPE::GROUP:
              case gui::GUI_TYPE::ERROR: { break; }
            }
        
          //get_element_smart(ele, x);
          //if (ele.visible == true) ele.draw(w.box.x,w.box.y);
        }

        if (win.stencil_mask == true){
          enigma_user::d3d_stencil_end_mask();
        }
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

  void gui_window_add_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
    win.child_elements.push_back(bid);
    but.parent_id = id;
  }

  void gui_window_add_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
    win.child_elements.push_back(tid);
    tog.parent_id = id;
  }

  void gui_window_add_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,sid);
    win.child_elements.push_back(sid);
    sli.parent_id = id;
  }

  void gui_window_add_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,sid);
    win.child_elements.push_back(sid);
    scr.parent_id = id;
  }

  void gui_window_add_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element(lab,gui::Label,gui::GUI_TYPE::LABEL,lid);
    win.child_elements.push_back(lid);
    lab.parent_id = id;
  }

  void gui_window_remove_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    check_element(gui::GUI_TYPE::BUTTON,bid);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), bid);
    if (it != win.child_elements.end()){
      get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,bid);
      win.child_elements.erase(it);
      but.parent_id = -1;
    }
  }

  void gui_window_remove_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    check_element(gui::GUI_TYPE::TOGGLE,tid);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), tid);
    if (it != win.child_elements.end()){
      get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,tid);
      win.child_elements.erase(it);
      tog.parent_id = -1;
    }
  }

  void gui_window_remove_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    check_element(gui::GUI_TYPE::SLIDER,sid);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), sid);
    if (it != win.child_elements.end()){
      get_element(sli,gui::Slider,gui::GUI_TYPE::SLIDER,sid);
      win.child_elements.erase(it);
      sli.parent_id = -1;
    }
  }

  void gui_window_remove_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    check_element(gui::GUI_TYPE::SCROLLBAR,sid);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), sid);
    if (it != win.child_elements.end()){
      get_element(scr,gui::Scrollbar,gui::GUI_TYPE::SCROLLBAR,sid);
      win.child_elements.erase(it);
      scr.parent_id = -1;
    }
  }

  void gui_window_remove_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    check_element(gui::GUI_TYPE::LABEL,lid);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), lid);
    if (it != win.child_elements.end()){
      get_element(lab,gui::Label,gui::GUI_TYPE::LABEL,lid);
      win.child_elements.erase(it);
      lab.parent_id = -1;
    }
  }
  
  int gui_window_get_button_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    int c = 0;
    for (unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::BUTTON) c++;
    }
    return c;
  }
  
  int gui_window_get_toggle_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    int c = 0;
    for (unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::TOGGLE) c++;
    }
    return c;
  }
  
  int gui_window_get_slider_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    int c = 0;
    for (unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::SLIDER) c++;
    }
    return c;
  }
  
  int gui_window_get_scrollbar_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    int c = 0;
    for (unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::SCROLLBAR) c++;
    }
    return c;
  }
  
  int gui_window_get_label_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    int c = 0;
    for (unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::LABEL) c++;
    }
    return c;
  }

  ///GETTERS FOR ELEMENTS
  int gui_window_get_button(int id, int but){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    if (but<0 || unsigned(but) >= win.child_elements.size()) return -1;
    int c = 0;
    for (const unsigned int &e : win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::BUTTON){
        if (c == but){
          return e;
        }
        c++; 
      }
    }
    return -1;
  }
  
  int gui_window_get_toggle(int id, int tog){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    if (tog<0 || unsigned(tog) >= win.child_elements.size()) return -1;
    int c = 0;
    for (const unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::TOGGLE){
        if (c == tog){
          return e;
        }
        c++; 
      }
    }
    return -1;
  }
  
  int gui_window_get_slider(int id, int sli){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    if (sli<0 || unsigned(sli) >= win.child_elements.size()) return -1;
    int c = 0;
    for (const unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::SLIDER){
        if (c == sli){
          return e;
        }
        c++; 
      }
    }
    return -1;
  }
  
  int gui_window_get_scrollbar(int id, int scr){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    if (scr<0 || unsigned(scr) >= win.child_elements.size()) return -1;
    int c = 0;
    for (const unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::SCROLLBAR){
        if (c == scr){
          return e;
        }
        c++; 
      }
    }
    return -1;
  }
  
  int gui_window_get_label(int id, int lab){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    if (lab<0 || unsigned(lab) >= win.child_elements.size()) return -1;
    int c = 0;
    for (const unsigned int &e: win.child_elements){
      check_element_existsv(e, -1);
      if (gui::gui_elements[e].type == gui::GUI_TYPE::LABEL){
        if (c == lab){
          return e;
        }
        c++; 
      }
    }
    return -1;
  }

  
  /*void gui_window_add_element(int id, int ele){ //This adds a generic element. No type checking is done, so be careful using this!
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    get_element_smart(el,ele);
    if (gui_elements[ele].type == gui::GUI_TYPE::WINDOW || gui_elements[ele].type == gui::GUI_TYPE::GROUP ||
        gui_elements[ele].type == gui::GUI_TYPE::SKIN   || gui_elements[ele].type == gui::GUI_TYPE::STYLE){
      return; //We cannot add these kinds of elements
    }
    win.child_elements.push_back(ele);
    el.parent_id = id;
  }

  void gui_window_remove_element(int id, int ele){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    auto it = find(win.child_elements.begin(), win.child_elements.end(), ele);
    if (it != win.child_elements.end()){
      win.child_elements.erase(it);
      get_element_smart(el,ele);
      el.parent_id = -1;
    }
  }*/
  
  int gui_window_get_element_count(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    return win.child_elements.size();
  }

  int gui_window_get_element(int id, int ele){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    if (ele<0 || unsigned(ele) >= win.child_elements.size()) return -1;
    check_element_exists(win.child_elements[ele]);    
    return win.child_elements[ele];
  }
}
