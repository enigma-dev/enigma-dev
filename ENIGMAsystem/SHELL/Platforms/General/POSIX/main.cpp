#include "Platforms/General/PFmain.h"

// lmao this has to be included or else
// SDL can not coopt main() with SDL_main
// for Win32
#ifdef ENIGMA_WS_SDL
#include <SDL2/SDL.h>
#endif

int main(int argc, char** argv) {
  return enigma::main(argc, argv);
}
