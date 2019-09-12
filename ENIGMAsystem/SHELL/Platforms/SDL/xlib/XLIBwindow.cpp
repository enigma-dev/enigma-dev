/** Copyright (C) 2019 Samuel Venable
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

#include "../Window.h"
#include "Platforms/xlib/XLIBicon.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
// #include "Universal_System/Resources/sprites.h"
// #include "Universal_System/Resources/background.h"
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

namespace enigma_user {

static string currentIcon;

string window_get_icon() {
  return file_exists(currentIcon) ? currentIcon : "";
}

void window_set_icon(const string &fname) {
  // the line below prevents glitchy minimizing when 
  // icons are changed rapidly (i.e. for animation).
  if (window_get_minimized()) return;
  currentIcon = filename_absolute(fname);

  // needs to be visible first to prevent segfault
  if (!window_get_visible()) window_set_visible(true);

  if (file_exists(fname)) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(enigma::windowHandle, &wmInfo);
    enigma::XSetIcon(wmInfo.info.x11.display, wmInfo.info.x11.window, fname.c_str());
  }
}

/* 

// sprite_save() and background_save() crash SDL. don't know why, but needs to be fixed for this.

void window_set_icon_from_unpacked_sprite(int ind, unsigned subimg, const std::string &fname) {
  sprite_save(ind, subimg, fname);
  window_set_icon(fname);
}

void window_set_icon_from_unpacked_background(int back, const std::string &fname) {
  background_save(back, fname);
  window_set_icon(fname);
}

*/

}


