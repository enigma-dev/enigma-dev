#include "Platforms/General/PFwindow.h"

namespace enigma_user {

void window_center() {
  int i = 0;
  int targetX = display_get_x() + ((display_get_width() - window_get_width()) / 2);
  int targetY = display_get_y() + ((display_get_height() - window_get_height()) / 2);
  // without this while loop, the window won't be guaranteed to center 
  // at game init, in xlib, which is weird, but this makes it necessary.
  while (enigma::windowX != targetX || enigma::windowY != targetY) {
    window_set_position(targetX, targetY);
    if (i >= 15) break; 
    i++;
  }
}

} // enigma_user
