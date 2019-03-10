#include "Platforms/SDL/Window.h"
#include "Bridges/Win32/WINDOWShandle.h"

#include <SDL2/SDL.h>

namespace enigma {
bool initGameWindow() {
  SDL_Init(SDL_INIT_VIDEO);
  windowHandle = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
  get_window_handle();
  return (windowHandle != nullptr);
}
}
