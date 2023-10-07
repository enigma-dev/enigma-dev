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

#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"
#include <X11/Xlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string>

namespace enigma {

namespace x11 {

Display *disp;
Window win;

}

// called from initGameWindow()
// capture sdl window disp/hwnd
void window_init() {
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(enigma::windowHandle, &wmInfo);
  enigma::x11::disp = wmInfo.info.x11.display;
  enigma::x11::win = wmInfo.info.x11.window;
}

}

namespace enigma_user {

window_t window_handle() {
  unsigned long long window_uint64 = enigma::x11::win;
  return reinterpret_cast<window_t>(window_uint64);
}

// returns an identifier for the SDL2 window
// this string can be used in shell scripts
wid_t window_identifier() {
  return std::to_string(reinterpret_cast<unsigned long long>(window_handle()));
}

// returns an identifier for certain window
// this string can be used in shell scripts
wid_t window_get_identifier(window_t hwnd) {
  return std::to_string(reinterpret_cast<unsigned long long>(hwnd));
}

}
