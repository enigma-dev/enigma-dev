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
#include "Platforms/General/PFsystem.h"
#include "Platforms/SDL/Window.h"
#include <windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string>

namespace enigma {

HWND hWnd;
HINSTANCE hInstance;

// called from initGameWindow()
// capture sdl window hwnd/inst
void window_init() {
  SDL_SysWMinfo systemInfo;
  SDL_VERSION(&systemInfo.version);
  SDL_GetWindowWMInfo(windowHandle, &systemInfo);
  hWnd = systemInfo.info.win.window;
  hInstance = systemInfo.info.win.hinstance;
}

} // namespace enigma

namespace enigma_user {

int os_type = os_windows;

window_t window_handle() {
  return reinterpret_cast<window_t>(enigma::hWnd);
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
