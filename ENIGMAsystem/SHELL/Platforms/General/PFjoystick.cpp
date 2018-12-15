#include "PFjoystick.h"

namespace enigma {

  int decrease_id(int id) {
    id -= 1;
    if (id < 0)
      id = 0;
    return id;
  }

}
