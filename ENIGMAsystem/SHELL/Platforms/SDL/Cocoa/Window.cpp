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

#include "Window.h"
#include "Platforms/SDL/Window.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

extern "C" void *cocoa_window_handle();
extern "C" long cocoa_window_identifier();
extern "C" long cocoa_window_get_identifier(void *hwnd);

static inline string remove_trailing_zeros(long numb) {
  string strnumb = std::to_string(numb);

  while (!strnumb.empty() && strnumb.find('.') != string::npos && (strnumb.back() == '.' || strnumb.back() == '0'))
    strnumb.pop_back();

  return strnumb;
}

namespace enigma {

void *NSWin; // NSWindow * a.k.a CocoaAPI Window Handle
long WinNum; // Window ID a.k.a [NSWindow windowNumber]

// called from initGameWindow()
// capture sdl window win/winid
void window_id_init() {
  enigma::NSWin = cocoa_window_handle();
  enigma::WinNum = cocoa_window_identifier();
}

}

namespace enigma_user {

#if GM_COMPATIBILITY_VERSION <= 81
unsigned long long window_handle() {
  return (unsigned long long)enigma::NSWin;
}
#else
void *window_handle() {
  return (void *)enigma::NSWin;
}
#endif

// returns an identifier for the sdl window
// this string can be used in shell scripts
string window_identifier() {
  return remove_trailing_zeros(enigma::WinNum);
}

// returns an identifier for certain window
// this string can be used in shell scripts
string window_get_identifier(void *hwnd) {
  return remove_trailing_zeros(cocoa_window_get_identifier(hwnd));
}

}
