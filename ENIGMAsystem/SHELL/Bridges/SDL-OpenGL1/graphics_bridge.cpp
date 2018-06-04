/** Copyright (C) 2013 forthevin
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

#include <GL/glxew.h>
#include "Platforms/xlib/XLIBmain.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"
#include "Platforms/SDL/Window.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <cstring>
#include <stdio.h>

#include "Platforms/xlib/XLIBwindow.h" // window_set_caption
#include "Universal_System/roomsystem.h" // room_caption, update_mouse_variables

namespace enigma_user {
  // Don't know where to query this on XLIB, just defaulting it to 2,4,and 8 samples all supported, Windows puts it in EnableDrawing
  int display_aa = 14;

  void set_synchronization(bool enable) {
    SDL_GL_SetSwapInterval(enable);
  }
    
  void display_reset(int samples, bool vsync) {
    set_synchronization(vsync);
  }
    
  void screen_refresh() {
    SDL_GL_SwapWindow(enigma::windowHandle);
    window_set_caption(room_caption);
  }

}

