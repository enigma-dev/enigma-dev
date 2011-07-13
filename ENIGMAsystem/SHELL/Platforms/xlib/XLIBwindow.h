/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
using std::string;

void gmw_init();

void Sleep(int ms);

/////////////
// VISIBLE //
/////////////

int window_set_visible(double visible);
int window_get_visible();

/////////////
// CAPTION //
/////////////
void window_set_caption(string caption);
char *window_get_caption();

///////////
// MOUSE //
///////////
int display_mouse_get_x();
int display_mouse_get_y();

int window_mouse_set(double x,double y);
int display_mouse_set(double x,double y);

////////////
// WINDOW //
////////////
//int getWindowDimension(int i);

//Getters
int window_get_x();
int window_get_y();
int window_get_width();
int window_get_height();

//Setters
void window_set_position(int x,int y);
void window_set_size(unsigned int w,unsigned int h);
void window_set_rectangle(int x,int y,int w,int h);

//Center
int window_center();

////////////////
// FULLSCREEN //
////////////////
void window_set_fullscreen(double full);
int window_get_fullscreen();

////////////
// CURSOR //
////////////
enum {
  cr_default = 0,
  cr_none    = -1,
  cr_arrow   = -2,
  cr_cross   = -3,
  cr_beam    = -4,
  cr_size_nesw = -6,
  cr_size_ns   = -7,
  cr_size_nwse = -8,
  cr_size_we   = -9,
  cr_uparrow   = -10,
  cr_hourglass = -11,
  cr_drag      = -12,
  cr_nodrop    = -13,
  cr_hsplit    = -14,
  cr_vsplit    = -15,
  cr_multidrag = -16,
  cr_sqlwait   = -17,
  cr_no        = -18,
  cr_appstart  = -19,
  cr_help      = -20,
  cr_handpoint = -21,
  cr_size_all  = -22
};

int window_set_cursor(double c);

void io_handle(), io_clear();
void keyboard_wait();

/*
display_get_width() // Returns the width of the display in pixels.
display_get_height() // Returns the height of the display in pixels.
display_set_size(w,h) Sets the width and height of the display in pixels. Returns whether this was
successful. (Realize that only certain combinations are allowed.)
display_get_colordepth() Returns the color depth in bits.
display_get_frequency() Returns the refresh frequency of the display.
display_set_colordepth(coldepth) Sets the color depth. In general only 16 and 32 are allowed values. Returns whether successful.
display_set_frequency(frequency) Sets the refresh frequency for the display. Only few frequencies are allowed. Typically you could set this to 60 with a same room speed to get smooth 60 frames per second motion. Returns whether successful.

display_set_all(w,h,frequency,coldepth) Sets all at once. Use -1 for values you do not want to change. Returns whether successful.
display_test_all(w,h,frequency,coldepth) Tests whether the indicated settings are allowed. It does not change the settings. Use -1 for values you do not want to change. Returns whether the settings are allowed.
display_reset() Resets the display settings to the ones when the program was started.


window_default()
window_get_cursor()
window_set_color(color)
window_get_color()
window_set_region_scale(scale,adaptwindow)
window_get_region_scale()
window_set_showborder(show)
window_get_showborder()
window_set_showicons(show)
window_get_showicons()
window_set_stayontop(stay)
window_get_stayontop()
window_set_sizeable(sizeable)
window_get_sizeable()*/


int show_message(string str);


namespace enigma {
  extern char** parameters;
  //void writename(char* x);
}

#define enigmacatchmouse() //Linux should hopefully do that automatically.
