/** Copyright (C) 2017 Faissal I. Bensefia
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
#include "Graphics_Systems/graphics_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Graphics_Systems/General/GScolors.h"

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <Universal_System/roomsystem.h> // room_caption, update_mouse_variables

namespace enigma_user {
  int display_aa = 14;

  void set_synchronization(bool enable){}
    
  void display_reset(int samples, bool vsync){}
    
  void screen_refresh(){}
}

