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
#include "Platforms/General/PFwindow.h" //For mouse_check_button
#include "Universal_System/Resources/resource_data.h" //For script_execute
//#include "Universal_System/Resources/sprites_internal.h"
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
  extern unsigned int gui_data_elements_maxid;
  extern unordered_map<unsigned int, Element> gui_elements;
  extern unordered_map<unsigned int, DataElement> gui_data_elements;

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
  void Window::update(gs_scalar ox, gs_scalar oy, gs_scalar tx, gs_scalar ty){
    if (visible == false) return;
    if (parent_id != -1){ //This stops update_children() from being called twice (once in gui_window/s_draw() and once here)
      parenter.update_children(ox+box.x, oy+box.y);
    }
    gs_scalar txox = tx-ox;
    gs_scalar tyoy = ty-oy;
    if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left)){ //Press
      if (windowStopPropagation == false && box.point_inside(txox,tyoy)){
        state = enigma_user::gui_state_on;
        callback_execute(enigma_user::gui_event_pressed);
        if (draggable == true){
          drag = true;
          drag_xoffset = txox-box.x;
          drag_yoffset = tyoy-box.y;
        }
      }else{
        state = enigma_user::gui_state_default;
      }
    }

    if (windowStopPropagation == false && box.point_inside(txox,tyoy)){ //Hover or resize
      if (resizable == true){
        rect box_left(box.x,box.y,20,box.h);
        rect box_top(box.x,box.y,box.w,20);
        rect box_right(box.w+box.x-20,box.y,20,box.h);
        rect box_bottom(box.x,box.y+box.h-20,box.w,20);

        //Resize
        if (enigma_user::mouse_check_button_pressed(enigma_user::mb_left) && (box_left.point_inside(txox,tyoy) || box_top.point_inside(txox,tyoy) || box_right.point_inside(txox,tyoy) || box_bottom.point_inside(txox,tyoy))){ //Press
          if (box_bottom.point_inside(txox,tyoy)){ //Check bottom and corners
            if (box_left.point_inside(txox,tyoy)){ resize_side = 7; }
            else if (box_right.point_inside(txox,tyoy)){ resize_side = 5; }
            else { resize_side = 6; }
          } else if (box_top.point_inside(txox, tyoy)){ //Check top and corners
            if (box_left.point_inside(txox,tyoy)){ resize_side = 1; }
            else if (box_right.point_inside(txox,tyoy)){ resize_side = 3; }
            else { resize_side = 2; }
          } else if (box_left.point_inside(txox,tyoy)){ //Check left
            resize_side = 0;
          } else if (box_right.point_inside(txox,tyoy)){ //Check right
            resize_side = 4;
          }
          drag_xoffset = txox-box.x;
          drag_yoffset = tyoy-box.y;
          resize_xoffset = txox;
          resize_yoffset = tyoy;
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
          box.w = fmax(resize_width - (txox - resize_xoffset), min_box.w);
          box.x = fmin(txox-drag_xoffset, resize_xoffset+resize_width-min_box.w-drag_xoffset);
        } break;
        case 1: { //Resizing top-left
          box.w = fmax(resize_width - (txox - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (tyoy - resize_yoffset), min_box.h);
          box.x = fmin(txox-drag_xoffset, resize_xoffset+resize_width-min_box.w-drag_xoffset);
          box.y = fmin(tyoy-drag_yoffset, resize_yoffset+resize_height-min_box.h-drag_yoffset);
        } break;
        case 2: { //Resizing top
          box.h = fmax(resize_height - (tyoy - resize_yoffset), min_box.h);
          box.y = fmin(tyoy-drag_yoffset, resize_yoffset+resize_height-min_box.h-drag_yoffset);
        } break;
        case 3: { //Resizing top-right
          box.w = fmax(resize_width + (txox - resize_xoffset), min_box.w);
          box.h = fmax(resize_height - (tyoy - resize_yoffset), min_box.h);
          box.y = fmin(tyoy-drag_yoffset, resize_yoffset+resize_height-min_box.h-drag_yoffset);
        } break;
        case 4: { //Resizing right side
          box.w = fmax(resize_width + (txox - resize_xoffset), min_box.w);
        } break;
        case 5: { //Resizing bottom-right
          box.w = fmax(resize_width + (txox - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (tyoy - resize_yoffset), min_box.h);
        } break;
        case 6: { //Resizing bottom
          box.h = fmax(resize_height + (tyoy - resize_yoffset), min_box.h);
        } break;
        case 7: { //Resizing bottom-left
          box.w = fmax(resize_width - (txox - resize_xoffset), min_box.w);
          box.h = fmax(resize_height + (tyoy - resize_yoffset), min_box.h);
          box.x = fmin(txox-drag_xoffset, resize_xoffset+resize_width-min_box.w-drag_xoffset);
        } break;
      }
      callback_execute(enigma_user::gui_event_resize);
      if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
        resize = false;
        callback_execute(enigma_user::gui_event_released);
      }
    }else if (drag == true){
      windowStopPropagation = true;
      box.x = txox-drag_xoffset;
      box.y = tyoy-drag_yoffset;
      callback_execute(enigma_user::gui_event_drag);
      if (enigma_user::mouse_check_button_released(enigma_user::mb_left)){
        drag = false;
        callback_execute(enigma_user::gui_event_released);
      }
    }
  }

  void Window::draw(gs_scalar ox, gs_scalar oy){
    if (visible == false) return;
    //Draw window
    get_data_element(sty,gui::Style,gui::GUI_TYPE::STYLE,style_id);
    if (sty.sprites[state] != -1){
      rect &b = (sty.box.zero == true ? box : sty.box);
      if (sty.border.zero == true){
        enigma_user::draw_sprite_stretched(sty.sprites[state],-1,
                                         ox + b.x,
                                         oy + b.y,
                                         b.w,
                                         b.h,
                                         sty.sprite_styles[state].color,
                                         sty.sprite_styles[state].alpha);

      }else{
        enigma_user::draw_sprite_padded(sty.sprites[state],-1,
                                      sty.border.left,
                                      sty.border.top,
                                      sty.border.right,
                                      sty.border.bottom,
                                      ox + b.x,
                                      oy + b.y,
                                      ox + b.x+b.w,
                                      oy + b.y+b.h,
                                      sty.sprite_styles[state].color,
                                      sty.sprite_styles[state].alpha);
      }
    }

    //Draw text
    if (text.empty() == false){
      sty.font_styles[state].use();

      gs_scalar textx = 0.0, texty = 0.0;
      switch (sty.font_styles[state].halign){
        case enigma_user::fa_left: textx = ox+box.x+sty.padding.left; break;
        case enigma_user::fa_center: textx = ox+box.x+box.w/2.0; break;
        case enigma_user::fa_right: textx = ox+box.x+box.w-sty.padding.right; break;
      }

      switch (sty.font_styles[state].valign){
        case enigma_user::fa_top: texty = oy+box.y+sty.padding.top; break;
        case enigma_user::fa_middle: texty = oy+box.y+box.h/2.0; break;
        case enigma_user::fa_bottom: texty = oy+box.y+box.h-sty.padding.bottom; break;
      }

      enigma_user::draw_text(textx,texty,text);
    }

    get_data_element(ssty,gui::Style,gui::GUI_TYPE::STYLE,(stencil_style_id==-1?style_id:stencil_style_id));
    if (stencil_mask == true && ssty.sprites[state] != -1){
      ///TODO(harijs) - parent window is looked up twice here!
      ///TODO(harijs) - Stenciling doesn't work when a child window with stencil mask enabled is inside a window with also stencil mask enabled
      ///               To fix this the stencil functions need to be changed, so they incr/decr and compare equal/more/less
      if (parent_id != -1){
        get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,parent_id);
        if (win.stencil_mask == false){
          enigma_user::d3d_stencil_start_mask();
        }else{
          enigma_user::d3d_stencil_continue_mask();
        }
      }else{
        enigma_user::d3d_stencil_start_mask();
      }
      gs_scalar stxoff = box.x+ox, styoff = box.y+oy;
      if (ssty.box.zero == false){
        stxoff += ssty.box.x;
        styoff += ssty.box.y;
      }
      rect &b = (ssty.box.zero == true ? box : ssty.box);
      if (ssty.border.zero == true){
        enigma_user::draw_sprite_stretched(ssty.sprites[state],-1,
                                         stxoff,
                                         styoff,
                                         b.w,
                                         b.h,
                                         ssty.sprite_styles[state].color,
                                         ssty.sprite_styles[state].alpha);

      }else{
        enigma_user::draw_sprite_padded(ssty.sprites[state],-1,
                                      ssty.border.left,
                                      ssty.border.top,
                                      ssty.border.right,
                                      ssty.border.bottom,
                                      stxoff,
                                      styoff,
                                      stxoff+b.w,
                                      styoff+b.h,
                                      ssty.sprite_styles[state].color,
                                      ssty.sprite_styles[state].alpha);
      }
      enigma_user::d3d_stencil_use_mask();
      parenter.draw_children(ox+box.x,oy+box.y);
      if (parent_id != -1){
        get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,parent_id);
        if (win.stencil_mask == false){
          enigma_user::d3d_stencil_end_mask();
        }
      }else{
        enigma_user::d3d_stencil_end_mask();
      }
    }else{
      parenter.draw_children(ox+box.x,oy+box.y);
    }
  }

  void Window::update_text_pos(int state){
  }
}

