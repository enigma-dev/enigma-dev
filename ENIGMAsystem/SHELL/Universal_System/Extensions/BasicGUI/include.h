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
#include "buttons.h"
#include "skins.h"

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
	  gui_state_all
	};

  ///BUTTONS
	int gui_button_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_button_create();

	void gui_button_destroy(int id);
	void gui_button_set_text(int id, string text);
	void gui_button_set_position(int id, gs_scalar x, gs_scalar y);

	//Font Style changes
	void gui_button_set_font(int id, int state, int font);
	void gui_button_set_font_color(int id, int state, int color);
	void gui_button_set_font_alpha(int id, int state, gs_scalar alpha);
	void gui_button_set_font_halign(int id, int state, unsigned int halign);
	void gui_button_set_font_valign(int id, int state, unsigned int valign);

	void gui_button_set_sprite(int id, int state, int sprid);

	void gui_button_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_button_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_button_set_size(int id, gs_scalar w, gs_scalar h);

	void gui_button_set_callback(int id, int script_id);
	void gui_button_set_togglable(int id, bool togglable);
	void gui_button_set_visible(int id, bool visible);

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

	//Font Style changes
	void gui_toggle_set_font(int id, int state, int font);
	void gui_toggle_set_font_color(int id, int state, int color);
	void gui_toggle_set_font_alpha(int id, int state, gs_scalar alpha);
	void gui_toggle_set_font_halign(int id, int state, unsigned int halign);
	void gui_toggle_set_font_valign(int id, int state, unsigned int valign);

	void gui_toggle_set_sprite(int id, int state, int sprid);

	void gui_toggle_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_toggle_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_toggle_set_size(int id, gs_scalar w, gs_scalar h);

	void gui_toggle_set_callback(int id, int script_id);
	void gui_toggle_set_visible(int id, bool visible);

	int gui_toggle_get_state(int id);
	bool gui_toggle_get_active(int id);
	void gui_toggle_draw(int id);
	void gui_toggle_draw();


	///Windows
	int gui_window_create(gs_scalar x, gs_scalar y, gs_scalar w, gs_scalar h, string text);
	int gui_window_create();

	void gui_window_destroy(int id);
	void gui_window_set_text(int id, string text);
	void gui_window_set_position(int id, gs_scalar x, gs_scalar y);

	//Font Style changes
	void gui_window_set_font(int id, int state, int font);
	void gui_window_set_font_color(int id, int state, int color);
	void gui_window_set_font_alpha(int id, int state, gs_scalar alpha);
	void gui_window_set_font_halign(int id, int state, unsigned int halign);
	void gui_window_set_font_valign(int id, int state, unsigned int valign);

	void gui_window_set_sprite(int id, int state, int sprid);

	void gui_window_set_padding(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_window_set_border(int id, gs_scalar left, gs_scalar top, gs_scalar right, gs_scalar bottom);
	void gui_window_set_size(int id, gs_scalar w, gs_scalar h);

	void gui_window_set_callback(int id, int script_id);
	void gui_window_set_visible(int id, bool visible);

	int gui_window_get_state(int id);
	void gui_window_draw(int id);
	void gui_windows_draw();

  //Window parenting
  void gui_window_add_button(int id, int bid);

	//Skins
	int gui_skin_create();
	void gui_skin_destroy(int id);
	void gui_skin_set(int id);
	int gui_skin_get_button(int id);
  int gui_skin_get_window(int id);
}

#endif
