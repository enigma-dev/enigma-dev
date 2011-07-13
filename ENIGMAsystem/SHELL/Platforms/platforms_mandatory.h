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

// This file contains functions that are required to be implemented by any API
// under this directory, /Platforms/. They are not required to do anything at all
// if the platform in question lacks support for them entirely.

namespace enigma
{
  // This method should write the name of the running module to exenamehere.
  void windowsystem_write_exename(char* exenamehere);

  // This method should take an integer framerate and perform the necessary operations to limit fps to that rate.
  void sleep_for_framerate(int framerate);
  
  // This method is called at load time. It allows for initializing arrays for input.
  void input_initialize();
}

// These functions are standard GML that are an integral part of the system.
// Each instance must implement these, even if they are unable to do anything on the target platform.

// This obviously displays an error message.
void show_error(string, const bool); // It should offer a button to end the game.

// This function sets the game window caption, if there is a place reserved for such.
void window_set_caption(string caption);

// This function should resize the window, meaning a lowered resolution if the device displays all windows full screen.
void window_set_size(unsigned int width, unsigned int height);

// These two are a surprisingly integral part of the system
int window_mouse_get_x();
int window_mouse_get_y();

// These two are used by screen_redraw for view calculations and such.
int window_get_width();
int window_get_height();
