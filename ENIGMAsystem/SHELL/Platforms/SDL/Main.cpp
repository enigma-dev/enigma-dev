#include "Platforms/General/PFmain.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...
#include <string>

namespace enigma {
    void set_working_directory() {
      enigma_user::working_directory = SDL_GetBasePath(); 
    }
}

int main(int argc, char** argv) {
  enigma::set_working_directory();
  return enigma::enigma_main(argc, argv);
}
