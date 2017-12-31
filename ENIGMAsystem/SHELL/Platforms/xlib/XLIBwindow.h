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

#ifndef _XLIB_WINDOW__H
#define _XLIB_WINDOW__H

#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"

#include <string>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace enigma {
  namespace x11
  {
    extern Display *disp;
    extern Screen *screen;
  }
  
  void gmw_init();
  void initkeymap();
}

#endif
