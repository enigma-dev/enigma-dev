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
#include <ctype.h> //isalnum
using std::string;
using std::to_string;
using std::unordered_map;

#include "Universal_System/var4.h"
#include "Universal_System/CallbackArrays.h" //For mouse_check_button and keyboard_check_button
#include "Universal_System/resource_data.h" //For script_execute
#include "Universal_System/estring.h" //For string manipulation

//#include "Universal_System/spritestruct.h"
#include "Graphics_Systems/General/GSsprite.h"
#include "Graphics_Systems/General/GSfont.h"
#include "Graphics_Systems/General/GScolors.h"

#include "elements.h"
#include "styles.h"
#include "skins.h"
#include "groups.h"
#include "textboxes.h"
#include "windows.h"
#include "include.h"
#include "common.h"

namespace gui
{
  extern unsigned int gui_elements_maxid;
  extern unsigned int gui_data_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;
  extern unordered_map<unsigned int, DataElement> gui_data_elements;

  extern int gui_bound_skin;
  extern unsigned int gui_style_textbox;

  extern bool windowStopPropagation;

  //Implements textbox class
  Textbox::Textbox(){
    style_id = gui_style_textbox; //Default style
    enigma_user::gui_style_set_font_halign(style_id, enigma_user::gui_state_all, enigma_user::fa_center);
    enigma_user::gui_style_set_font_valign(style_id, enigma_user::gui_state_all, enigma_user::fa_middle);
    callback.fill(-1); //Default callbacks don't exist (so it doesn't call any script)
  }

  void Textbox::callback_execute(int event){
    if (callback[event] != -1){
      enigma_user::script_execute(callback[event], id, active, state, event);
    }
  }

