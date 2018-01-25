/** Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
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

#ifndef ENIGMA_XLIB_WINDOW_H
#define ENIGMA_XLIB_WINDOW_H

#include "../General/PFwindow.h"
#include "../General/PFmain.h"

#include <string>
using std::string;

void gmw_init();

void Sleep(int ms);

namespace enigma_user {
  static inline void sleep(int ms) { Sleep(ms); }
}

namespace enigma {
  extern string*  parameters;
  extern int parameterc;
  extern int current_room_speed;
  //void writename(char* x);
  void initkeymap();
}

#endif
