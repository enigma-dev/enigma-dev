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

#ifndef BGUI_INCLUDE_H
#define BGUI_INCLUDE_H

#include "Universal_System/scalar.h"
#include "Universal_System/var4.h"

#ifndef JUST_DEFINE_IT_RUN
#include "elements.h"
#endif

namespace enigma_user {
///NOTE: CALLBACKS AND STYLES CAN BE A SECURITY RISK IF STATE IS NOT THE DEFINED ENUM's. NO CHECKING TAKES PLACE DURING RUNTIME
enum {
  gui_error = static_cast<int>(enigma::gui::GUI_TYPE::ERROR),
  gui_button = static_cast<int>(enigma::gui::GUI_TYPE::BUTTON),
  gui_toggle = static_cast<int>(enigma::gui::GUI_TYPE::TOGGLE),
  gui_slider = static_cast<int>(enigma::gui::GUI_TYPE::SLIDER),
  gui_skin = static_cast<int>(enigma::gui::GUI_TYPE::SKIN),
  gui_window = static_cast<int>(enigma::gui::GUI_TYPE::WINDOW),
  gui_label = static_cast<int>(enigma::gui::GUI_TYPE::LABEL),
  gui_group = static_cast<int>(enigma::gui::GUI_TYPE::GROUP),
  gui_scrollbar = static_cast<int>(enigma::gui::GUI_TYPE::SCROLLBAR),
  gui_style = static_cast<int>(enigma::gui::GUI_TYPE::STYLE),
  gui_textbox = static_cast<int>(enigma::gui::GUI_TYPE::TEXTBOX)
};

enum {
  gui_state_default,
  gui_state_on,
  gui_state_hover,
  gui_state_active,
  gui_state_on_hover,
  gui_state_on_active,
  gui_state_all
};

enum {
  gui_event_pressed,
  gui_event_released,
  gui_event_hover,
  gui_event_drag,
  gui_event_resize,
  gui_event_change,
  gui_event_all
};

///BUTTONS
int gui_button_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
int gui_button_create();

int gui_button_duplicate(int id);
void gui_button_destroy(int id);

//Setters
void gui_button_set_text(int id, string text);
void gui_button_set_position(int id, gs_scalar x, gs_scalar y);

void gui_button_set_size(int id, gs_scalar w, gs_scalar h);
void gui_button_set_style(int id, int style_id);

void gui_button_set_callback(int id, int event, int script_id);
void gui_button_set_togglable(int id, bool togglable);
void gui_button_set_visible(int id, bool visible);
void gui_button_set_active(int id, bool active);

//Getters
int gui_button_get_style(int id);
int gui_button_get_state(int id);
int gui_button_get_callback(int id, int event);
int gui_button_get_parent(int id);
bool gui_button_get_active(int id);
bool gui_button_get_visible(int id);
bool gui_button_get_togglable(int id);
gs_scalar gui_button_get_width(int id);
gs_scalar gui_button_get_height(int id);
gs_scalar gui_button_get_x(int id);
gs_scalar gui_button_get_y(int id);
string gui_button_get_text(int id);

//Drawers
void gui_button_draw(int id);
void gui_buttons_draw();

//Button parenting
void gui_button_add_button(int id, int bid);
void gui_button_add_toggle(int id, int tid);
void gui_button_add_slider(int id, int sid);
void gui_button_add_scrollbar(int id, int scr);
void gui_button_add_label(int id, int lid);
void gui_button_add_window(int id, int wid);

void gui_button_remove_button(int id, int bid);
void gui_button_remove_toggle(int id, int tid);
void gui_button_remove_slider(int id, int sid);
void gui_button_remove_scrollbar(int id, int scr);
void gui_button_remove_label(int id, int lid);
void gui_button_remove_window(int id, int wid);

int gui_button_get_button_count(int id);
int gui_button_get_toggle_count(int id);
int gui_button_get_slider_count(int id);
int gui_button_get_scrollbar_count(int id);
int gui_button_get_label_count(int id);
int gui_button_get_window_count(int id);

int gui_button_get_button(int id, int but);
int gui_button_get_toggle(int id, int tog);
int gui_button_get_slider(int id, int sli);
int gui_button_get_scrollbar(int id, int scr);
int gui_button_get_label(int id, int lab);
int gui_button_get_window(int id, int wid);

///TOGGLES
int gui_toggle_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
int gui_toggle_create();
int gui_toggle_duplicate(int id);
void gui_toggle_destroy(int id);

//Setters
void gui_toggle_set_text(int id, string text);
void gui_toggle_set_position(int id, gs_scalar x, gs_scalar y);

void gui_toggle_set_size(int id, gs_scalar w, gs_scalar h);
void gui_toggle_set_style(int id, int style_id);

void gui_toggle_set_callback(int id, int event, int script_id);
void gui_toggle_set_visible(int id, bool visible);
void gui_toggle_set_active(int id, bool active);

//Getters
int gui_toggle_get_style(int id);
int gui_toggle_get_state(int id);
int gui_toggle_get_callback(int id, int event);
int gui_toggle_get_parent(int id);
bool gui_toggle_get_active(int id);
bool gui_toggle_get_visible(int id);
gs_scalar gui_toggle_get_width(int id);
gs_scalar gui_toggle_get_height(int id);
gs_scalar gui_toggle_get_x(int id);
gs_scalar gui_toggle_get_y(int id);
string gui_toggle_get_text(int id);

//Drawers
void gui_toggle_draw(int id);
void gui_toggles_draw();

///SLIDERS
int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                      gs_scalar ind_w, gs_scalar ind_h, string text);
int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                      gs_scalar ind_w, gs_scalar ind_h, double val, double minVal, double maxVal, double incrVal,
                      string text);
