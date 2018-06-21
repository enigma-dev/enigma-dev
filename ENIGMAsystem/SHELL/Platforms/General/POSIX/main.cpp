#include "Platforms/General/PFmain.h"

#ifdef ENIGMA_WS_SDL
#include <SDL2/SDL.h>
#endif

int main(int argc, char** argv) {
  return enigma::main(argc, argv);
}
