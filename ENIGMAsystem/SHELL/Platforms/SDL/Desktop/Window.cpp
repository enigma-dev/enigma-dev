#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"

namespace enigma_user {

void window_set_size(unsigned w, unsigned h) {
  SDL_SetWindowSize(enigma::windowHandle, w, h);
}

}
