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

#ifndef PLATFORMS_MANDATORY__H
#define PLATFORMS_MANDATORY__H

// This file contains functions that are required to be implemented by any API
// under this directory, /Platforms/. They are not required to do anything at all
// if the platform in question lacks support for them entirely.

#include "os_types.h"

namespace enigma_user {
  extern const int os_type;
  extern unsigned long current_time; // milliseconds since the start of the game
  extern unsigned long delta_time; // microseconds since the last step event
}

namespace enigma
{
  // This method should write the name of the running module to exenamehere.
  void windowsystem_write_exename(char* exenamehere);

  // This method should take an integer framerate and perform the necessary operations to limit fps to that rate.
  void set_room_speed(int framerate);

  // This method is called at load time. It allows for initializing arrays for input.
  void input_initialize();
}

namespace enigma_user
{

// These functions are standard GML that are an integral part of the system.
// Each instance must implement these, even if they are unable to do anything on the target platform.

// This is used with roomsystem
void window_default();

// These two are a surprisingly integral part of the system
int window_mouse_get_x();
int window_mouse_get_y();
int window_views_mouse_get_x(); // mouse_x constant, with respect to all views
int window_views_mouse_get_y(); // mouse_y constant, with respect to all views

// These two are used by screen_redraw for view calculations and such.
int window_get_region_width_scaled();
int window_get_region_height_scaled();

// For game settings
void window_set_fullscreen(bool full);

int window_set_cursor(int c);
inline void action_set_cursor(int spr, bool c) {
	// TODO: if spr exists should create a sprite cursor, game maker allows
	// both the sprite cursor and system cursor to be visible at the same time
	if (c) {
		window_set_cursor(0); // no system cursor
	} else {
		window_set_cursor(-1); // default system cursor
	}
}
void window_set_region_scale(double scale, bool adaptwindow);
void window_set_stayontop(bool stay);
void window_set_sizeable(bool sizeable);
void window_set_showborder(bool show);
void window_set_showicons(bool show);
void window_set_freezeonlosefocus(bool freeze);
bool set_working_directory(std::string dir = "");
unsigned long get_timer(); // number of microseconds since the game started
}

#endif
