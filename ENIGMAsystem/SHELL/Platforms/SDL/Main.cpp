#include "Platforms/General/PFmain.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

namespace enigma {
    void initialize_directory_globals() {
      enigma_user::working_directory = SDL_GetBasePath(); 
    }
}

int main(int argc, char** argv) {
  joystick_init();
  return enigma::enigma_main(argc, argv);
}
