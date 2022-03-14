#include "Platforms/General/PFmain.h"

#include <SDL2/SDL.h> //sdl does a #define main SDL_main...

namespace enigma {
    void initialize_directory_globals() {
    #ifdef __ANDROID__ //FIXME: this is wrong path but better to set it somewhere I guess
      enigma_user::working_directory = SDL_AndroidGetInternalStoragePath();
    #else
      enigma_user::working_directory = SDL_GetBasePath();
    #endif
    }
}

int main(int argc, char** argv) {
  return enigma::enigma_main(argc, argv);
}