int gui_slider_create();
int gui_slider_duplicate(int id);
void gui_slider_destroy(int id);

//Setters
void gui_slider_set_text(int id, string text);
void gui_slider_set_position(int id, gs_scalar x, gs_scalar y);

void gui_slider_set_size(int id, gs_scalar w, gs_scalar h);
void gui_slider_set_style(int id, int style_id);
void gui_slider_set_indicator_style(int id, int indicator_style_id);

void gui_slider_set_callback(int id, int event, int script_id);
void gui_slider_set_visible(int id, bool visible);
void gui_slider_set_active(int id, bool active);
void gui_slider_set_value(int id, double value);
void gui_slider_set_minvalue(int id, double minvalue);
void gui_slider_set_maxvalue(int id, double maxvalue);
void gui_slider_set_incvalue(int id, double incvalue);

//Getters
int gui_slider_get_style(int id);
int gui_slider_get_indicator_style(int id);

int gui_slider_get_state(int id);
int gui_slider_get_callback(int id, int event);
bool gui_slider_get_active(int id);
bool gui_slider_get_visible(int id);
gs_scalar gui_slider_get_width(int id);
gs_scalar gui_slider_get_height(int id);
gs_scalar gui_slider_get_x(int id);
gs_scalar gui_slider_get_y(int id);
gs_scalar gui_slider_get_indicator_width(int id);
gs_scalar gui_slider_get_indicator_height(int id);
gs_scalar gui_slider_get_indicator_x(int id);
gs_scalar gui_slider_get_indicator_y(int id);
string gui_slider_get_text(int id);

double gui_slider_get_value(int id);
double gui_slider_get_minvalue(int id);
double gui_slider_get_maxvalue(int id);
double gui_slider_get_incvalue(int id);

//Drawers
void gui_slider_draw(int id);
void gui_sliders_draw();

///SCROLLBARS
int gui_scrollbar_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y,
                         bool direction = false, double val = 0.0, double minVal = 0.0, double maxVal = 1.0,
                         double incrVal = 0.0, gs_scalar size = 0.1);
int gui_scrollbar_create();
int gui_scrollbar_duplicate(int id);
void gui_scrollbar_destroy(int id);

//Setters
void gui_scrollbar_set_position(int id, gs_scalar x, gs_scalar y);

void gui_scrollbar_set_size(int id, gs_scalar w, gs_scalar h);
void gui_scrollbar_set_style(int id, int style_id);
void gui_scrollbar_set_indicator_style(int id, int indicator_style_id);

void gui_scrollbar_set_callback(int id, int event, int script_id);
void gui_scrollbar_set_visible(int id, bool visible);
void gui_scrollbar_set_active(int id, bool active);
void gui_scrollbar_set_value(int id, double value);
void gui_scrollbar_set_minvalue(int id, double minvalue);
void gui_scrollbar_set_maxvalue(int id, double maxvalue);
void gui_scrollbar_set_incvalue(int id, double incvalue);
void gui_scrollbar_set_indicator_size(int id, gs_scalar size);
void gui_scrollbar_set_direction(int id, bool direction);

//Getters
int gui_slider_scrollbar_style(int id);
int gui_slider_scrollbar_indicator_style(int id);

int gui_slider_scrollbar_state(int id);
int gui_slider_scrollbar_callback(int id, int event);
bool gui_slider_scrollbar_active(int id);
bool gui_slider_scrollbar_visible(int id);
gs_scalar gui_scrollbar_get_width(int id);
gs_scalar gui_scrollbar_get_height(int id);
gs_scalar gui_scrollbar_get_x(int id);
gs_scalar gui_scrollbar_get_y(int id);
gs_scalar gui_scrollbar_get_indicator_width(int id);
gs_scalar gui_scrollbar_get_indicator_height(int id);
gs_scalar gui_scrollbar_get_indicator_x(int id);
gs_scalar gui_scrollbar_get_indicator_y(int id);
gs_scalar gui_scrollbar_get_indicator_size(int id);

