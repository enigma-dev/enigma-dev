#include "Platforms/General/PFwindow.h"
#include "Platforms/SDL/Window.h"


namespace enigma_user {

void window_set_size(unsigned w, unsigned h) {
  DEBUG_MESSAGE("resizing window is not possible on android", MESSAGE_TYPE::M_WARNING);
}

}