  //Update all possible textbox states (hover, click, toggle etc.)
  void Textbox::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
    //Update children
    parenter.update_children(ox+box.x, oy+box.y);

    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,parent_id);

    get_data_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    sty.font_styles[state].use();
    ///TODO(harijs) - This box check needs to take into account multi-level parenting
    bool pacheck = (parent_id == -1 || (parent_id != -1 && (win.stencil_mask == false || win.box.point_inside(tx,ty))));
    if (gui::windowStopPropagation == false && pacheck == true && box.point_inside(tx-ox,ty-oy)){
      callback_execute(enigma_user::gui_event_hover);
      gui::windowStopPropagation = true;
      if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        active = true;
        state = enigma_user::gui_state_on_active;
        callback_execute(enigma_user::gui_event_pressed);
        //Update cursor position
        int h = floor((ty-oy-box.y) / (double)enigma_user::font_height(enigma_user::draw_get_font()));
        //printf("Line check [%i] with mouse_y = %f and oy = %f and boxy = %f and font_height = %i\n", h, ty, oy, box.y, enigma_user::font_height(enigma_user::draw_get_font()));
        lines = text.size();
        if (h >= lines){
          cursor_position = text[lines-1].length();
          cursor_x = enigma_user::string_width(text[lines-1]);
          cursor_y = (lines-1) * (double)enigma_user::font_height(enigma_user::draw_get_font());
          cursor_line = (lines-1);
          //printf("h>=Line [%i] = [%s] and cursor [%i] with x [%f] y [%f]\n", cursor_line, text[cursor_line].c_str(), cursor_position, cursor_x, cursor_y);
        }else{
          double wi = 0.0;
          for (int c=0; c<text[h].length(); ++c){
            //printf("char [%i] and xcheck %f < wi %f\n", c, tx-ox-box.x, wi);
            if (tx-ox-box.x < wi){
              break;
            }
            cursor_position = c;
            wi += enigma_user::string_width(text[h].substr(c,1));
          }
          cursor_x = enigma_user::string_width(text[h].substr(0,cursor_position));
          cursor_y = h * (double)enigma_user::font_height(enigma_user::draw_get_font());
          cursor_line = h;
          //printf("h<Line [%i] = [%s] and cursor [%i] with x [%f] y [%f]\n", cursor_line, text[cursor_line].c_str(), cursor_position, cursor_x, cursor_y);
        }
        blink_timer = 0;
      }else{
        if (state != enigma_user::gui_state_active && state != enigma_user::gui_state_on_active){
          if (active == false){
            state = enigma_user::gui_state_hover;
          }else{
            state = enigma_user::gui_state_on_hover;
          }
        }else{
          if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
            callback_execute(enigma_user::gui_event_released);
          }
        }
      }
    }else{
      if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){
        active = false;
      }
      state = enigma_user::gui_state_on;
    }

    if (active == true){
      if (enigma_user::keyboard_check_pressed(enigma_user::vk_anykey)){
        repeat_timer = 0;
      }

      if (enigma_user::keyboard_check(enigma_user::vk_anykey)){
        if (repeat_timer == 0 || repeat_timer > 15){
          switch (enigma_user::keyboard_lastkey){
            case enigma_user::vk_backspace:{
              //printf("Line [%i] = [%s] and cursor [%i] with x [%f] y [%f]\n", cursor_line, text[cursor_line].c_str(), cursor_position, cursor_x, cursor_y);
              if (cursor_position > 0) {
                cursor_position--;
                //This would be more efficient, but string_width() returns rounded values
                //cursor_x -= enigma_user::string_width(text[cursor_line][cursor_position]);
                //printf("Line [%i] = [%s] and cursor [%i] with x [%f] y [%f]\n", cursor_line, text[cursor_line].c_str(), cursor_position, cursor_x, cursor_y);
                text[cursor_line].erase(cursor_position,1);
              }else{
                if (cursor_line > 0){
                  cursor_line--;
                  cursor_position = text[cursor_line].length();
                  cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());

                  text[cursor_line] += text[cursor_line+1];
                  text.erase(text.begin()+cursor_line+1);
                }
              }
              break;
            }
            case enigma_user::vk_delete:{
              if (cursor_position < text[cursor_line].length()){
                text[cursor_line].erase(cursor_position,1);
              }else{
                if (cursor_line < text.size()-1){
                  text[cursor_line] += text[cursor_line+1];
                  text.erase(text.begin()+cursor_line+1);
                }
              }
              break;
            }
            case enigma_user::vk_left:{
              if (cursor_position > 0){
                cursor_position--;
                //printf("Line [%i] = [%c] and delta x = %u\n", cursor_line, text[cursor_line][cursor_position], enigma_user::string_width(string(1,text[cursor_line][cursor_position]))/*enigma_user::string_width(to_string(text[cursor_line][cursor_position]))*/);
                //This would be more efficient, but string_width() returns rounded values
                //cursor_x -= enigma_user::string_width(string(1,text[cursor_line][cursor_position]));
                //printf("String [%s]\n", text[cursor_line].substr(0,cursor_position).c_str());
              }else{
                if (cursor_line > 0){
                  cursor_line--;
                  cursor_position = text[cursor_line].length();
                  cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
                }
              }
              break;
            }
            case enigma_user::vk_right:{
              if (cursor_position < text[cursor_line].length()){
                cursor_position++;
                //This would be more efficient, but string_width() returns rounded values
                //cursor_x += enigma_user::string_width(string(1,text[cursor_line][cursor_position]));
                //printf("String [%s]\n", text[cursor_line].substr(0,cursor_position).c_str());
              }else{
                if (cursor_line < text.size()-1){
                  cursor_line++;
                  cursor_position = 0;
                  cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
                }
              }
              break;
            }
            case enigma_user::vk_enter:{
              //printf("Line [%i] = [%s] and cursor [%i] with x [%f] y [%f]\n", cursor_line, text[cursor_line].c_str(), cursor_position, cursor_x, cursor_y);
              text.insert(text.begin()+cursor_line+1,text[cursor_line].substr(cursor_position, string::npos));
              /*int in=0;
              for (auto &str : text){
                printf("String[%i] - %s\n",in,str.c_str());
                in++;
              }*/
              text[cursor_line].erase(cursor_position, string::npos);
              cursor_line++;
              cursor_position = 0;
              cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
              break;
            }
            case enigma_user::vk_up:{
              if (cursor_line > 0){
                cursor_line--;
                if (cursor_position > text[cursor_line].length()){
                  cursor_position = text[cursor_line].length();
                }
                cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
              }
              break;
            }
            case enigma_user::vk_down:{
              if (cursor_line < text.size()-1){
                cursor_line++;
                if (cursor_position > text[cursor_line].length()){
                  cursor_position = text[cursor_line].length();
                }
                cursor_y = cursor_line * (double)enigma_user::font_height(enigma_user::draw_get_font());
              }
              break;
            }
            default:{
              text[cursor_line].insert(cursor_position,enigma_user::keyboard_lastchar);
              cursor_position++;
              break;
            }
          }
          cursor_x = enigma_user::string_width(text[cursor_line].substr(0,cursor_position));
          blink_timer = 0;
        }
        repeat_timer++;
      }
    }
  }

  void Textbox::draw(gs_scalar ox, gs_scalar oy){
    //Draw textbox
    get_data_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    if (sty.sprites[state] != -1){
      if (sty.border.zero == true){
        enigma_user::draw_sprite_stretched(sty.sprites[state],-1,
                                           ox + box.x,
                                           oy + box.y,
                                           box.w,
                                           box.h,
                                           sty.sprite_styles[state].color,
                                           sty.sprite_styles[state].alpha);
      }else{
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

    int l = 0;
    double h = (double)enigma_user::font_height(enigma_user::draw_get_font());
    for (auto &str : text){
      if (str.empty() == false){
        printf("Drawing text at %f and %f\n", textx, texty);
        enigma_user::draw_text(ox + textx,oy + texty + l * h,str);
      }
      l++;
    }
    
    if (active == true){
      if (blink_timer < 15){
        switch (sty.font_styles[state].halign){
          case enigma_user::fa_left: textx = box.x+sty.padding.left; break;
          case enigma_user::fa_center: textx = box.x+box.w/2.0+cursor_x/2.0; break;
          case enigma_user::fa_right: textx = box.x+box.w-sty.padding.right; break;
        }

        switch (sty.font_styles[state].valign){
          case enigma_user::fa_top: texty = box.y+sty.padding.top; break;
          case enigma_user::fa_middle: texty = box.y+box.h/2.0; break;
          case enigma_user::fa_bottom: texty = box.y+box.h-sty.padding.bottom; break;
        }
        printf("Drawing cursor at %f and %f\n", textx + cursor_x, texty + cursor_y);
        double cw = enigma_user::string_width("|")/2.0;
        enigma_user::draw_text(ox + textx + cursor_x - cw, oy + texty + cursor_y, "|");
      }
      blink_timer++;
      if (blink_timer > 30){
        blink_timer = 0;
      }
    }
    //Draw children
    parenter.draw_children(ox+box.x,oy+box.y);
  }

  void Textbox::update_text_pos(int state){
    //gui::gui_styles[style_id].update_text_pos(box, state);
  }
}

namespace enigma_user
{
  int gui_textbox_create(){
    if (gui::gui_bound_skin == -1){ //Add default one
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(gui::Textbox(), gui::gui_elements_maxid));
      //printf("Creating default button with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
    }else{
      get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
      get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,ski.textbox_style,-1);
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(tex, gui::gui_elements_maxid));
      //printf("Creating button with size %i\n", sizeof(gui::gui_elements[gui::gui_elements_maxid]));
    }
    gui::Textbox &b = gui::gui_elements[gui::gui_elements_maxid];
    b.visible = true;
    b.id = gui::gui_elements_maxid;
    return (gui::gui_elements_maxid++);
  }

  int gui_textbox_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
    if (gui::gui_bound_skin == -1){ //Add default one
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(gui::Textbox(), gui::gui_elements_maxid));
    }else{
      get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
      get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,ski.textbox_style,-1);
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(tex, gui::gui_elements_maxid));
    }
    gui::Textbox &b = gui::gui_elements[gui::gui_elements_maxid];
    b.visible = true;
    b.id = gui::gui_elements_maxid;
    b.box.set(x, y, w, h);
    b.text[0] = text;
    b.update_text_pos();
    return (gui::gui_elements_maxid++);
  }

  void gui_textbox_destroy(int id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    if (tex.parent_id != -1){
      gui_window_remove_button(tex.parent_id, id);
    }
    gui::gui_elements.erase(gui::gui_elements.find(id));
  }