double gui_scrollbar_get_value(int id);
double gui_scrollbar_get_minvalue(int id);
double gui_scrollbar_get_maxvalue(int id);
double gui_scrollbar_get_incvalue(int id);
bool gui_scrollbar_get_direction(int id);

//Drawers
void gui_scrollbar_draw(int id);
void gui_scrollbars_draw();

///TEXTBOXES
int gui_textbox_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
int gui_textbox_create();

int gui_textbox_duplicate(int id);
void gui_textbox_destroy(int id);

//Setters
void gui_textbox_set_text(int id, string text);
void gui_textbox_set_position(int id, gs_scalar x, gs_scalar y);

void gui_textbox_set_size(int id, gs_scalar w, gs_scalar h);
void gui_textbox_set_style(int id, int style_id);
void gui_textbox_set_marker_style(int id, int style_id);
void gui_textbox_set_numeric(int id, bool numeric);
void gui_textbox_set_callback(int id, int event, int script_id);
void gui_textbox_set_togglable(int id, bool togglable);
void gui_textbox_set_visible(int id, bool visible);
void gui_textbox_set_active(int id, bool active);
void gui_textbox_set_max_length(int id, int length);
void gui_textbox_set_max_lines(int id, int lines);

//Getters
int gui_textbox_get_style(int id);
int gui_textbox_get_state(int id);
int gui_textbox_get_callback(int id, int event);
int gui_textbox_get_parent(int id);
bool gui_textbox_get_active(int id);
bool gui_textbox_get_visible(int id);
bool gui_textbox_get_togglable(int id);
bool gui_textbox_get_numeric(int id);

gs_scalar gui_textbox_get_width(int id);
gs_scalar gui_textbox_get_height(int id);
gs_scalar gui_textbox_get_x(int id);
gs_scalar gui_textbox_get_y(int id);
string gui_textbox_get_text(int id);

//Drawers
void gui_textbox_draw(int id);
void gui_textboxs_draw();

//textbox parenting
void gui_textbox_add_buton(int id, int bid);
void gui_textbox_add_textbox(int id, int tid);
void gui_textbox_add_toggle(int id, int tid);
void gui_textbox_add_slider(int id, int sid);
void gui_textbox_add_scrollbar(int id, int scr);
void gui_textbox_add_label(int id, int lid);
void gui_textbox_add_window(int id, int wid);

void gui_textbox_remove_button(int id, int bid);
void gui_textbox_remove_textbox(int id, int tid);
void gui_textbox_remove_toggle(int id, int tid);
void gui_textbox_remove_slider(int id, int sid);
void gui_textbox_remove_scrollbar(int id, int scr);
void gui_textbox_remove_label(int id, int lid);
void gui_textbox_remove_window(int id, int wid);

int gui_textbox_get_button_count(int id);
int gui_textbox_get_textbox_count(int id);
int gui_textbox_get_toggle_count(int id);
int gui_textbox_get_slider_count(int id);
int gui_textbox_get_scrollbar_count(int id);
int gui_textbox_get_label_count(int id);
int gui_textbox_get_window_count(int id);

int gui_textbox_get_button(int id, int but);
int gui_textbox_get_textbox(int id, int tex);
int gui_textbox_get_toggle(int id, int tog);
int gui_textbox_get_slider(int id, int sli);
int gui_textbox_get_scrollbar(int id, int scr);
int gui_textbox_get_label(int id, int lab);
int gui_textbox_get_window(int id, int wid);

///Windows
int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
int gui_window_create();
int gui_window_duplicate(int id);
void gui_window_destroy(int id);

//Setters
void gui_window_set_style(int id, int style_id);
void gui_window_set_stencil_style(int id, int style_id);
void gui_window_set_callback(int id, int event, int script_id);
void gui_window_set_draggable(int id, bool draggable);
void gui_window_set_resizable(int id, bool resizable);
void gui_window_set_visible(int id, bool visible);

void gui_window_set_position(int id, gs_scalar x, gs_scalar y);
void gui_window_set_size(int id, gs_scalar w, gs_scalar h);
void gui_window_set_minsize(int id, gs_scalar w, gs_scalar h);

void gui_window_set_text(int id, string text);
void gui_window_set_state(int id, int state);
void gui_window_set_stencil_mask(int id, bool stencil);

//Getters
int gui_window_get_style(int id);
int gui_window_get_stencil_style(int id);
int gui_window_get_state(int id);
int gui_window_get_callback(int id, int event);
bool gui_window_get_draggable(int id);
bool gui_window_get_resizable(int id);
bool gui_window_get_visible(int id);
gs_scalar gui_window_get_width(int id);
gs_scalar gui_window_get_height(int id);
gs_scalar gui_window_get_x(int id);
gs_scalar gui_window_get_y(int id);
string gui_window_get_text(int id);
bool gui_window_get_stencil_mask(int id);

