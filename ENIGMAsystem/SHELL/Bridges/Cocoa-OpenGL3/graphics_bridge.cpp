/** Copyright (C) 2014 Robert B. Colton
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

#include "../General/glxew.h"
#include "Platforms/Cocoa/CocoaMain.h"
#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

#include <iostream>
#include <cstring>

// NOTE: Changes/fixes that applies to this likely also applies to the OpenGL3 version.

namespace enigma {
  GLuint msaa_fbo = 0;
  
  void WindowResized() {
    // clear the window color, viewport does not need set because backbuffer was just recreated
    enigma_user::draw_clear(enigma_user::window_get_color());
  }
}

#include "Platforms/General/PFwindow.h" // window_set_caption
#include "Universal_System/roomsystem.h" // room_caption, update_mouse_variables

namespace enigma_user {
  // Don't know where to query this on Cocoa, just defaulting it to 2,4,and 8 samples all supported, Windows puts it in EnableDrawing
  int display_aa = 14;

  void set_synchronization(bool enable) {

  }
    
  void display_reset(int samples, bool vsync) {
    set_synchronization(vsync);
    //TODO: Copy over from the Win32 bridge
  }
    
  //void screen_refresh() {

  //}

}

