#include "Platforms/General/PFmain.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
