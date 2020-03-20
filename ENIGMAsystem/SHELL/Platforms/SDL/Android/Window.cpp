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
// #include <?????.h> FIXME: idk what android-specific header(s) define(s) ANativeWindow * and EGLSurface...
#include <SDL2/SDL.h>
// #include <SDL2/SDL_syswm.h> // FIXME: pussy not found !

namespace enigma {

// ANativeWindow *ANWin;
// EGLSurface EGLSurf;

// called from initGameWindow()
// capture sdl window hwnd/surf
void window_id_init() {
  /*
  SDL_SysWMinfo systemInfo;
  SDL_VERSION(&systemInfo.version);
  SDL_GetWindowWMInfo(windowHandle, &systemInfo);
  ANWin = systemInfo.info.android.window;
  EGLSurf = systemInfo.info.android.surface;
  */
}

}

namespace enigma_user {

/*
window_t window_handle() {
  unsigned long long window_uint64 = static_cast<unsigned long long>(enigma::ANWin);
  return *(static_cast<window_t *>(window_uint64));
}
*/
  
}
