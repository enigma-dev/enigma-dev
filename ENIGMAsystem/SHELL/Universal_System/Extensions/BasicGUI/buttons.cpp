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

#include <unordered_map>
#include <string>
using std::string;
using std::unordered_map;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button
#include "Universal_System/resource_data.h" //For script_execute
//#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "elements.h"
#include "styles.h"
#include "skins.h"
#include "groups.h"
#include "toggles.h"
#include "buttons.h"
#include "windows.h"
#include "include.h"
#include "common.h"

namespace gui
{
  extern unsigned int gui_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;

	extern int gui_bound_skin;
	extern unsigned int gui_style_button;

	extern bool windowStopPropagation;

	//Implements button class
	Button::Button(){
	  style_id = gui_style_button; //Default style
	  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
	}

	void Button::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, active, state, event);
    }
	}

	//Update all possible button states (hover, click, toggle etc.)
	void Button::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,parent_id);
	  bool pacheck = (parent_id == -1 || (parent_id != -1 && (win.stencil_mask == false || win.box.point_inside(tx,ty))));
		if (box.point_inside(tx-ox,ty-oy) && gui::windowStopPropagation == false && pacheck == true){
      callback_execute(enigma_user::gui_event_hover);
      gui::windowStopPropagation = true;
			if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        if (active == false){
          state = enigma_user::gui_state_active;
        }else{
          state = enigma_user::gui_state_on_active;
        }
        callback_execute(enigma_user::gui_event_pressed);
			}else{
				if (state != enigma_user::gui_state_active && state != enigma_user::gui_state_on_active){
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
            callback_execute(enigma_user::gui_event_released);
						if (togglable == false){
							active = false;
						}

						if (active == false){
							state = enigma_user::gui_state_hover;
						}else{
							state = enigma_user::gui_state_on_hover;
							if (group_id != -1){ //Groups disable any other active element
                get_element(gro,gui::Group,gui::GUI_TYPE::GROUP,group_id);
                for (const auto &b : gro.group_buttons){
                  if (b != id) {
                    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,b);
                    but.active = false;
                  }
                }
                for (const auto &t : gro.group_toggles){
                  get_element(tog,gui::Toggle,gui::GUI_TYPE::TOGGLE,t);
                  tog.active = false;
                }
							}
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

	void Button::draw(gs_scalar ox, gs_scalar oy){
		//Draw button
    get_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    if (sty.sprites[state] != -1){
      enigma_user::draw_sprite_padded(sty.sprites[state],-1,
                                      sty.border.left,
                                      sty.border.top,
                                      sty.border.right,
                                      sty.border.bottom,
                                      ox + box.x,
                                      oy + box.y,
                                      ox + box.x+box.w,
                                      oy + box.y+box.h,
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

		enigma_user::draw_text(ox + textx,oy + texty,text);
	}

	void Button::update_text_pos(int state){
	  //gui::gui_styles[style_id].update_text_pos(box, state);
	}
}

namespace enigma_user
{
	int gui_button_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Button());
      printf("Creating default button with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.button_style]);
      printf("Creating button with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
		}
		gui::Button &b = gui::gui_elements[gui::gui_elements_maxid];
		b.visible = true;
		b.id = gui::gui_elements_maxid;
		return (gui::gui_elements_maxid++);
	}

	int gui_button_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::Button());
		}else{
      get_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
			gui::gui_elements.emplace(gui::gui_elements_maxid, gui::gui_elements[ski.button_style]);
		}
    gui::Button &b = gui::gui_elements[gui::gui_elements_maxid];
		b.visible = true;
		b.id = gui::gui_elements_maxid;
		b.box.set(x, y, w, h);
		b.text = text;
		b.update_text_pos();
		return (gui::gui_elements_maxid++);
	}

	void gui_button_destroy(int id){
	  get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
    if (but.parent_id != -1){
      gui_window_remove_button(but.parent_id, id);
	  }
		gui::gui_elements.erase(gui::gui_elements.find(id));
	}

  ///Setters
	void gui_button_set_text(int id, string text){
	  get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.text = text;
	}

	void gui_button_set_position(int id, gs_scalar x, gs_scalar y){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.box.x = x;
		but.box.y = y;
	}

  void gui_button_set_size(int id, gs_scalar w, gs_scalar h){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.box.w = w;
		but.box.h = h;
		but.update_text_pos();
	}

  void gui_button_set_callback(int id, int event, int script_id){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
    if (event == enigma_user::gui_event_all){
      but.callback.fill(script_id);
	  }else{
      but.callback[event] = script_id;
	  }
	}

  void gui_button_set_style(int id, int style_id){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
    but.style_id = (style_id != -1? style_id : gui::gui_style_button);
  }

  void gui_button_set_active(int id, bool active){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.active = active;
	}

	void gui_button_set_togglable(int id, bool togglable){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.togglable = togglable;
	}

	void gui_button_set_visible(int id, bool visible){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		but.visible = visible;
	}

	///Getters
  int gui_button_get_style(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.style_id;
  }

	int gui_button_get_state(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
		return but.state;
	}

	bool gui_button_get_active(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,false);
		return but.active;
	}

  bool gui_button_get_togglable(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,false);
    return but.togglable;
  }

	bool gui_button_get_visible(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,false);
    return but.visible;
	}

  int gui_button_get_callback(int id, int event){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.callback[event];
  }

  int gui_button_get_parent(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.parent_id;
  }

  gs_scalar gui_button_get_width(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.box.w;
  }

  gs_scalar gui_button_get_height(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.box.h;
  }

	gs_scalar gui_button_get_x(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.box.x;
  }

	gs_scalar gui_button_get_y(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return but.box.y;
  }

  string gui_button_get_text(int id){
    get_elementv(but,gui::Button,gui::GUI_TYPE::BUTTON,id,"");
    return but.text;
  }

  ///Drawers
	void gui_button_draw(int id){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
    but.update();
		but.draw();
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
		for (auto &b : gui::gui_elements){
		  ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
      if (b.second.type == gui::GUI_TYPE::BUTTON){
        get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,b.first);
        if (but.visible == true && but.parent_id == -1){
          but.update();
          but.draw();
        }
      }
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}
}

