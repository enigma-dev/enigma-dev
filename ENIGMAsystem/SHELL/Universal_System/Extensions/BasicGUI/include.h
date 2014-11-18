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

#include <unordered_map>
using std::unordered_map;
using std::pair;

namespace enigma_user
{
	enum {
	  gui_state_default,
	  gui_state_on,
	  gui_state_hover,
	  gui_state_active,
	  gui_state_on_hover,
	  gui_state_on_active,
	  gui_state_all
	};

  ///BUTTONS
	int gui_button_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_button_create();

	void gui_button_destroy(int id);
	void gui_button_set_text(int id, string text);
	void gui_button_set_position(int id, gs_scalar x, gs_scalar y);

  void gui_button_set_size(int id, gs_scalar w, gs_scalar h);
  void gui_button_set_style(int id, int style_id);

	void gui_button_set_callback(int id, int script_id);
	void gui_button_set_togglable(int id, bool togglable);
	void gui_button_set_visible(int id, bool visible);
  void gui_button_set_active(int id, bool active);

  int gui_button_get_style(int id);
	int gui_button_get_state(int id);
	bool gui_button_get_active(int id);
	void gui_button_draw(int id);
	void gui_buttons_draw();

  ///TOGGLES
	int gui_toggle_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_toggle_create();

	void gui_toggle_destroy(int id);
	void gui_toggle_set_text(int id, string text);
	void gui_toggle_set_position(int id, gs_scalar x, gs_scalar y);

	void gui_toggle_set_size(int id, gs_scalar w, gs_scalar h);
  void gui_toggle_set_style(int id, int style_id);

	void gui_toggle_set_callback(int id, int script_id);
	void gui_toggle_set_visible(int id, bool visible);
  void gui_toggle_set_active(int id, bool active);

  int gui_toggle_get_style(int id);
	int gui_toggle_get_state(int id);
	bool gui_toggle_get_active(int id);
	void gui_toggle_draw(int id);
	void gui_toggles_draw();

  ///SLIDERS
  int gui_slider_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, gs_scalar ind_x, gs_scalar ind_y, gs_scalar ind_w, gs_scalar ind_h, double val, double minVal, double maxVal, double incrVal, string text);
	int gui_slider_create();

	void gui_slider_destroy(int id);
	void gui_slider_set_text(int id, string text);
	void gui_slider_set_position(int id, gs_scalar x, gs_scalar y);


	void gui_slider_set_size(int id, gs_scalar w, gs_scalar h);
  void gui_slider_set_style(int id, int style_id);
  void gui_slider_set_indicator_style(int id, int indicator_style_id);

	void gui_slider_set_callback(int id, int script_id);
	void gui_slider_set_visible(int id, bool visible);
  void gui_slider_set_active(int id, bool active);

  int gui_slider_get_style(int id);
  int gui_slider_get_indicator_style(int id);

	int gui_slider_get_state(int id);
	bool gui_slider_get_active(int id);
  double gui_slider_get_value(int id);
	void gui_slider_draw(int id);
	void gui_sliders_draw();


	///Windows
	int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_window_create();

	void gui_window_destroy(int id);
	void gui_window_set_text(int id, string text);
	void gui_window_set_position(int id, gs_scalar x, gs_scalar y);

	void gui_window_set_size(int id, gs_scalar w, gs_scalar h);
  void gui_window_set_style(int id, int style_id);

	void gui_window_set_callback(int id, int script_id);
	void gui_window_set_visible(int id, bool visible);
  void gui_window_set_draggable(int id, bool draggable);

  int gui_window_get_style(int id);
	int gui_window_get_state(int id);
	void gui_window_draw(int id);
	void gui_windows_draw();

  ///LABELS
	int gui_label_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_label_create();

	void gui_label_destroy(int id);
	void gui_label_set_text(int id, string text);
	void gui_label_set_position(int id, gs_scalar x, gs_scalar y);

  void gui_label_set_size(int id, gs_scalar w, gs_scalar h);
  void gui_label_set_style(int id, int style_id);
	void gui_label_set_visible(int id, bool visible);

  int gui_label_get_style(int id);
	void gui_label_draw(int id);
	void gui_labels_draw();

  ///GROUPS
	int gui_group_create();
  void gui_group_destroy(int id);
  void gui_group_add_button(int id, int bid);
  void gui_group_add_toggle(int id, int tid);

  //Window parenting
  void gui_window_add_button(int id, int bid);
  void gui_window_add_toggle(int id, int tid);
  void gui_window_add_slider(int id, int sid);
  void gui_window_add_label(int id, int sid);

	//Skins
	int gui_skin_create();
	void gui_skin_destroy(int id);
	void gui_skin_set(int id);
	int gui_skin_get_button(int id);
  int gui_skin_get_window(int id);
  int gui_skin_get_toggle(int id);
  int gui_skin_get_slider(int id);
  int gui_skin_get_label(int id);

  //Styles
  int gui_style_create();
  void gui_style_destroy(int id);
  void gui_style_set_font(int id, int state, int font);
	void gui_style_set_font_halign(int id, int state, unsigned int halign);
	void gui_style_set_font_valign(int id, int state, unsigned int valign);
	void gui_style_set_font_color(int id, int state, int color);
	void gui_style_set_font_alpha(int id, int state, gs_scalar alpha);
	void gui_style_set_sprite(int id, int state, int sprid);
	void gui_style_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_style_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
  void gui_style_set_image_offset(int id, gs_scalar x, gs_scalar y);
}

#endif