//depth changers
void gui_window_push_to_front(int id);
void gui_window_push_to_back(int id);
void gui_window_group_push_to_front(int gid);
void gui_window_group_push_to_back(int gid);

//Updaters
bool gui_windows_update();
bool gui_windows_group_update(int gid, bool continueProp = false);

//Drawers
void gui_window_draw(int id);
void gui_windows_draw();
void gui_windows_group_draw(int gid);

//Window parenting
void gui_window_add_button(int id, int bid);
void gui_window_add_toggle(int id, int tid);
void gui_window_add_slider(int id, int sid);
void gui_window_add_scrollbar(int id, int scr);
void gui_window_add_label(int id, int lid);
void gui_window_add_window(int id, int wid);
void gui_window_add_textbox(int id, int tex);

void gui_window_remove_button(int id, int bid);
void gui_window_remove_toggle(int id, int tid);
void gui_window_remove_slider(int id, int sid);
void gui_window_remove_scrollbar(int id, int scr);
void gui_window_remove_label(int id, int lid);
void gui_window_remove_window(int id, int wid);
void gui_window_remove_textbox(int id, int tex);

int gui_window_get_button_count(int id);
int gui_window_get_toggle_count(int id);
int gui_window_get_slider_count(int id);
int gui_window_get_scrollbar_count(int id);
int gui_window_get_label_count(int id);
int gui_window_get_window_count(int id);
int gui_window_get_textbox_count(int id);

int gui_window_get_button(int id, int but);
int gui_window_get_toggle(int id, int tog);
int gui_window_get_slider(int id, int sli);
int gui_window_get_scrollbar(int id, int scr);
int gui_window_get_label(int id, int lab);
int gui_window_get_window(int id, int wid);
int gui_window_get_textbox(int id, int tex);

///LABELS
int gui_label_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
int gui_label_create();
int gui_label_duplicate(int id);
void gui_label_destroy(int id);

//Setters
void gui_label_set_text(int id, string text);
void gui_label_set_position(int id, gs_scalar x, gs_scalar y);

void gui_label_set_size(int id, gs_scalar w, gs_scalar h);
void gui_label_set_style(int id, int style_id);
void gui_label_set_visible(int id, bool visible);

//Getters
int gui_label_get_style(int id);
bool gui_label_get_visible(int id);
gs_scalar gui_label_get_width(int id);
gs_scalar gui_label_get_height(int id);
gs_scalar gui_label_get_x(int id);
gs_scalar gui_label_get_y(int id);
string gui_label_get_text(int id);
int gui_label_get_parent(int id);

//Drawers
void gui_label_draw(int id);
void gui_labels_draw();

///GROUPS
int gui_group_create();
int gui_group_duplicate(int group);
void gui_group_destroy(int id);
void gui_group_add_button(int id, int bid);
void gui_group_add_toggle(int id, int tid);
void gui_group_add_window(int id, int wid);
void gui_group_remove_button(int id, int bid);
void gui_group_remove_toggle(int id, int tid);
void gui_group_remove_window(int id, int wid);

//Skins
int gui_skin_create();
int gui_skin_duplicate(int skin);
void gui_skin_destroy(int id);
void gui_skin_set(int id);
int gui_skin_get_button(int id);
int gui_skin_get_window(int id);
int gui_skin_get_toggle(int id);
int gui_skin_get_slider(int id);
int gui_skin_get_label(int id);
int gui_skin_get_scrollbar(int id);
int gui_skin_get_textbox(int id);

//Styles
int gui_style_create();
int gui_style_duplicate(int style);
void gui_style_destroy(int id);
void gui_style_set_font(int id, int state, int font);
void gui_style_set_font_halign(int id, int state, unsigned int halign);
void gui_style_set_font_valign(int id, int state, unsigned int valign);
void gui_style_set_font_color(int id, int state, int color);
void gui_style_set_font_alpha(int id, int state, gs_scalar alpha);

void gui_style_set_sprite(int id, int state, int sprid);
void gui_style_set_sprite_color(int id, int state, int color);
void gui_style_set_sprite_alpha(int id, int state, gs_scalar alpha);
void gui_style_set_sprite_direction(int id, int state, gs_scalar direction);
void gui_style_set_sprite_scale(int id, int state, gs_scalar scale_x, gs_scalar scale_y);

void gui_style_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
void gui_style_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
void gui_style_set_image_offset(int id, gs_scalar x, gs_scalar y);
void gui_style_set_box(int id, gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h);

int gui_element_get_type(int ele);

void gui_continue_propagation();
}  //namespace enigma_user

#endif
