/*********************************************************************************\
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
\*********************************************************************************/

/**Standard window functions***************************************************\

cr_default, cr_none, cr_arrow, cr_cross, cr_beam, cr_size_nesw, cr_size_ns, cr_size_nwse, 
cr_size_we, cr_uparrow, cr_hourglass, cr_drag, cr_nodrop, cr_hsplit, cr_vsplit, 
cr_multidrag, cr_sqlwait, cr_no, cr_appstart, cr_help, cr_handpoint, cr_size_all

int show_message(std::string str)
int window_get_x()
int window_get_y()
int window_get_width()
int window_get_height()
int window_set_visible(double visible)
int window_get_visible()
int window_set_stayontop(double stay)
int window_get_stayontop()
int window_set_caption(char* caption)
int window_set_caption(std::string caption)
std::string window_get_caption()
double window_set_color(double color)
double window_get_color()
int window_set_region_scale(double scale, double adaptwindow)
double window_get_region_scale()
int window_set_position(double x, double y)
int window_set_size(double width, double height)
int window_set_rectangle(double x, double y, double width, double height)
double window_center()
double window_default()
double window_mouse_get_x()
double window_mouse_get_y()
int window_mouse_set(double x,double y)
std::string get_string(std::string message, std::string def)
int window_set_fullscreen(double full)
int get_color(double defcolor)
int game_end()

\******************************************************************************/

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



namespace enigma
{
  void clampparent();
}

int show_message(std::string str);
int window_get_x();
int window_get_y();
int window_get_width();
int window_get_height();
int window_set_visible(double visible);
int window_get_visible();
int window_set_stayontop(double stay);
int window_get_stayontop();
int window_set_caption(char* caption);
int window_set_caption(std::string caption);
std::string window_get_caption();
double window_set_color(double color);
double window_get_color();
int window_set_region_scale(double scale, double adaptwindow);
double window_get_region_scale();
int window_set_position(double x, double y);
int window_set_size(double width, double height);
int window_set_rectangle(double x, double y, double width, double height);
double window_center();
double window_default();
double window_mouse_get_x();
double window_mouse_get_y();
int window_mouse_set(double x,double y);

int window_set_fullscreen(double full);

namespace getstr
{
  void init();
}

std::string get_string(std::string message, std::string def);
int game_end();
int get_color(double defcolor);
