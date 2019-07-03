/** Copyright (C) 2015 Harijs Grinbergs
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
#include "Platforms/General/PFmain.h" //For mouse_check_button and keyboard_check_button
#include "Universal_System/Resources/resource_data.h" //For script_execute
#include "Universal_System/estring.h" //For string manipulation
#include "Platforms/General/PFwindow.h" //For clipboard

//#include "Universal_System/Resources/sprites_internal.h"
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

        set_cursor(tx-ox, ty-oy);
        set_marker_start(cursor_line, cursor_position);

        //Check double click
        if (mouse_timer>0){
          size_t c = cursor_position;
          for (c=cursor_position; c>0; --c){
            if (text[cursor_line][c] == ' '){
              ++c;
              break;
            }
          }
          set_marker_start(cursor_line, c);
          for (c=cursor_position; c<text[cursor_line].length(); ++c){
            if (text[cursor_line][c] == ' '){
              break;
            }
          }
          set_marker(cursor_line, c);
          cursor_position = c;
          update_cursor();
          mark = true;
          double_click = true;
        }
        mouse_timer = 10;
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
        mark = false;
      }
      state = enigma_user::gui_state_on;
    }

    if (active == true){
      if (mouse_timer>0) mouse_timer--;
      if (mouse_timer == 0) { double_click = false; }

      if (enigma_user::mouse_check_button(enigma_user::mb_left) && double_click == false){ //Drag select
        mark = true;
        set_cursor(tx-ox, ty-oy);
        set_marker(cursor_line, cursor_position);
      }

      if (enigma_user::keyboard_check_pressed(enigma_user::vk_anykey)){
        repeat_timer = 0;
      }

      if (enigma_user::keyboard_check(enigma_user::vk_anykey)){
        bool change = false;
        if (repeat_timer == 0 || repeat_timer > 15){
          ///Todo(harijs) : This might go in the bottom switch for each time c,C,v,V is pressed
          if (enigma_user::keyboard_check(enigma_user::vk_control)){
            switch (enigma_user::keyboard_lastkey){
              case 'v':
              case 'V':
                marker_insert(enigma_user::clipboard_get_text()); change = true;
              break;
              case 'c':
              case 'C':
                if (mark == true) {
                  enigma_user::clipboard_set_text(marker_string());
                }
              break;
              case 'a':
              case 'A':
                mark = true;
                set_marker_start(0, 0);
                set_marker(text.size()-1, text.back().length());
                cursor_position = text.back().length();
                cursor_line = text.size()-1;
                update_cursor();
              break;
            }
          }else{
            switch (enigma_user::keyboard_lastkey){
              case enigma_user::vk_backspace:{
                change = true;
                if (mark == true){
                  marker_delete();
                  update_cursor();
                  break;
                }
                if (cursor_position > 0) {
                  cursor_position--;
                  //This would be more efficient, but string_width() returns rounded values
                  //cursor_x -= enigma_user::string_width(text[cursor_line][cursor_position]);
                  text[cursor_line].erase(cursor_position,1);
                }else{
                  if (cursor_line > 0){
                    cursor_line--;
                    cursor_position = text[cursor_line].length();
                    update_cursory();

                    text[cursor_line] += text[cursor_line+1];
                    text.erase(text.begin()+cursor_line+1);
                    update_cursory();
                  }
                }
                update_cursorx();
                break;
              }
              case enigma_user::vk_delete:{
                change = true;
                if (mark == true){
                  marker_delete();
                  update_cursor();
                  break;
                }
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
                int pcp = cursor_position;
                if (cursor_position > 0){
                  cursor_position--;
                  //This would be more efficient, but string_width() returns rounded values
                  //cursor_x -= enigma_user::string_width(string(1,text[cursor_line][cursor_position]));
                }else{
                  if (cursor_line > 0){
                    cursor_line--;
                    cursor_position = text[cursor_line].length();
                    update_cursory();
                  }
                }
                update_cursorx();
                if (enigma_user::keyboard_check(enigma_user::vk_shift)){
                  if (mark == false){
                    set_marker_start(cursor_line, pcp);
                    set_marker(cursor_line, cursor_position);
                    mark = true;
                  }else{ //Update marker
                    set_marker(cursor_line, cursor_position);
                  }
                }else{
                  mark = false;
                }
                break;
              }
              case enigma_user::vk_right:{
                int pcp = cursor_position;
                if (cursor_position < text[cursor_line].length()){
                  cursor_position++;
                  //This would be more efficient, but string_width() returns rounded values
                  //cursor_x += enigma_user::string_width(string(1,text[cursor_line][cursor_position]));
                }else{
                  if (cursor_line < text.size()-1){
                    cursor_line++;
                    cursor_position = 0;
                    update_cursory();
                  }
                }
                update_cursorx();
                if (enigma_user::keyboard_check(enigma_user::vk_shift)){
                  if (mark == false){
                    set_marker_start(cursor_line, pcp);
                    set_marker(cursor_line, cursor_position);
                    mark = true;
                  }else{ //Update marker
                    set_marker(cursor_line, cursor_position);
                  }
                }else{
                  mark = false;
                }
                break;
              }
              case enigma_user::vk_enter:{
                if (line_limit>0 && text.size()>=line_limit) break;
                change = true;
                marker_delete();
                text.insert(text.begin()+cursor_line+1,text[cursor_line].substr(cursor_position, string::npos));
                text[cursor_line].erase(cursor_position, string::npos);
                cursor_line++;
                cursor_position = 0;
                update_cursor();
                mark = false;
                break;
              }
              case enigma_user::vk_up:{
                int pcp = cursor_position;
                int pcl = cursor_line;
                if (cursor_line > 0){
                  cursor_line--;
                  if (cursor_position > text[cursor_line].length()){
                    cursor_position = text[cursor_line].length();
                  }
                  update_cursor();
                }
                if (enigma_user::keyboard_check(enigma_user::vk_shift)){
                  if (mark == false){
                    set_marker_start(pcl, pcp);
                    set_marker(cursor_line, cursor_position);
                    mark = true;
                  }else{ //Update marker
                    set_marker(cursor_line, cursor_position);
                  }
                }else{
                  mark = false;
                }
                break;
              }
              case enigma_user::vk_down:{
                int pcp = cursor_position;
                int pcl = cursor_line;
                if (cursor_line < text.size()-1){
                  cursor_line++;
                  if (cursor_position > text[cursor_line].length()){
                    cursor_position = text[cursor_line].length();
                  }
                  update_cursor();
                }
                if (enigma_user::keyboard_check(enigma_user::vk_shift)){
                  if (mark == false){
                    set_marker_start(pcl, pcp);
                    set_marker(cursor_line, cursor_position);
                    mark = true;
                  }else{ //Update marker
                    set_marker(cursor_line, cursor_position);
                  }
                }else{
                  mark = false;
                }
                break;
              }
              break;
              case enigma_user::vk_control:
              case enigma_user::vk_shift:
              case enigma_user::vk_escape:
              break;
              default:{
                if (numbers_only == true){
                  char* p;
                  string str = text[cursor_line];
                  str.insert(cursor_position,enigma_user::keyboard_lastchar);
                  double val = strtod(str.c_str(), &p);
                  if (*p != 0){
                    break;
                  }
                  /*if (cursor_position == 0){
                    if (text[cursor_line].find_first_not_of("-+") && !enigma_user::keyboard_lastchar.find_first_not_of("0123456789.-+")){ //+ and - can only be the first chars
                      break;
                    }
                  }else if (!(enigma_user::keyboard_lastchar.find_first_not_of("0123456789."))){ //We could do >'0' && <'9' || '.' || '-' || '+', but this wouldn't work in unicode
                    break;
                  }*/
                }
                if (mark == true){
                  change = true;
                  marker_delete(); //If something is selected then this deletes it and resets mark
                }
                if (char_limit==0 || text[cursor_line].length()+1<=char_limit){
                  change = true;
                  text[cursor_line].insert(cursor_position,enigma_user::keyboard_lastchar);
                  cursor_position++;
                  update_cursorx();
                }
                break;
              }
            }
            blink_timer = 0;
          }
        }
        repeat_timer++;
        if (change == true){
          //Resize if needed
          /*double h = (double)enigma_user::font_height(enigma_user::draw_get_font());
          double maxw = 0.0;
          //TODO(harijs): Cache this so we don't have to recalculate every time
          for (auto &t : text){
            double w = enigma_user::string_width(t);
            if (w>maxw) maxw = w;
          }
          box.w = maxw + sty.padding.left + sty.padding.right;
          box.h = h * text.size() + sty.padding.top + sty.padding.bottom;*/

          callback_execute(enigma_user::gui_event_change);
        }
      }
    }
  }

  void Textbox::draw(gs_scalar ox, gs_scalar oy){
    //Draw textbox
    get_data_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);

    sty.font_styles[state].use();
    double h = (double)enigma_user::font_height(enigma_user::draw_get_font());
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

    //Draw marker
    get_data_element(msty,gui::Style,gui::GUI_TYPE::STYLE,marker_style_id);
    if (mark == true){
      if (msty.sprites[state] != -1){
        if (msty.border.zero == true){
          for (int l=mark_start_line; l<mark_end_line+1; ++l ){
            double msx, msy, mex, mey;
            if (mark_start_line == l && mark_end_line == l){
              msx = ox + textx + enigma_user::string_width(text[mark_start_line].substr(0, mark_start_pos));
              mex = enigma_user::string_width(text[mark_end_line].substr(mark_start_pos, mark_end_pos-mark_start_pos));
            }else if (l == mark_start_line){
              msx = ox + textx + enigma_user::string_width(text[mark_start_line].substr(0, mark_start_pos));
              mex = enigma_user::string_width(text[mark_start_line].substr(mark_start_pos, string::npos));
            }else if (l == mark_end_line){
              msx = ox + textx;
              mex = enigma_user::string_width(text[mark_end_line].substr(0, mark_end_pos));
            }else{
              msx = ox + textx;
              mex = enigma_user::string_width(text[l]);
            }
            enigma_user::draw_sprite_stretched(msty.sprites[state],-1,
                                               msx,
                                               oy + texty + l * h,
                                               mex,
                                               h,
                                               msty.sprite_styles[state].color,
                                               msty.sprite_styles[state].alpha);
          }
        }else{
          for (int l=mark_start_line; l<mark_end_line+1; ++l ){
            double msx, msy, mex, mey;
            if (mark_start_line == l && mark_end_line == l){
              msx = ox + textx + enigma_user::string_width(text[mark_start_line].substr(0, mark_start_pos));
              mex = ox + textx + enigma_user::string_width(text[mark_end_line].substr(0, mark_end_pos));
            }else if (l == mark_start_line){
              msx = ox + textx + enigma_user::string_width(text[mark_start_line].substr(0, mark_start_pos));
              mex = ox + textx + enigma_user::string_width(text[mark_start_line].substr(mark_start_pos, string::npos));
            }else if (l == mark_end_line){
              msx = ox + textx;
              mex = ox + textx + enigma_user::string_width(text[mark_end_line].substr(0, mark_end_pos));
            }else{
              msx = ox + textx;
              mex = ox + textx + enigma_user::string_width(text[l]);
            }
            enigma_user::draw_sprite_padded(msty.sprites[state],-1,
                                            msty.border.left,
                                            msty.border.top,
                                            msty.border.right,
                                            msty.border.bottom,
                                            msx,
                                            oy + box.y + l * h,
                                            mex,
                                            oy + box.y+box.h + l * h,
                                            msty.sprite_styles[state].color,
                                            msty.sprite_styles[state].alpha);
          }
        }
      }
    }

    //Draw text
    int l = 0;
    for (auto &str : text){
      if (str.empty() == false){
        enigma_user::draw_text(ox + textx,oy + texty + l * h,str);
      }
      l++;
    }

    if (active == true){
      if (blink_timer < 15){
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

  void Textbox::update_text_pos(){
  }

  void Textbox::set_marker(int line, int pos){
    if (line == mark_line){
      if (pos == mark_pos) { mark = false; return; }
      mark_start_line = line;
      mark_end_line = line;
      if (pos < mark_pos){
        mark_start_pos = pos;
        mark_end_pos = mark_pos;
      }else{
        mark_start_pos = mark_pos;
        mark_end_pos = pos;
      }
    }else{
      if (line < mark_line){
        mark_start_line = line;
        mark_start_pos = pos;
        mark_end_line = mark_line;
        mark_end_pos = mark_pos;
      }else{
        mark_start_line = mark_line;
        mark_start_pos = mark_pos;
        mark_end_line = line;
        mark_end_pos = pos;
      }
    }
  }

  void Textbox::marker_delete(){
    if (mark == true){
      if ( mark_start_line == mark_end_line ){
        text[mark_start_line].erase(mark_start_pos, mark_end_pos-mark_start_pos);
      }else{
        //Remove at start line
        text[mark_start_line].erase(mark_start_pos, string::npos);

        //Add to start
        text[mark_start_line] += text[mark_end_line].substr(mark_end_pos, string::npos);

        //Remove the rest
        int l = mark_start_line+1;
        for (auto it = text.begin()+l; it != text.end();){
          it = text.erase(it);
          if (++l > mark_end_line){ break; }
        }
      }
      cursor_position = mark_start_pos;
      cursor_line = mark_start_line;
      mark = false;
    }
  }

  string Textbox::marker_string(){
    string str = "";
    if (mark == true){
      if ( mark_start_line == mark_end_line ){
        str = text[mark_start_line].substr(mark_start_pos, mark_end_pos-mark_start_pos);
      }else{
        //Get at start line
        str += text[mark_start_line].substr(mark_start_pos, string::npos);
        str += "\n";

        //Add the middle
        int l = mark_start_line+1;
        for (int l = mark_start_line+1; l < mark_end_line; ++l){
          str += text[l];
          str += "\n";
          if (++l >mark_end_line){ break; }
        }

        //Add the end
        str += text[mark_end_line].substr(0, mark_end_pos);
      }
    }
    return str;
  }

  void Textbox::marker_insert(string str){
    if (mark == true){
      marker_delete();
    }
    size_t ni = 0;
    size_t line = cursor_line;
    size_t i;
    string endOfFirst = text[cursor_line].substr(cursor_position, string::npos);
    text[cursor_line].erase(cursor_position, string::npos);

    bool limit = false;
    if (char_limit==0){ //UNLIMITED POWE... chars
      for (i = 0; i < str.length(); ++i){
        if (str[i] == '\r' or str[i] == '\n'){
          if (line_limit>0 && (text.size()+1)>line_limit){ limit = true; break; }
          if (line == cursor_line) { //This is still the first line, so we append
            text[line].insert(cursor_position,str.substr(0, i));
          } else { text.insert(text.begin()+line,str.substr(ni, i-ni)); }
          ++i; //Skip /r or /n
          i += (str[i] == '\n'); //If we have /r/n then then skip to next one
          ni = i;
          line++;
        }
      }
      if (ni != i){
        if (line != cursor_line){ //End of multiline
          if (line_limit==0 || (line_limit>0 && text.size()+1<=line_limit)){
            text.insert(text.begin()+line, str.substr(ni, string::npos));
          }
        }else{ //We only had one line without line endings OR the line limit was reached on the first line of multiline string
          if (limit == false){
            text[line].insert(cursor_position,str);
          }else{ //Line limit was reached, so we append only to the first eol
            text[line].insert(cursor_position,str.substr(ni, i-ni));
          }
        }
        text[line].append(endOfFirst);
      }
    }else{ //Limited chars
      for (i = 0; i < str.length(); ++i){
        if (str[i] == '\r' or str[i] == '\n'){
          if (line_limit>0 && (text.size()+1)>line_limit){ limit = true; break; }
          if (line == cursor_line) { //This is still the first line, so we append
            if (text[line].length()+i>=char_limit){ //Char limit reached in the line
              text[line].insert(cursor_position,str.substr(0, char_limit-text[line].length()));
            }else{ //We can fit the whole thing
              text[line].insert(cursor_position,str.substr(0, i));
            }
          } else { text.insert(text.begin()+line,str.substr(ni, i-ni)); }
          ++i; //Skip /r or /n
          i += (str[i] == '\n'); //If we have /r/n then then skip to next one
          ni = i;
          line++;
        }
      }
      if (ni != i){
        if (line != cursor_line){ //End of multiline
          if (line_limit==0 || (line_limit>0 && text.size()+1<=line_limit)){
            if (text[line].length()+str.length()-ni+endOfFirst.length()>=char_limit){
              text.insert(text.begin()+line, str.substr(ni, char_limit-text[line].length()-endOfFirst.length()));
            }else{
              text.insert(text.begin()+line, str.substr(ni, string::npos));
            }
          }
        }else{ //We only had one line without line endings OR the line limit was reached on the first line of multiline string
          if (limit == false){
            if (text[line].length()+str.length()+endOfFirst.length()>=char_limit){
              text[line].insert(cursor_position,str.substr(0, char_limit-text[line].length()-endOfFirst.length()));
            }else{
              text[line].insert(cursor_position,str);
            }
          }else{ //Line limit was reached, so we append only to the first eol
            if (text[line].length()+i-ni+endOfFirst.length()>=char_limit){
              text[line].insert(cursor_position,str.substr(ni, char_limit-text[line].length()-endOfFirst.length()));
            }else{
              text[line].insert(cursor_position,str.substr(ni, i-ni));
            }
          }
        }
        text[line].append(endOfFirst);
      }
    }

    cursor_line = line;
    cursor_position = text[line].length() - endOfFirst.length();
    update_cursor();
  }

  void Textbox::set_cursor(double x, double y){
    lines = text.size();
    if (y-texty < 0){ //Mouse x less than textbox y, so cursor at beginning
      cursor_position = 0;
      cursor_line = 0;
    }else if (y-texty >= (double)enigma_user::font_height(enigma_user::draw_get_font())*lines){ //Mouse y more than textbox height, so cursot at end
      cursor_position = text[lines-1].length();
      cursor_line = (lines-1);
    }else{
      size_t h = floor((y-texty) / (double)enigma_user::font_height(enigma_user::draw_get_font()));
      if (text[h].length() == 0 || x-textx < enigma_user::string_char_width(text[h][0])/2.0){
        cursor_position = 0;
        cursor_line = h;
      }else{
        cursor_position = 0;
        double wi = 0.0;
        for (size_t c=0; c<text[h].length(); ++c){
          double cw = enigma_user::string_char_width(text[h].substr(c,1));
          if (x-textx < (wi + cw/2.0)){
            break;
          }
          cursor_position = c;
          wi += cw;
        }
        cursor_position++; //Cursor at end is AFTER the last char
        cursor_line = h;
      }
    }
    update_cursor();
    blink_timer = 0;
  }

  string Textbox::get_text(){
    string str = "";
    for (size_t l = 0; l < text.size(); ++l){
      str += text[l];
      str += "\n";
    }
    return str;
  }
}

namespace enigma_user
{
  int gui_textbox_create(){
    if (gui::gui_bound_skin == -1){ //Add default one
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(gui::Textbox(), gui::gui_elements_maxid));
    }else{
      get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
      get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,ski.textbox_style,-1);
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(tex, gui::gui_elements_maxid));
    }
    gui::Textbox &t = gui::gui_elements[gui::gui_elements_maxid];
    t.visible = true;
    t.id = gui::gui_elements_maxid;
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
    gui::Textbox &t = gui::gui_elements[gui::gui_elements_maxid];
    t.visible = true;
    t.id = gui::gui_elements_maxid;
    t.box.set(x, y, w, h);
    t.marker_insert(text);
    t.update_text_pos();
    return (gui::gui_elements_maxid++);
  }

  int gui_textbox_duplicate(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(tex, gui::gui_elements_maxid));
    auto &e = gui::gui_elements[gui::gui_elements_maxid];
    e.id = gui::gui_elements_maxid;
    gui::Textbox &t = gui::gui_elements[gui::gui_elements_maxid];
    t.id = gui::gui_elements_maxid;
    t.parent_id = -1; //We cannot duplicate parenting for now
    return gui::gui_elements_maxid++;
  }

  void gui_textbox_destroy(int id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    if (tex.parent_id != -1){
      gui_window_remove_textbox(tex.parent_id, id);
    }
    gui::gui_elements.erase(gui::gui_elements.find(id));
  }

  ///Setters
  void gui_textbox_set_text(int id, string text){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.clear();
    tex.marker_insert(text);
  }

  void gui_textbox_set_numeric(int id, bool numeric){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.numbers_only = numeric;
  }

  void gui_textbox_set_max_length(int id, int length){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.char_limit = length;
  }

  void gui_textbox_set_max_lines(int id, int lines){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.line_limit = lines;
  }

  void gui_textbox_set_position(int id, gs_scalar x, gs_scalar y){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.box.x = x;
    tex.box.y = y;
  }

  void gui_textbox_set_size(int id, gs_scalar w, gs_scalar h){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.box.w = w;
    tex.box.h = h;
    tex.update_text_pos();
  }

  void gui_textbox_set_callback(int id, int event, int script_id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    if (event == enigma_user::gui_event_all){
      tex.callback.fill(script_id);
    }else{
      tex.callback[event] = script_id;
    }
  }

  void gui_textbox_set_style(int id, int style_id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    check_data_element(gui::GUI_TYPE::STYLE, style_id);
    tex.style_id = (style_id != -1? style_id : gui::gui_style_textbox);
  }

  void gui_textbox_set_marker_style(int id, int style_id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    check_data_element(gui::GUI_TYPE::STYLE, style_id);
    tex.marker_style_id = (style_id != -1? style_id : gui::gui_style_textbox);
  }


  void gui_textbox_set_active(int id, bool active){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.active = active;
  }

  void gui_textbox_set_visible(int id, bool visible){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    tex.visible = visible;
  }

  ///Getters
  int gui_textbox_get_style(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.style_id;
  }

  int gui_textbox_get_state(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.state;
  }

  bool gui_textbox_get_active(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,false);
    return tex.active;
  }

  bool gui_textbox_get_visible(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,false);
    return tex.visible;
  }

  int gui_textbox_get_callback(int id, int event){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.callback[event];
  }

  int gui_textbox_get_parent(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.parent_id;
  }

  bool gui_textbox_get_numeric(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.numbers_only;
  }

  gs_scalar gui_textbox_get_width(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.box.w;
  }

  gs_scalar gui_textbox_get_height(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.box.h;
  }

  gs_scalar gui_textbox_get_x(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.box.x;
  }

  gs_scalar gui_textbox_get_y(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return tex.box.y;
  }

  string gui_textbox_get_text(int id){
    get_elementv(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,"");
    return tex.get_text();
  }

  ///Drawers
  void gui_textbox_draw(int id){
    get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    gui::font_style psty = gui::get_current_draw_state();
    tex.update();
    tex.draw();
    gui::set_current_draw_state(psty);
  }

  void gui_textbox_draw(){
    gui::font_style psty = gui::get_current_draw_state();
    for (auto &b : gui::gui_elements){
      ///TODO(harijs) - THIS NEEDS TO BE A LOT PRETTIER (now it does lookup twice)
      if (b.second.type == gui::GUI_TYPE::TEXTBOX){
        get_element(tex,gui::Textbox,gui::GUI_TYPE::TEXTBOX,b.first);
        if (tex.visible == true && tex.parent_id == -1){
          tex.update();
          tex.draw();
        }
      }
    }
    gui::set_current_draw_state(psty);
  }

  ///Parenting
  void gui_textbox_add_textbox(int id, int tid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.textbox_add(tid);
  }

  void gui_textbox_add_button(int id, int bid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.button_add(bid);
  }

  void gui_textbox_add_toggle(int id, int tid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.toggle_add(tid);
  }

  void gui_textbox_add_slider(int id, int sid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.slider_add(sid);
  }

  void gui_textbox_add_scrollbar(int id, int sid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.scrollbar_add(sid);
  }

  void gui_textbox_add_label(int id, int lid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.label_add(lid);
  }

  void gui_textbox_add_window(int id, int wid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.window_add(wid);
  }

  void gui_textbox_remove_textbox(int id, int tid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.textbox_remove(tid);
  }

  void gui_textbox_remove_button(int id, int bid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.button_remove(bid);
  }

  void gui_textbox_remove_toggle(int id, int tid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.toggle_remove(tid);
  }

  void gui_textbox_remove_slider(int id, int sid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.slider_remove(sid);

  }

  void gui_textbox_remove_scrollbar(int id, int sid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.scrollbar_remove(sid);
  }

  void gui_textbox_remove_label(int id, int lid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.label_remove(lid);
  }

  void gui_textbox_remove_window(int id, int wid){
    get_element(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id);
    ele.parenter.window_remove(wid);
  }

  int gui_textbox_get_textbox_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.textbox_count();
  }

  int gui_textbox_get_button_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.button_count();
  }

  int gui_textbox_get_toggle_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.toggle_count();
  }

  int gui_textbox_get_slider_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.slider_count();
  }

  int gui_textbox_get_scrollbar_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.scrollbar_count();
  }

  int gui_textbox_get_label_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.label_count();
  }

  int gui_textbox_get_window_count(int id){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.window_count();
  }

  ///GETTERS FOR ELEMENTS
  int gui_textbox_get_textbox(int id, int tex){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.textbox(tex);
  }

  int gui_textbox_get_button(int id, int but){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.button(but);
  }

  int gui_textbox_get_toggle(int id, int tog){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.toggle(tog);
  }

  int gui_textbox_get_slider(int id, int sli){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.slider(sli);
  }

  int gui_textbox_get_scrollbar(int id, int scr){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.scrollbar(scr);
  }

  int gui_textbox_get_label(int id, int lab){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.label(lab);
  }

  int gui_textbox_get_window(int id, int wid){
    get_elementv(ele,gui::Textbox,gui::GUI_TYPE::TEXTBOX,id,-1);
    return ele.parenter.window(wid);
  }
}
