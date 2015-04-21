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
#include "scrollbars.h"
#include "include.h"
#include "common.h"

namespace gui
{
  unordered_map<unsigned int, gui_scrollbar> gui_scrollbars;
	unsigned int gui_scrollbars_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
  extern unordered_map<unsigned int, gui_style> gui_styles;
	extern unsigned int gui_skins_maxid;
  extern unsigned int gui_style_scrollbar;

	extern bool windowStopPropagation; //This stops event propagation between window elements

	//Implements scrollbar class
	gui_scrollbar::gui_scrollbar(){
    style_id = gui_style_scrollbar; //Default style
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
	}

  void gui_scrollbar::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, active, state, event);
    }
	}

	void gui_scrollbar::update_spos(){
      if (direction == false){
        if (indicator_box.w != box.w){
          scroll_offset = fmin(scroll_offset, box.w-indicator_box.w);
          value = round((minValue + scroll_offset/(box.w-indicator_box.w) * (maxValue-minValue)) / incValue) * incValue;
          scroll_offset = (box.w-indicator_box.w)*((value-minValue)/(maxValue-minValue));
        }
      }else{
        if (indicator_box.h != box.h){
          scroll_offset = fmin(scroll_offset, box.h-indicator_box.h);
          value = round((minValue + scroll_offset/(box.h-indicator_box.h) * (maxValue-minValue)) / incValue) * incValue;
          scroll_offset = (box.h-indicator_box.h)*((value-minValue)/(maxValue-minValue));
        }
      }
	}

	//Update all possible scrollbar states (hover, click etc.)
	void gui_scrollbar::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
		if ((box.point_inside(tx-ox,ty-oy) || indicator_box.point_inside(tx-ox-box.x-scroll_offset-indicator_box.x,ty-box.y-oy-indicator_box.y)) && gui::windowStopPropagation == false){
      callback_execute(enigma_user::gui_event_hover);
      windowStopPropagation = true;
			if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        state = enigma_user::gui_state_active;
        active = true;
        drag = true;

        if (direction == false){
          if (indicator_box.point_inside(tx-ox-box.x-indicator_box.x-scroll_offset,ty-oy-box.y-indicator_box.y)){ //We press on the indicator itself
              drag_offset = tx-ox-indicator_box.x-box.x-scroll_offset;
          }else{ //We press on the whole widget box, so the indicator must jump
              drag_offset = indicator_box.w/2.0; //Center it on mouse otherwise
          }
        }else{
          if (indicator_box.point_inside(tx-ox-box.x-indicator_box.x,ty-oy-box.y-indicator_box.y-scroll_offset)){ //We press on the indicator itself
              drag_offset = ty-oy-indicator_box.y-box.y-scroll_offset;
          }else{ //We press on the whole widget box, so the indicator must jump
              drag_offset = indicator_box.h/2.0; //Center it on mouse otherwise
          }
        }
        callback_execute(enigma_user::gui_event_pressed);
      }else{
				if (state != enigma_user::gui_state_active){
					state = enigma_user::gui_state_hover;
				}else{
					if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
						active = false;
						state = enigma_user::gui_state_hover;
					}
				}
			}
		}else{
      state = enigma_user::gui_state_default;
		}
		if (drag == true){
      windowStopPropagation = true;

      if (direction == false){
        if (indicator_box.w != box.w){
          scroll_offset = fmin(fmax(0,tx-box.x-ox-drag_offset), box.w-indicator_box.w);
          value = round((minValue + scroll_offset/(box.w-indicator_box.w) * (maxValue-minValue)) / incValue) * incValue;
          scroll_offset = (box.w-indicator_box.w)*((value-minValue)/(maxValue-minValue));
        }else{
          value = 1.0;
        }
      }else{
        if (indicator_box.h != box.h){
          scroll_offset = fmin(fmax(0,ty-box.y-oy-drag_offset), box.h-indicator_box.h);
          value = round((minValue + scroll_offset/(box.h-indicator_box.h) * (maxValue-minValue)) / incValue) * incValue;
          scroll_offset = (box.h-indicator_box.h)*((value-minValue)/(maxValue-minValue));
        }else{
          value = 1.0;
        }
      }
      callback_execute(enigma_user::gui_event_drag);

			if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
				drag = false;
				callback_execute(enigma_user::gui_event_released);
			}
		}
	}

	void gui_scrollbar::draw(gs_scalar ox, gs_scalar oy){
		//Draw scrollbar and indicator
    if (gui::gui_styles[style_id].sprites[state] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[state],-1,
                                      gui::gui_styles[style_id].border.left,
                                      gui::gui_styles[style_id].border.top,
                                      gui::gui_styles[style_id].border.right,
                                      gui::gui_styles[style_id].border.bottom,
                                      ox + box.x,
                                      oy + box.y,
                                      ox + box.x+box.w,
                                      oy + box.y+box.h,
                                      gui::gui_styles[style_id].sprite_styles[state].color,
                                      gui::gui_styles[style_id].sprite_styles[state].alpha);
		}

    if (gui::gui_styles[indicator_style_id].sprites[state] != -1){
      auto &ist = gui::gui_styles[indicator_style_id];
      if (direction == false){
        enigma_user::draw_sprite_padded(ist.sprites[state],-1,
                                        ist.border.left,
                                        ist.border.top,
                                        ist.border.right,
                                        ist.border.bottom,
                                        ist.image_offset.x + ox + box.x + scroll_offset + indicator_box.x,
                                        ist.image_offset.y + oy + box.y + indicator_box.y,
                                        ist.image_offset.x + ox + box.x + indicator_box.w + scroll_offset + indicator_box.x,
                                        ist.image_offset.y + oy + box.y+indicator_box.h + indicator_box.y,
                                        ist.sprite_styles[state].color,
                                        ist.sprite_styles[state].alpha);
      }else{
        enigma_user::draw_sprite_padded(ist.sprites[state],-1,
                                        ist.border.left,
                                        ist.border.top,
                                        ist.border.right,
                                        ist.border.bottom,
                                        ist.image_offset.x + ox + box.x + indicator_box.x,
                                        ist.image_offset.y + oy + box.y + scroll_offset + indicator_box.y,
                                        ist.image_offset.x + ox + box.x + indicator_box.w + indicator_box.x,
                                        ist.image_offset.y + oy + box.y+indicator_box.h + scroll_offset + indicator_box.y,
                                        ist.sprite_styles[state].color,
                                        ist.sprite_styles[state].alpha);
      }
    }
	}
}

