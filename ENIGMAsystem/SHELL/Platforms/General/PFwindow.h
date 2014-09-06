/** Copyright (C) 2008 Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
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

#ifndef ENIGMA_PLATFORM_WINDOW_H
#define ENIGMA_PLATFORM_WINDOW_H

namespace enigma_user
{

enum {
  cr_default    = 0,
  cr_none       = -1,
  cr_arrow      = -2,
  cr_cross      = -3,
  cr_beam       = -4,
  cr_size_nesw  = -6,
  cr_size_ns    = -7,
  cr_size_nwse  = -8,
  cr_size_we    = -9,
  cr_uparrow    = -10,
  cr_hourglass  = -11,
  cr_drag       = -12,
  cr_nodrop     = -13,
  cr_hsplit     = -14,
  cr_vsplit     = -15,
  cr_multidrag  = -16,
  cr_sqlwait    = -17,
  cr_no         = -18,
  cr_appstart   = -19,
  cr_help       = -20,
  cr_handpoint  = -21,
  cr_size_all   = -22
};

enum {
  display_landscape,
  display_landscape_flipped,
  display_portrait,
  display_portrait_flipped
};

void io_handle();
void io_clear();
void keyboard_wait();
void mouse_wait();
void keyboard_clear(const int key);
bool keyboard_check_direct(int key);
void keyboard_key_press(int key);
void keyboard_key_release(int key);
bool keyboard_get_numlock();
bool keyboard_get_capital();
bool keyboard_get_scroll();
void keyboard_set_numlock(bool on);
void keyboard_set_capital(bool on);
void keyboard_set_scroll(bool on);
void keyboard_set_map(int key1, int key2);
int keyboard_get_map(int key);
void keyboard_unset_map();
void mouse_clear(const int button);

int display_mouse_get_x();
int display_mouse_get_y();
void display_mouse_set(int x,int y);
int display_get_width();
int display_get_height();
int display_get_colordepth();
int display_get_frequency();
int display_get_orientation();
unsigned display_get_dpi_x();
unsigned display_get_dpi_y();
extern int display_aa;
void display_reset(int samples, bool vsync);
void display_reset(); //TODO: for some odd reason this has to be second or it will error if you call the one that has parameters
bool display_set_colordepth(int depth);
bool display_set_size(int w, int h);
bool display_set_frequency(int freq);
bool display_set_all(int w, int h, int freq, int bitdepth);
bool display_test_all(int w, int h, int freq, int bitdepth);
void set_synchronization(bool enable);

int window_get_x();
int window_get_y();

int window_get_width();
int window_get_height();

void window_set_caption(string caption);
string window_get_caption();
void window_set_color(int color);
int window_get_color();
void window_set_alpha(double alpha);
double window_get_alpha();
double window_get_region_scale();
void window_set_position(int x, int y);
void window_set_size(unsigned int width, unsigned int height);
void window_set_rectangle(int x, int y, int width, int height);
void window_center();
void window_default(bool center_size); // default false specified in platforms mandatory
void window_set_region_size(int w, int h, bool adaptwindow);

int window_get_region_width();
int window_get_region_height();
int window_get_region_width_scaled();
int window_get_region_height_scaled();

void window_set_minimized(bool minimized);
void window_set_visible(bool visible);
int window_get_visible();
bool window_get_stayontop();
bool window_get_sizeable();
bool window_get_showborder();
bool window_get_showicons();
bool window_get_freezeonlosefocus();
bool window_get_minimized();

int window_mouse_get_x();
int window_mouse_get_y();
int window_view_mouse_get_x(int id);
int window_view_mouse_get_y(int id);

string clipboard_get_text();
void clipboard_set_text(string text);
bool clipboard_has_text();

}

#endif