/*  ///Setters
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
*/
  void gui_textbox_set_style(int id, int style_id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    check_data_element(gui::GUI_TYPE::STYLE, style_id);
    tex.style_id = (style_id != -1? style_id : gui::gui_style_textbox);
  }/*

  void gui_button_set_active(int id, bool active){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
    but.active = active;
  }

  void gui_button_set_togglable(int id, bool togglable){
    get_element(but,gui::Button,gui::GUI_TYPE::BUTTON,id);
    but.togglable = togglable;
  }
*/
  void gui_textbox_set_visible(int id, bool visible){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.visible = visible;
  }
/*
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
    int pfont = enigma_user::draw_get_font();
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
    enigma_user::draw_set_font(pfont);
  }

  void gui_buttons_draw(){
    int pfont = enigma_user::draw_get_font();
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
    enigma_user::draw_set_font(pfont);
  }

  ///Parenting
  void gui_button_add_button(int id, int bid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.button_add(bid);
  }

  void gui_button_add_toggle(int id, int tid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.toggle_add(tid);
  }

  void gui_button_add_slider(int id, int sid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.slider_add(sid);
  }

  void gui_button_add_scrollbar(int id, int sid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.scrollbar_add(sid);
  }

  void gui_button_add_label(int id, int lid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.label_add(lid);
  }

  void gui_button_add_window(int id, int wid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.window_add(wid);
  }

  void gui_button_remove_button(int id, int bid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.button_remove(bid);
  }

  void gui_button_remove_toggle(int id, int tid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.toggle_remove(tid);
  }

  void gui_button_remove_slider(int id, int sid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.slider_remove(sid);

  }

  void gui_button_remove_scrollbar(int id, int sid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.scrollbar_remove(sid);
  }

  void gui_button_remove_label(int id, int lid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.label_remove(lid);
  }

  void gui_button_remove_window(int id, int wid){
    get_element(ele,gui::Button,gui::GUI_TYPE::BUTTON,id);
    ele.parenter.window_remove(wid);
  }

  int gui_button_get_button_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.button_count();
  }

  int gui_button_get_toggle_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.toggle_count();
  }

  int gui_button_get_slider_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.slider_count();
  }

  int gui_button_get_scrollbar_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.scrollbar_count();
  }

  int gui_button_get_label_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.label_count();
  }

  int gui_button_get_window_count(int id){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.window_count();
  }

  ///GETTERS FOR ELEMENTS
  int gui_button_get_button(int id, int but){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.button(but);
  }

  int gui_button_get_toggle(int id, int tog){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.toggle(tog);
  }

  int gui_button_get_slider(int id, int sli){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.slider(sli);
  }

  int gui_button_get_scrollbar(int id, int scr){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.scrollbar(scr);
  }

  int gui_button_get_label(int id, int lab){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.label(lab);
  }

  int gui_button_get_window(int id, int wid){
    get_elementv(ele,gui::Button,gui::GUI_TYPE::BUTTON,id,-1);
    return ele.parenter.window(wid);
  }*/
}

