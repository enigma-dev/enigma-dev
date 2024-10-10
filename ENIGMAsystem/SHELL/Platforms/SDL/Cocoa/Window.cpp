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

#include "CocoaWindow.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string>

extern "C" long cocoa_window_identifier();
extern "C" long cocoa_window_get_identifier(void *hwnd);

void *cocoa_window_handle() {
  SDL_SysWMinfo wmInfo;
  SDL_VERSION(&wmInfo.version);
  SDL_GetWindowWMInfo(enigma::windowHandle, &wmInfo);
  return (void *)wmInfo.info.cocoa.window;
}

namespace enigma {

void *NSWin; // NSWindow * a.k.a CocoaAPI Window Handle
unsigned long WinNum; // CGWindowID a.k.a [NSWindow windowNumber]

// called from initGameWindow()
// capture sdl window win/winid
void window_init() {
  enigma::NSWin = cocoa_window_handle();
  enigma::WinNum = cocoa_window_identifier();
}

}

namespace enigma_user {

window_t window_handle() {
  return reinterpret_cast<window_t>(enigma::NSWin);
}

// returns an identifier for the SDL2 window
// this string can be used in shell scripts
wid_t window_identifier() {
  return std::to_string(enigma::WinNum);
}

// returns an identifier for certain window
// this string can be used in shell scripts
wid_t window_get_identifier(window_t hwnd) {
  return std::to_string(cocoa_window_get_identifier(reinterpret_cast<void *>(hwnd)));
}

}
