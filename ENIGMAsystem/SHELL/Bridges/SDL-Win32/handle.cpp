#include "Bridges/Win32/WINDOWShandle.h"

#include "Platforms/SDL/Window.h"

#include <SDL2/SDL_syswm.h>

namespace enigma {

HWND hWnd;
HINSTANCE hInstance;

HWND get_window_handle() {
  if (hWnd == NULL) { // dsound or d3d could init this first depending on systems used
    SDL_SysWMinfo systemInfo;
    SDL_VERSION(&systemInfo.version);
    SDL_GetWindowWMInfo((SDL_Window*)windowHandle, &systemInfo);
    hWnd = systemInfo.info.win.window;
    hInstance = systemInfo.info.win.hinstance;
  }

  return hWnd;
}

} //namespace enigma
