/** Copyright (C) 2008-2017 Josh Ventura
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

#ifndef PLATFORMS_MANDATORY_H
#define PLATFORMS_MANDATORY_H

// This file contains functions that are required to be implemented by any API
// under this directory, /Platforms/. They are not required to do anything at all
// if the platform in question lacks support for them entirely.

#include "Platforms/General/PFsystem.h"

namespace enigma_user {
  extern const int os_type;
  extern unsigned long current_time; // milliseconds since the start of the game
  extern unsigned long delta_time; // microseconds since the last step event
}

namespace enigma
{
  bool initGameWindow();

  void destroyWindow();

  void initInput();
  void handleInput();

  void EnableDrawing(void* handle = nullptr);
  void DisableDrawing(void* handle = nullptr);
  void showWindow();

  // System / Window events
  int handleEvents();

  // This function is generated by compiler based on event.res
  int ENIGMA_events();

  // This method should write the name of the running module to exenamehere.
  void windowsystem_write_exename(char* exenamehere);

  // This method should take an integer framerate and perform the necessary operations to limit fps to that rate.
  void set_room_speed(int framerate);

  // This method is called at load time. It allows for initializing arrays for input.
  void input_initialize();

  // This method initializes all of the major systems generically and is universally
  // called by every platform.
  int initialize_everything();

  // Control variables.
  /// Controls whether we suspend the event loop while the game is out of focus.
  extern bool freezeOnLoseFocus;

  /**
   * Gives a cached value of the dimensions of the screen. This allows the main
   * draw loop to avoid querying the window server every frame. Performing these
   * queries is relatively expensive, and can lower ENIGMA's peak framerate to
   * around 300 (which is a metric naive users compare to determine whether
   * ENIGMA competes with their toolchain of choice).
   */
  extern int windowWidth, windowHeight;

  /// Controls whether the window will be adapted to fit its content.
  extern bool windowAdapt;
  /// Probably the dumbest variable in ENIGMA. Serves as a scaling factor
  /// between 1 and 100 (for no reason). If it's zero, rather than scaling by
  /// this factor, the drawing area should be shrunk to fit. If this value goes
  /// negative, a third behavior is used: the drawing region is shrunk to fit,
  /// but the aspect ratio is maintained.
  extern int viewScale;
  /// These are the view-calculated or user-defined dimensions of the drawing
  /// region. They are used in fitting the graphics to the window.
  extern int regionWidth, regionHeight;
  /// These are the drawing area dimensions, calculated according to the scaling
  /// settings defined above.
  extern double scaledWidth, scaledHeight;

  // Game Settings variables. TODO: Remove; pass to a window_initialize() method.
  extern bool isSizeable, showBorder, showIcons, isFullScreen;
  extern bool treatCloseAsEscape; // TODO: Not implemented outside of Windows...
  extern int windowColor;
}

namespace enigma_user
{

// These functions are standard GML that are an integral part of the system.
// Each instance must implement these, even if they are unable to do anything
// on the target platform.

// This is used with roomsystem
void window_default(bool center_size = false);

// These four are a surprisingly integral part of the system
int window_mouse_get_x();
int window_mouse_get_y();
void window_view_mouse_set(int id, int x, int y);
void window_mouse_set(int x,int y);

// These two are used by screen_redraw for view calculations and such.
int window_get_region_width_scaled();
int window_get_region_height_scaled();

// For game settings
void window_set_fullscreen(bool full);
bool window_get_fullscreen();

void window_set_region_scale(double scale, bool adaptwindow);
void window_set_stayontop(bool stay);
void window_set_sizeable(bool sizeable);
void window_set_showborder(bool show);
void window_set_showicons(bool show);
void window_set_freezeonlosefocus(bool freeze);
unsigned long get_timer(); // number of microseconds since the game started

}  // namespace enigma_user

#endif
