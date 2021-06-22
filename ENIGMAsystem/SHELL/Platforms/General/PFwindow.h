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

#include "libEGMstd.h"
#include "Platforms/platforms_mandatory.h"

#include <string>

namespace enigma {

extern char mousestatus[3];
extern char last_mousestatus[3];
extern char last_keybdstatus[256];
extern char keybdstatus[256];
extern int cursorInt;
extern int windowX;
extern int windowY;

extern int window_min_width;
extern int window_max_width;
extern int window_min_height;
extern int window_max_height;

void input_initialize();
void input_push();
void input_key_down(int key);
void input_key_up(int key);

}  // namespace enigma

namespace enigma_user {

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

enum { display_landscape, display_landscape_flipped, display_portrait, display_portrait_flipped };

enum {
  mb_any    = -1,
  mb_none   =  0,
  mb_left   =  1,
  mb_right  =  2,
  mb_middle =  3
};

enum {
  vk_anykey = 1,
  vk_nokey  = 0,

  vk_left  = 37,
  vk_right = 39,
  vk_up    = 38,
  vk_down  = 40,

  vk_tab    = 9,
  vk_enter  = 13,
  vk_shift  = 16,
  vk_control= 17,
  vk_alt    = 18,
  vk_space  = 32,

  vk_numpad0 = 96,
  vk_numpad1 = 97,
  vk_numpad2 = 98,
  vk_numpad3 = 99,
  vk_numpad4 = 100,
  vk_numpad5 = 101,
  vk_numpad6 = 102,
  vk_numpad7 = 103,
  vk_numpad8 = 104,
  vk_numpad9 = 105,

  vk_multiply = 106,
  vk_add      = 107,
  vk_subtract = 109,
  vk_decimal  = 110,
  vk_divide   = 111,

  vk_f1  = 112,
  vk_f2  = 113,
  vk_f3  = 114,
  vk_f4  = 115,
  vk_f5  = 116,
  vk_f6  = 117,
  vk_f7  = 118,
  vk_f8  = 119,
  vk_f9  = 120,
  vk_f10 = 121,
  vk_f11 = 122,
  vk_f12 = 123,

  vk_backspace = 8,
  vk_escape    = 27,
  vk_pageup    = 33,
  vk_pagedown  = 34,
  vk_end       = 35,
  vk_home      = 36,
  vk_insert    = 45,
  vk_delete    = 46,

  // These are for check_direct only
  vk_lshift   = 160,
  vk_rshift   = 161,
  vk_lcontrol = 162,
  vk_rcontrol = 163,
  vk_lalt     = 164,
  vk_ralt     = 165,

  // This one's Windows only
  vk_printscreen = 42,

  //These are ENIGMA-only
  vk_caps   = 20,
  vk_scroll = 145,
  vk_pause  = 19,
  vk_lsuper = 91,
  vk_rsuper = 92
};

extern double mouse_x, mouse_y;
extern int mouse_button, mouse_lastbutton;
extern std::string keyboard_lastchar;
extern int keyboard_key;
extern int keyboard_lastkey;
extern short mouse_hscrolls;
extern short mouse_vscrolls;

extern std::string working_directory;
extern std::string program_directory;

void io_handle();
void io_clear();
void keyboard_wait();
bool mouse_wheel_up();
bool mouse_wheel_down();
bool mouse_check_button(int button);
bool mouse_check_button_pressed(int button);
bool mouse_check_button_released(int button);
void mouse_wait();
void keyboard_clear(const int key);
bool keyboard_check(int key);
bool keyboard_check_pressed(int key);
bool keyboard_check_released(int key);
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
void display_mouse_set(int x, int y);
int display_get_x();
int display_get_y();
int display_get_width();
int display_get_height();
int display_get_colordepth();
int display_get_frequency();
//int display_get_orientation();
unsigned display_get_dpi_x();
unsigned display_get_dpi_y();
extern int display_aa;
void display_reset(int samples, bool vsync);
void display_reset();  //TODO: for some odd reason this has to be second or it will error if you call the one that has parameters
bool display_set_colordepth(int depth);
bool display_set_size(int w, int h);
bool display_set_frequency(int freq);
bool display_set_all(int w, int h, int freq, int bitdepth);
bool display_test_all(int w, int h, int freq, int bitdepth);
void set_synchronization(bool enable);

window_t window_handle();
wid_t window_identifier(); // a string containing the number corresponding to the game's main window handle (shell script)
wid_t window_get_identifier(window_t hwnd); // a string containing the number corresponding to the specified window handle
  
int window_get_x();
int window_get_y();

int window_get_width();
int window_get_height();

void window_set_caption(const std::string &caption);
template<typename T> void window_set_caption(T caption) { return window_set_caption(enigma_user::toString(caption)); }
std::string window_get_caption();
void window_set_color(int color);
int window_get_color();
void window_set_alpha(double alpha);
double window_get_alpha();
double window_get_region_scale();
void window_set_position(int x, int y);
void window_set_size(unsigned int width, unsigned int height);
void window_set_rectangle(int x, int y, int width, int height);
void window_center();
void window_default(bool center);  // default true specified in platforms mandatory
void window_set_region_size(int w, int h, bool adaptwindow);

int window_get_region_width();
int window_get_region_height();
int window_get_region_width_scaled();
int window_get_region_height_scaled();

void window_set_min_width(int width);
void window_set_min_height(int height);
void window_set_max_width(int width);
void window_set_max_height(int height);

void window_set_minimized(bool minimized);
void window_set_maximized(bool maximized);
void window_set_icon(int ind, unsigned subimg);
void window_set_visible(bool visible);
int window_get_icon_index();
unsigned window_get_icon_subimg();
int window_get_visible();
bool window_get_stayontop();
bool window_get_sizeable();
bool window_get_showborder();
bool window_get_showicons();
bool window_get_freezeonlosefocus();
bool window_get_minimized();
bool window_get_maximized();
bool window_has_focus();

int window_mouse_get_x();
int window_mouse_get_y();
int window_view_mouse_get_x(int id);
int window_view_mouse_get_y(int id);

int window_set_cursor(int c);
int window_get_cursor();

extern int cursor_sprite;
inline void action_set_cursor(int spr, bool c) {
  cursor_sprite = spr;
  window_set_cursor(c ? cr_default : cr_none);
}

std::string clipboard_get_text();
void clipboard_set_text(std::string text);
bool clipboard_has_text();

void os_lock_orientation(float x ,float y ,float w ,float h , bool lock);
int display_get_orientation();
float device_get_tilt_x();
float device_get_tilt_y();
float device_get_tilt_z();
}  // namespace enigma_user

#endif
