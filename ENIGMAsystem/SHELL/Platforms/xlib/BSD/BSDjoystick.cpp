#include "Platforms/General/PFjoystick.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <string>

using std::string;

namespace enigma
{
  void init_joysticks() {
    DEBUG_MESSAGE("Joysticks are not yet implemented in X11 for FreeBSD!", MESSAGE_TYPE::M_WARNING);
  }

  void handle_joysticks() {
    
  }
}  // namespace enigma
