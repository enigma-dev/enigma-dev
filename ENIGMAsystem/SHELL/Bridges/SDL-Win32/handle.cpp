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

namespace enigma_user {

#if GM_COMPATIBILITY_VERSION <= 81
unsigned long long window_handle() {
  return (unsigned long long)enigma::hWnd;
}
#else
void* window_handle() {
  return enigma::hWnd;
}
#endif

} // namespace enigma_user
