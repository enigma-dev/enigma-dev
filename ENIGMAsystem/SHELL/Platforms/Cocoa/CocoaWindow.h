/********************************************************************************\
 **                                                                              **
 **  Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>                      **
 **                                                                              **
 **  This file is a part of the ENIGMA Development Environment.                  **
 **                                                                              **
 **                                                                              **
 **  ENIGMA is free software: you can redistribute it and/or modify it under the **
 **  terms of the GNU General Public License as published by the Free Software   **
 **  Foundation, version 3 of the license or any later version.                  **
 **                                                                              **
 **  This application and its source code is distributed AS-IS, WITHOUT ANY      **
 **  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
 **  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
 **  details.                                                                    **
 **                                                                              **
 **  You should have recieved a copy of the GNU General Public License along     **
 **  with this code. If not, see <http://www.gnu.org/licenses/>                  **
 **                                                                              **
 **  ENIGMA is an environment designed to create games and other programs with a **
 **  high-level, fully compilable language. Developers of ENIGMA or anything     **
 **  associated with ENIGMA are in no way responsible for its users or           **
 **  applications created by its users, or damages caused by the environment     **
 **  or programs made in the environment.                                        **
 **                                                                              **
 \********************************************************************************/

#include <string>
#include "file_manip.h"
using std::string;

//TODO: Remove all this shit in this header that is in Platforms/General/PFwindow.h since these
//functions should be the same for all platforms, and just include the general header.
//Leave what is not defined in general headers, possibly the cursor constants.

void gmw_init();

namespace enigma_user {
    void sleep(int ms);

void window_set_position(int x,int y);

/////////////
// VISIBLE //
/////////////

int window_set_visible(int visible);
int window_get_visible();

/////////////
// CAPTION //
/////////////
void window_set_caption(std::string caption);
char *window_get_caption();

///////////
// MOUSE //
///////////
int display_mouse_get_x();
int display_mouse_get_y();
int window_mouse_get_x();
int window_mouse_get_y();

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
void window_set_size(unsigned int w,unsigned int h);
int window_set_rectangle(double x,double y,double w,double h);

//Center
int window_center();

////////////////
// FULLSCREEN //
////////////////
bool window_get_fullscreen();

////////////
// CURSOR //
////////////
#define cr_default 0
#define cr_none -1
#define cr_arrow -2
#define cr_cross -3
#define cr_beam -4
#define cr_size_nesw -6
#define cr_size_ns -7
#define cr_size_nwse -8
#define cr_size_we -9
#define cr_uparrow -10
#define cr_hourglass -11
#define cr_drag -12
#define cr_nodrop -13
#define cr_hsplit -14
#define cr_vsplit -15
#define cr_multidrag -16
#define cr_sqlwait -17
#define cr_no -18
#define cr_appstart -19
#define cr_help -20
#define cr_handpoint -21
#define cr_size_all -22

int window_set_cursor(int c);

void window_set_color(int color);

int window_view_mouse_get_x(int wid);
int window_view_mouse_get_y(int wid);
void window_view_mouse_set(int wid, int x, int y);

int window_get_region_width();
int window_get_region_height();

bool window_get_stayontop();
bool window_get_sizeable();
bool window_get_showborder();
bool window_get_showicons();

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

void io_handle();
void io_clear();
void keyboard_wait();

bool window_get_region_scale();
void window_set_region_size(int w, int h, bool adaptwindow);
void window_default();
int window_get_region_width_scaled();
int window_get_region_height_scaled();

#define enigmacatchmouse() //Linux should hopefully do that automatically.

void game_end();
void action_end_game();

}

namespace enigma {
	extern char** parameters;
	//void writename(char* x);
    long int current_time();
}
