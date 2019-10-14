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
// #include <SDL2/SDL_syswm.h> // pussy not found !

namespace enigma {

// called from initGameWindow()
// capture sdl window disp/hwnd
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
// returns sdl window pointer for extensions
// we cast to/from a void * for generic-ness
void *window_handle() {
  return (void *)enigma::ANWin;
}
*/
  
}
