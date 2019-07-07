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

#include "Universal_System/var4.h"
#include "Platforms/General/PFmain.h" //For mouse_check_button
#include "Universal_System/Resources/resource_data.h" //For script_execute
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "elements.h"
#include "styles.h"
#include "skins.h"
#include "labels.h"
#include "common.h"
#include "include.h"

#include <unordered_map>
#include <deque>
#include <string>

using namespace enigma::gui;
using std::string;
using std::unordered_map;
using std::deque;

namespace enigma {
namespace gui {
	//Implements label class
	Label::Label(){
	  style_id = guiElements.gui_style_label; //Default style
	  enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_left);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_top);
	}

	void Label::draw(gs_scalar ox, gs_scalar oy){
		//Draw sprite
    get_data_element(sty,Style,GUI_TYPE::STYLE,style_id);
    if (sty.sprites[enigma_user::gui_state_default] != -1){
      if (sty.border.zero == true){
        enigma_user::draw_sprite_stretched(sty.sprites[enigma_user::gui_state_default],-1,
                                           ox + box.x,
                                           oy + box.y,
                                           box.w,
                                           box.h,
                                           sty.sprite_styles[enigma_user::gui_state_default].color,
                                           sty.sprite_styles[enigma_user::gui_state_default].alpha);
      }else{
        enigma_user::draw_sprite_padded(sty.sprites[enigma_user::gui_state_default],-1,
                                      sty.border.left,
                                      sty.border.top,
                                      sty.border.right,
                                      sty.border.bottom,
                                      ox + box.x,
                                      oy + box.y,
                                      ox + box.x+box.w,
                                      oy + box.y+box.h,
                                      sty.sprite_styles[enigma_user::gui_state_default].color,
                                      sty.sprite_styles[enigma_user::gui_state_default].alpha);
      }
		}

		//Draw text
    if (text.empty() == false){
  		sty.font_styles[enigma_user::gui_state_default].use();

      gs_scalar textx = 0.0, texty = 0.0;
      switch (sty.font_styles[enigma_user::gui_state_default].halign){
        case enigma_user::fa_left: textx = box.x+sty.padding.left; break;
        case enigma_user::fa_center: textx = box.x+box.w/2.0; break;
        case enigma_user::fa_right: textx = box.x+box.w-sty.padding.right; break;
      }

      switch (sty.font_styles[enigma_user::gui_state_default].valign){
        case enigma_user::fa_top: texty = box.y+sty.padding.top; break;
        case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
        case enigma_user::fa_bottom: texty = box.y+box.h-sty.padding.bottom; break;
      }

  		enigma_user::draw_text(ox + textx,oy + texty,text);
    }
	}
} //namespace gui
} //namespace enigma

namespace enigma_user
{
	int gui_label_create(){
		if (gui_bound_skin == -1){ //Add default one
    	guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid), std::forward_as_tuple(Label(), guiElements.gui_elements_maxid));
		}else{
      get_data_elementv(ski,Skin,GUI_TYPE::SKIN,gui_bound_skin,-1);
      get_elementv(lab,Label,GUI_TYPE::LABEL,ski.label_style,-1);
      guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid), std::forward_as_tuple(lab, guiElements.gui_elements_maxid));
		}
    Label &lab = guiElements.gui_elements[guiElements.gui_elements_maxid];
		lab.visible = true;
		lab.id = guiElements.gui_elements_maxid;
		return (guiElements.gui_elements_maxid++);
	}

	int gui_label_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
		if (gui_bound_skin == -1){ //Add default one
    	guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid), std::forward_as_tuple(Label(), guiElements.gui_elements_maxid));
		}else{
      get_data_elementv(ski,Skin,GUI_TYPE::SKIN,gui_bound_skin,-1);
      get_elementv(lab,Label,GUI_TYPE::LABEL,ski.label_style,-1);
      guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid), std::forward_as_tuple(lab, guiElements.gui_elements_maxid));
		}
    Label &lab = guiElements.gui_elements[guiElements.gui_elements_maxid];
		lab.visible = true;
		lab.id = guiElements.gui_elements_maxid;
		lab.box.set(x, y, w, h);
		lab.text = text;
		return (guiElements.gui_elements_maxid++);
	}

  int gui_label_duplicate(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    guiElements.gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(guiElements.gui_elements_maxid), std::forward_as_tuple(lab, guiElements.gui_elements_maxid));
    guiElements.gui_elements[guiElements.gui_elements_maxid].id = guiElements.gui_elements_maxid;
    Label &l = guiElements.gui_elements[guiElements.gui_elements_maxid];
    l.id = guiElements.gui_elements_maxid;
    l.parent_id = -1; //We cannot duplicate parenting for now
    return guiElements.gui_elements_maxid++;
  }

	void gui_label_destroy(int id){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
    if (lab.parent_id != -1){
      gui_window_remove_label(lab.parent_id, id);
	  }
		guiElements.gui_elements.erase(guiElements.gui_elements.find(id));
	}

  ///Setters
	void gui_label_set_text(int id, string text){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
		lab.text = text;
	}

	void gui_label_set_position(int id, gs_scalar x, gs_scalar y){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
		lab.box.x = x;
		lab.box.y = y;
	}

  void gui_label_set_size(int id, gs_scalar w, gs_scalar h){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
		lab.box.w = w;
		lab.box.h = h;
	}

	void gui_label_set_style(int id, int style_id){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
    check_data_element(GUI_TYPE::STYLE, style_id);
    lab.style_id = (style_id != -1? style_id : guiElements.gui_style_label);
  }

	void gui_label_set_visible(int id, bool visible){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
		lab.visible = visible;
	}

  ///Getters
  gs_scalar gui_label_get_width(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-2);
    return lab.box.w;
  }

  gs_scalar gui_label_get_height(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    return lab.box.h;
  }

  gs_scalar gui_label_get_x(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    return lab.box.x;
  }

  gs_scalar gui_label_get_y(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    return lab.box.y;
  }

  int gui_label_get_style(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    return lab.style_id;
  }

	bool gui_label_get_visible(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,false);
    return lab.visible;
  }

  int gui_label_get_parent(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,-1);
    return lab.parent_id;
  }

	string gui_label_get_text(int id){
    get_elementv(lab,Label,GUI_TYPE::LABEL,id,"");
    return lab.text;
  }

  ///Drawers
	void gui_label_draw(int id){
    get_element(lab,Label,GUI_TYPE::LABEL,id);
    int pfont = enigma_user::draw_get_font();
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		lab.draw();
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
    enigma_user::draw_set_font(pfont);
	}

	void gui_labels_draw(){
    int pfont = enigma_user::draw_get_font();
		unsigned int phalign = enigma_user::draw_get_halign();
		unsigned int pvalign = enigma_user::draw_get_valign();
		int pcolor = enigma_user::draw_get_color();
		gs_scalar palpha = enigma_user::draw_get_alpha();
		for (auto &l : guiElements.gui_elements){
		  ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
      if (l.second.type == GUI_TYPE::LABEL){
        get_element(lab,Label,GUI_TYPE::LABEL,l.first);
        if (lab.visible == true && lab.parent_id == -1){
          lab.draw();
        }
      }
		}
		enigma_user::draw_set_halign(phalign);
		enigma_user::draw_set_valign(pvalign);
		enigma_user::draw_set_color(pcolor);
		enigma_user::draw_set_alpha(palpha);
    enigma_user::draw_set_font(pfont);
	}
} //namespace enigma_user
