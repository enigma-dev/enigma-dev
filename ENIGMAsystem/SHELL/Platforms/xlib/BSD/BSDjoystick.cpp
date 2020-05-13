#include "Platforms/General/PFjoystick.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <string>

using std::string;

namespace enigma
{
  static bool init_jsmsg = false;
  
  void init_joysticks() {
    if (init_jsmsg) {
      DEBUG_MESSAGE("Joysticks are not yet implemented in X11 for FreeBSD!", MESSAGE_TYPE::M_WARNING);
      init_jsmsg = true;
    }
  }

  void handle_joysticks() {
    
  }
}  // namespace enigma
