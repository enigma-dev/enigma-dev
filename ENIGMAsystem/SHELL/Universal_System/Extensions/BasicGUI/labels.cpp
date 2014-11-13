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
#include "labels.h"
#include "include.h"
#include "common.h"

namespace gui
{
  unordered_map<unsigned int, gui_label> gui_labels;
	unsigned int gui_labels_maxid = 0;

	extern int gui_bound_skin;
	extern unordered_map<unsigned int, gui_skin> gui_skins;
	extern unordered_map<unsigned int, gui_style> gui_styles;
	extern unsigned int gui_skins_maxid;
	extern unsigned int gui_style_label;

	extern bool windowStopPropagation;

	//Implements label class
	gui_label::gui_label(){
	  style_id = gui_style_label; //Default style
	  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
	}

	void gui_label::draw(gs_scalar ox, gs_scalar oy){
		//Draw button
    if (gui::gui_styles[style_id].sprites[enigma_user::gui_state_default] != -1){
      enigma_user::draw_sprite_padded(gui::gui_styles[style_id].sprites[enigma_user::gui_state_default],-1,gui::gui_styles[style_id].border.left,gui::gui_styles[style_id].border.top,gui::gui_styles[style_id].border.right,gui::gui_styles[style_id].border.bottom,ox + box.x,oy + box.y,ox + box.x+box.w,oy + box.y+box.h);
		}

		//Draw text
		gui::gui_styles[style_id].font_styles[enigma_user::gui_state_default].use();

    gs_scalar textx = 0.0, texty = 0.0;
    switch (gui::gui_styles[style_id].font_styles[enigma_user::gui_state_default].halign){
      case enigma_user::fa_left: textx = box.x+gui::gui_styles[style_id].padding.left; break;
      case enigma_user::fa_center: textx = box.x+box.w/2.0; break;
      case enigma_user::fa_right: textx = box.x+box.w-gui::gui_styles[style_id].padding.right; break;
    }

    switch (gui::gui_styles[style_id].font_styles[enigma_user::gui_state_default].valign){
      case enigma_user::fa_top: texty = box.y+gui::gui_styles[style_id].padding.top; break;
      case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
      case enigma_user::fa_bottom: texty = box.y+box.h-gui::gui_styles[style_id].padding.bottom; break;
    }

		enigma_user::draw_text(ox + textx,oy + texty,text);
	}
}

namespace enigma_user
{
	int gui_label_create(){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_labels.insert(pair<unsigned int, gui::gui_label >(gui::gui_labels_maxid, gui::gui_label()));
		}else{
			gui::gui_labels.insert(pair<unsigned int, gui::gui_label >(gui::gui_labels_maxid, gui::gui_labels[gui::gui_skins[gui::gui_bound_skin].label_style]));
		}
		gui::gui_labels[gui::gui_labels_maxid].visible = true;
		gui::gui_labels[gui::gui_labels_maxid].id = gui::gui_labels_maxid;
		return (gui::gui_labels_maxid++);
	}

	int gui_label_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui::gui_bound_skin == -1){ //Add default one
			gui::gui_labels.insert(pair<unsigned int, gui::gui_label >(gui::gui_labels_maxid, gui::gui_label()));
		}else{
			gui::gui_labels.insert(pair<unsigned int, gui::gui_label >(gui::gui_labels_maxid, gui::gui_labels[gui::gui_skins[gui::gui_bound_skin].label_style]));
		}
		gui::gui_labels[gui::gui_labels_maxid].visible = true;
		gui::gui_labels[gui::gui_labels_maxid].id = gui::gui_labels_maxid;
		gui::gui_labels[gui::gui_labels_maxid].box.set(x, y, w, h);
		gui::gui_labels[gui::gui_labels_maxid].text = text;
		return (gui::gui_labels_maxid++);
	}

	void gui_label_destroy(int id){
		gui::gui_labels.erase(gui::gui_labels.find(id));
	}

	void gui_label_set_text(int id, string text){
		gui::gui_labels[id].text = text;
	}

	void gui_label_set_position(int id, gs_scalar x, gs_scalar y){
		gui::gui_labels[id].box.x = x;
		gui::gui_labels[id].box.y = y;
	}

  void gui_label_set_size(int id, gs_scalar w, gs_scalar h){
		gui::gui_labels[id].box.w = w;
		gui::gui_labels[id].box.h = h;
	}

	void gui_label_set_style(int id, int style_id){
    gui::gui_labels[id].style_id = (style_id != -1? style_id : gui::gui_style_label);
  }

	void gui_label_set_visible(int id, bool visible){
		gui::gui_labels[id].visible = visible;
	}

  int gui_label_get_style(int id){
    return gui::gui_labels[id].style_id;
  }

	void gui_label_draw(int id){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		gui::gui_labels[id].draw();
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}

	void gui_labels_draw(){
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		for (unsigned int i=0; i<gui::gui_labels_maxid; ++i){
			if (gui::gui_labels[i].visible == true && gui::gui_labels[i].parent_id == -1){
				gui::gui_labels[i].draw();
			}
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
	}
}

