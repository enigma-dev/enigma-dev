#include "Platforms/General/PFmain.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

int main(int argc, char** argv) {
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
  SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");
  return enigma::enigma_main(argc, argv);
}
