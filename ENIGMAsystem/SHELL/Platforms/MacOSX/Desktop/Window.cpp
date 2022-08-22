#include "Platforms/General/PFwindow.h"
#include "Platforms/MacOSX/Window.h"

namespace enigma_user {

void window_set_size(unsigned w, unsigned h) {
  SDL_SetWindowSize(enigma::windowHandle, w, h);
}

}