namespace enigma_user
{
  int gui_window_create(){
    if (gui::gui_bound_skin == -1){ //Add default one
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(gui::Window(), gui::gui_elements_maxid));
    }else{
      get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
      get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,ski.window_style,-1);
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(win, gui::gui_elements_maxid));
    }
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
    win.visible = true;
    win.id = gui::gui_elements_maxid;
    win.parenter.element_id = win.id;
    gui::gui_window_order.emplace_back(gui::gui_elements_maxid);
    gui::gui_element_order.emplace(gui::gui_elements_maxid, gui::gui_elements_maxid);
    return gui::gui_elements_maxid++;
  }

  int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text){
    if (gui::gui_bound_skin == -1){ //Add default one
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(gui::Window(), gui::gui_elements_maxid));
    }else{
      get_data_elementv(ski,gui::Skin,gui::GUI_TYPE::SKIN,gui::gui_bound_skin,-1);
      get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,ski.window_style,-1);
      gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(win, gui::gui_elements_maxid));
    }
    gui::Window &win = gui::gui_elements[gui::gui_elements_maxid];
    win.visible = true;
    win.id = gui::gui_elements_maxid;
    win.box.set(x, y, w, h);
    win.text = text;
    win.parenter.element_id = win.id;
    gui::gui_window_order.emplace_back(gui::gui_elements_maxid);
    gui::gui_element_order.emplace(gui::gui_elements_maxid, gui::gui_elements_maxid);
    return gui::gui_elements_maxid++;
  }

  int gui_window_duplicate(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    gui::gui_elements.emplace(std::piecewise_construct, std::forward_as_tuple(gui::gui_elements_maxid), std::forward_as_tuple(win, gui::gui_elements_maxid));
    gui::gui_elements[gui::gui_elements_maxid].id = gui::gui_elements_maxid;
    gui::Window &w = gui::gui_elements[gui::gui_elements_maxid];
    w.id = gui::gui_elements_maxid;
    w.parent_id = -1; //We cannot duplicate parenting for now
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

  void gui_window_set_minsize(int id, gs_scalar w, gs_scalar h){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.min_box.w = w;
    win.min_box.h = h;
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
    if (style_id != -1){
      check_data_element(gui::GUI_TYPE::STYLE,style_id);
      win.style_id = style_id;
    }else{
      win.style_id = gui::gui_style_window;
    }
  }

  void gui_window_set_stencil_style(int id, int style_id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    if (style_id != -1){
      check_data_element(gui::GUI_TYPE::STYLE,style_id);
    }
    win.stencil_style_id = style_id;
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

  int gui_window_get_stencil_style(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.stencil_style_id;
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

  ///TODO(harijs) - The next two functions are awful. They are N^2 or worse, so optimizations are welcome
  void gui_window_group_push_to_front(int gid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,gid);
    if (gro.group_windows.size() == 0) return;

    std::vector<unsigned int> tmp_group;
    tmp_group.reserve(gro.group_windows.size());
    for (auto it=gui::gui_window_order.begin(); it!=gui::gui_window_order.end(); ){
      bool found = false;
      for (const auto &gw : gro.group_windows){
        if (*it == gw){
          it = gui::gui_window_order.erase(it);
          tmp_group.push_back(gw);
          found = true;
          break;
        }
      }
      if (found == false) ++it;
    }
    gui::gui_window_order.insert(gui::gui_window_order.end(), tmp_group.begin(), tmp_group.end());
  }

  void gui_window_group_push_to_back(int gid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,gid);
    if (gro.group_windows.size() == 0) return;
    std::vector<unsigned int> tmp_group;
    tmp_group.reserve(gro.group_windows.size());
    for (auto it=gui::gui_window_order.begin(); it!=gui::gui_window_order.end(); ){
      bool found = false;
      for (const auto &gw : gro.group_windows){
        if (*it == gw){
          it = gui::gui_window_order.erase(it);
          tmp_group.push_back(gw);
          found = true;
          break;
        }
      }
      if (found == false) ++it;
    }
    gui::gui_window_order.insert(gui::gui_window_order.begin(), tmp_group.begin(), tmp_group.end());
  }

  ///Updaters
  bool gui_windows_update(){
    if (gui::gui_window_order.size() == 0) return false;
    gui::windowStopPropagation = false;
    //Update loop in reverse direction
    bool window_click = false; //Something clicked in the window (or the window itself)?
    int window_swap_id = -1;
    for (int ind = gui::gui_window_order.size()-1; ind >= 0; --ind){
      int i = gui::gui_window_order[ind];
      get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,i,false);
      if (win.visible == true && win.parent_id == -1){
        //Update children
        win.parenter.update_children(win.box.x, win.box.y);

        //This checks if any of the inside elements are pressed
        if (gui::windowStopPropagation == true && window_click == false) { window_click = true; window_swap_id = ind; }
        win.update();
        //This checks for the click on the window itself. I cannot just check window propagation, because hover also stops propagation, but should not change the draw order
        if (gui::windowStopPropagation == true && window_click == false) {
          window_click = true;
          if (win.state == enigma_user::gui_state_on) { window_swap_id = ind; }
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

    return gui::windowStopPropagation;
  }

  bool gui_windows_group_update(int gid, bool continueProp){
    if (continueProp == false){ gui::windowStopPropagation = false; }
    if (gui::gui_window_order.size() == 0) return false;
    get_data_elementv(gro,gui::Group,gui::GUI_TYPE::GROUP,gid,false);
    if (gro.group_windows.size() == 0) return false;
    //Update loop in reverse direction
    bool window_click = false; //Something clicked in the window (or the window itself)?
    int window_swap_id = -1;
    ///TODO(harijs): This needs to be made more efficient.
    for (int ind = gui::gui_window_order.size()-1; ind >= 0; --ind){
      unsigned int i = gui::gui_window_order[ind];
      for (const auto &gw : gro.group_windows){
        if (i == gw){
          get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,i,false);
          if (win.visible == true && win.parent_id == -1){
            //Update children
            win.parenter.update_children(win.box.x, win.box.y);

            //This checks if any of the inside elements are pressed
            if (gui::windowStopPropagation == true && window_click == false) { window_click = true; window_swap_id = ind; }
            win.update();
            //This checks for the click on the window itself. I cannot just check window propagation, because hover also stops propagation, but should not change the draw order
            if (gui::windowStopPropagation == true && window_click == false) {
              window_click = true;
              if (win.state == enigma_user::gui_state_on) { window_swap_id = ind; }
            }
          }
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

    return gui::windowStopPropagation;
  }

  ///Drawers
  void gui_window_draw(int id){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    if (win.visible == true){
      gui::font_style psty = gui::get_current_draw_state();
      win.draw();
      gui::set_current_draw_state(psty);
    }
  }

  void gui_windows_draw(){
    if (gui::gui_window_order.size() == 0) return;
    gui::font_style psty = gui::get_current_draw_state();
    for (const auto &wi : gui::gui_window_order){
      get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,wi);
      if (win.parent_id == -1){
        win.draw();
      }
    }
    gui::set_current_draw_state(psty);
  }

  void gui_windows_group_draw(int gid){
    get_data_element(gro,gui::Group,gui::GUI_TYPE::GROUP,gid);
    if (gro.group_windows.size() == 0) return;
    gui::font_style psty = gui::get_current_draw_state();
    ///TODO(harijs): This needs to be made more efficient.
    for (const auto &wi : gui::gui_window_order){
      for (const auto &gw : gro.group_windows){
        if (wi == gw){
          get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,wi);
          if (win.parent_id == -1){
            win.draw();
          }
          break;
        }
      }
    }
    gui::set_current_draw_state(psty);
  }

  void gui_window_add_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.button_add(bid);
  }

  void gui_window_add_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.toggle_add(tid);
  }

  void gui_window_add_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.slider_add(sid);
  }

  void gui_window_add_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.scrollbar_add(sid);
  }

  void gui_window_add_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.label_add(lid);
  }

  void gui_window_add_window(int id, int wid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.window_add(wid);
  }

  void gui_window_add_textbox(int id, int tex){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.textbox_add(tex);
  }

  void gui_window_remove_button(int id, int bid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.button_remove(bid);
  }

  void gui_window_remove_toggle(int id, int tid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.toggle_remove(tid);
  }

  void gui_window_remove_slider(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.slider_remove(sid);

  }

  void gui_window_remove_scrollbar(int id, int sid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.scrollbar_remove(sid);
  }

  void gui_window_remove_label(int id, int lid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.label_remove(lid);
  }

  void gui_window_remove_window(int id, int wid){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.window_remove(wid);
  }

  void gui_window_remove_textbox(int id, int tex){
    get_element(win,gui::Window,gui::GUI_TYPE::WINDOW,id);
    win.parenter.textbox_remove(tex);
  }

  int gui_window_get_button_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.button_count();
  }

  int gui_window_get_toggle_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.toggle_count();
  }

  int gui_window_get_slider_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.slider_count();
  }

  int gui_window_get_scrollbar_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.scrollbar_count();
  }

  int gui_window_get_label_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.label_count();
  }

  int gui_window_get_window_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.window_count();
  }

  int gui_window_get_textbox_count(int id){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.textbox_count();
  }

  ///GETTERS FOR ELEMENTS
  int gui_window_get_button(int id, int but){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.button(but);
  }

  int gui_window_get_toggle(int id, int tog){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.toggle(tog);
  }

  int gui_window_get_slider(int id, int sli){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.slider(sli);
  }

  int gui_window_get_scrollbar(int id, int scr){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.scrollbar(scr);
  }

  int gui_window_get_label(int id, int lab){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.label(lab);
  }

  int gui_window_get_window(int id, int wid){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.window(wid);
  }

  int gui_window_get_textbox(int id, int tex){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id, -1);
    return win.parenter.textbox(tex);
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
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.parenter.element_count();
  }

  int gui_window_get_element(int id, int ele){
    get_elementv(win,gui::Window,gui::GUI_TYPE::WINDOW,id,-1);
    return win.parenter.element(ele);
  }

  void gui_continue_propagation(){
    gui::windowStopPropagation = false;
  }
}
