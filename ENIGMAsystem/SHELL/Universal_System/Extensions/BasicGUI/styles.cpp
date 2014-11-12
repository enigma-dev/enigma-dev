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
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "include.h"
#include "styles.h"

namespace gui{
	gui_style::gui_style(){
		font_styles[0].halign = font_styles[1].halign = font_styles[2].halign = font_styles[3].halign = font_styles[4].halign = font_styles[5].halign = enigma_user::fa_left;
		font_styles[0].valign = font_styles[1].valign = font_styles[2].valign = font_styles[3].valign = font_styles[4].valign = font_styles[5].valign = enigma_user::fa_middle;
    sprites.fill(-1);
	}

	unordered_map<unsigned int, gui_style> gui_styles;
	unsigned int gui_styles_maxid = 0;

	//Default styles
	unsigned int gui_style_button = enigma_user::gui_style_create();
	unsigned int gui_style_toggle = enigma_user::gui_style_create();
	unsigned int gui_style_window = enigma_user::gui_style_create();
	unsigned int gui_style_slider = enigma_user::gui_style_create();
  unsigned int gui_style_label = enigma_user::gui_style_create();
}

namespace enigma_user
{
	int gui_style_create(){
		gui::gui_styles.insert(pair<unsigned int, gui::gui_style >(gui::gui_styles_maxid, gui::gui_style()));
		gui::gui_styles[gui::gui_styles_maxid].id = gui::gui_styles_maxid;
		return gui::gui_styles_maxid++;
	}

	void gui_style_destroy(int id){
		gui::gui_styles.erase(gui::gui_styles.find(id));
	}

  void gui_style_set_font(int id, int state, int font){
		if (state == enigma_user::gui_state_all){
      for ( auto &it : gui::gui_styles[id].font_styles ){
        it.font = font;
      }
		}else{
			gui::gui_styles[id].font_styles[state].font = font;
		}
	}

	void gui_style_set_font_halign(int id, int state, unsigned int halign){
		if (state == enigma_user::gui_state_all){
      for ( auto &it : gui::gui_styles[id].font_styles ){
        it.halign = halign;
      }
		}else{
			gui::gui_styles[id].font_styles[state].halign = halign;
		}
	}

	void gui_style_set_font_valign(int id, int state, unsigned int valign){
		if (state == enigma_user::gui_state_all){
      for ( auto &it : gui::gui_styles[id].font_styles ){
        it.valign = valign;
      }
		}else{
			gui::gui_styles[id].font_styles[state].valign = valign;
		}
	}

	void gui_style_set_font_color(int id, int state, int color){
		if (state == enigma_user::gui_state_all){
      for ( auto &it : gui::gui_styles[id].font_styles ){
        it.color = color;
      }
		}else{
			gui::gui_styles[id].font_styles[state].color = color;
		}
	}

	void gui_style_set_font_alpha(int id, int state, gs_scalar alpha){
		if (state == enigma_user::gui_state_all){
      for ( auto &it : gui::gui_styles[id].font_styles ){
        it.alpha = alpha;
      }
		}else{
			gui::gui_styles[id].font_styles[state].alpha = alpha;
		}
	}

	void gui_style_set_sprite(int id, int state, int sprid){
	  if (state == enigma_user::gui_state_all){
      gui::gui_styles[id].sprites.fill(sprid);
	  }else{
      gui::gui_styles[id].sprites[state] = sprid;
	  }
	}

	void gui_style_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_styles[id].padding.set(left,top,right,bottom);
	}

	void gui_style_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom){
		gui::gui_styles[id].border.set(left,top,right,bottom);
	}

  void gui_style_set_image_offset(int id, gs_scalar x, gs_scalar y){
		gui::gui_styles[id].image_offset.set(x,y);
	}
}
