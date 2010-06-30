/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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


namespace enigma {
  void clampparent();
}

int  show_message(string str);
int  window_get_x();
int  window_get_y();
int  window_get_width();
int  window_get_height();
void window_set_visible(int visible);
int  window_get_visible();
void window_set_stayontop(int stay);
int  window_get_stayontop();
void window_set_caption(char* caption);
void window_set_caption(string caption);
string window_get_caption();
double window_set_color(int color);
double window_get_color();
void window_set_region_scale(int scale, int adaptwindow);
double window_get_region_scale();
void window_set_position(int x, int y);
void window_set_size(unsigned int width, unsigned int height);
void window_set_rectangle(int x, int y, int width, int height);
void window_center();
int  window_default();
int  window_mouse_get_x();
int  window_mouse_get_y();
void window_mouse_set(int x,int y);
void window_set_fullscreen(bool full);
int  window_get_fullscreen();

namespace getstr {
  void init();
}

string get_string(string message, string def);
int game_end();
int get_color(double defcolor);