namespace enigma_user
{
	int gui_scrollbar_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_scrollbars.insert(pair<unsigned int, gui::gui_scrollbar >(gui::gui_scrollbars_maxid, gui::gui_scrollbar()));
		}else{
			gui::gui_scrollbars.insert(pair<unsigned int, gui::gui_scrollbar >(gui::gui_scrollbars_maxid, gui::gui_scrollbars[gui::gui_skins[gui::gui_bound_skin].scrollbar_style]));
		}
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].visible = true;
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].id = gui::gui_scrollbars_maxid;
		return (gui::gui_scrollbars_maxid++);
	}

	int gui_scrollbar_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y, bool direction, double val, double minVal, double maxVal, double incrVal, gs_scalar size){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_scrollbars.insert(pair<unsigned int, gui::gui_scrollbar >(gui::gui_scrollbars_maxid, gui::gui_scrollbar()));
		}else{
			gui::gui_scrollbars.insert(pair<unsigned int, gui::gui_scrollbar >(gui::gui_scrollbars_maxid, gui::gui_scrollbars[gui::gui_skins[gui::gui_bound_skin].scrollbar_style]));
		}
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].visible = true;
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].id = gui::gui_scrollbars_maxid;
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].box.set(x, y, w, h);
    gui::gui_scrollbars[gui::gui_scrollbars_maxid].minValue = minVal;
		gui::gui_scrollbars[gui::gui_scrollbars_maxid].maxValue = maxVal;
    gui::gui_scrollbars[gui::gui_scrollbars_maxid].size = size;
    gui::gui_scrollbars[gui::gui_scrollbars_maxid].value = val;
    gui::gui_scrollbars[gui::gui_scrollbars_maxid].direction = direction;

    if (direction == false){
      gui::gui_scrollbars[gui::gui_scrollbars_maxid].indicator_box.set(ind_x, ind_y, size*w, h);
    }else{
      gui::gui_scrollbars[gui::gui_scrollbars_maxid].indicator_box.set(ind_x, ind_y, w, size*h);
    }

    if (incrVal == 0){
      gui::gui_scrollbars[gui::gui_scrollbars_maxid].incValue = 1.0/(direction == false?w:h);
    }else{
      gui::gui_scrollbars[gui::gui_scrollbars_maxid].incValue = incrVal;
    }

		gui::gui_scrollbars[gui::gui_scrollbars_maxid].update_spos();
		return (gui::gui_scrollbars_maxid++);
	}

	void gui_scrollbar_destroy(int id){
    if (gui::gui_scrollbars[id].parent_id != -1){
      gui_window_remove_scrollbar(gui::gui_scrollbars[id].parent_id, id);
	  }
		gui::gui_scrollbars.erase(gui::gui_scrollbars.find(id));
	}

  ///Setters
	void gui_scrollbar_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_scrollbars[id].box.x = x;
		gui::gui_scrollbars[id].box.y = y;
	}

	void gui_scrollbar_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_scrollbars[id].box.w = w;
		gui::gui_scrollbars[id].box.h = h;
		if (gui::gui_scrollbars[id].direction == false){
      gui::gui_scrollbars[id].indicator_box.w = gui::gui_scrollbars[id].size*w;
		}else{
      gui::gui_scrollbars[id].indicator_box.h = gui::gui_scrollbars[id].size*h;
		}
    gui_scrollbar_set_incvalue(id, gui_scrollbar_get_incvalue(id));
	}

	void gui_scrollbar_set_callback(int id, int event, int script_id){
    if (event == enigma_user::gui_event_all){
      gui::gui_scrollbars[id].callback.fill(script_id);
	  }else{
      gui::gui_scrollbars[id].callback[event] = script_id;
	  }
  }

  void gui_scrollbar_set_style(int id, int style_id){
    gui::gui_scrollbars[id].style_id = (style_id != -1? style_id : gui::gui_style_scrollbar);
  }

  void gui_scrollbar_set_indicator_style(int id, int style_id){
    gui::gui_scrollbars[id].indicator_style_id = (style_id != -1? style_id : gui::gui_style_scrollbar);
  }

  void gui_scrollbar_set_visible(int id, bool visible){
		gui::gui_scrollbars[id].visible = visible;
	}

  void gui_scrollbar_set_active(int id, bool active){
		gui::gui_scrollbars[id].active = active;
	}

  void gui_scrollbar_set_value(int id, double value){
		gui::gui_scrollbars[id].value = value;
    gui::gui_scrollbars[id].update_spos();
  }

  void gui_scrollbar_set_minvalue(int id, double minvalue){
    gui::gui_scrollbars[id].minValue = minvalue;
    gui::gui_scrollbars[id].update_spos();
  }

  void gui_scrollbar_set_maxvalue(int id, double maxvalue){
    gui::gui_scrollbars[id].maxValue = maxvalue;
    gui::gui_scrollbars[id].update_spos();
  }

  void gui_scrollbar_set_incvalue(int id, double incvalue){
    if (incvalue == 0){
      gui::gui_scrollbars[id].incValue = 1.0/(gui::gui_scrollbars[id].direction == false?gui::gui_scrollbars[id].box.w:gui::gui_scrollbars[id].box.h);
    }else{
      gui::gui_scrollbars[id].incValue = incvalue;
    }
    gui::gui_scrollbars[id].update_spos();
  }

  void gui_scrollbar_set_indicator_size(int id, gs_scalar size){
    size = (size < 0.0 ? 0.0 : (size > 1.0 ? 1.0 : size));
    gui::gui_scrollbars[id].size = size;
		if (gui::gui_scrollbars[id].direction == false){
      gui::gui_scrollbars[id].indicator_box.w = gui::gui_scrollbars[id].size*gui::gui_scrollbars[id].box.w;
		}else{
      gui::gui_scrollbars[id].indicator_box.h = gui::gui_scrollbars[id].size*gui::gui_scrollbars[id].box.h;
		}
		gui::gui_scrollbars[id].update_spos();
  }

  void gui_scrollbar_set_direction(int id, bool direction){
		gui::gui_scrollbars[id].direction = direction;
	}

  ///Getters
  int gui_scrollbar_get_style(int id){
    return gui::gui_scrollbars[id].style_id;
  }

  int gui_scrollbar_get_indicator_style(int id){
    return gui::gui_scrollbars[id].indicator_style_id;
  }

	int gui_scrollbar_get_state(int id){
		return gui::gui_scrollbars[id].state;
	}

  int gui_scrollbar_get_callback(int id, int event){
    return gui::gui_scrollbars[id].callback[event];
  }

	bool gui_scrollbar_get_active(int id){
		return gui::gui_scrollbars[id].active;
	}

	bool gui_scrollbar_get_visible(int id){
		return gui::gui_scrollbars[id].visible;
	}

	gs_scalar gui_scrollbar_get_width(int id){
    return gui::gui_scrollbars[id].box.w;
	}

	gs_scalar gui_scrollbar_get_height(int id){
    return gui::gui_scrollbars[id].box.h;
	}

	gs_scalar gui_scrollbar_get_x(int id){
    return gui::gui_scrollbars[id].box.x;
	}

	gs_scalar gui_scrollbar_get_y(int id){
    return gui::gui_scrollbars[id].box.y;
	}

  gs_scalar gui_scrollbar_get_indicator_width(int id){
    return gui::gui_scrollbars[id].indicator_box.w;
	}

	gs_scalar gui_scrollbar_get_indicator_height(int id){
    return gui::gui_scrollbars[id].indicator_box.h;
	}

	gs_scalar gui_scrollbar_get_indicator_x(int id){
    return gui::gui_scrollbars[id].indicator_box.x;
	}

	gs_scalar gui_scrollbar_get_indicator_y(int id){
    return gui::gui_scrollbars[id].indicator_box.y;
	}

  double gui_scrollbar_get_value(int id){
		return gui::gui_scrollbars[id].value;
  }

  double gui_scrollbar_get_minvalue(int id){
    return gui::gui_scrollbars[id].minValue;
  }

  double gui_scrollbar_get_maxvalue(int id){
    return gui::gui_scrollbars[id].maxValue;
  }

  double gui_scrollbar_get_incvalue(int id){
    return gui::gui_scrollbars[id].incValue;
  }

  gs_scalar gui_scrollbar_get_indicator_size(int id){
    return gui::gui_scrollbars[id].size;
  }

  bool gui_scrollbar_get_direction(int id){
    return gui::gui_scrollbars[id].direction;
  }

  ///Drawers
	void gui_scrollbar_draw(int id){
    gui::gui_scrollbars[id].update();
		gui::gui_scrollbars[id].draw();
	}

	void gui_scrollbars_draw(){
		for (unsigned int i=0; i<gui::gui_scrollbars_maxid; ++i){
			if (gui::gui_scrollbars[i].visible == true && gui::gui_scrollbars[i].parent_id == -1){
      	gui::gui_scrollbars[i].update();
				gui::gui_scrollbars[i].draw();
			}
		}
	}
}

