/** Copyright (C) 2014 Robert B. Colton
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <cstdlib>
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "XLIBwindow.h"
#include "XLIBsystem.h"

namespace enigma_user {

string os_get_config() {
  return "";
}

int os_get_info() {
  return 0;
}

string os_get_language() {
  char *s = getenv("LANG");
  if (!s || !*s || !s[1]) return "";
  if (!s[2] || s[2] == '.' || s[2] == '_') {
    return string(s, 2);
  }
  return s; // It won't match people's ISO-639 checks, but it's better than "".
}

string os_get_region() {
  // Most distributions are only aware of location to the extent required to
  // give accurate time information; we can't accurately give an ISO 3166-1
  // compliant string for the device.
  return "";
}

bool os_is_network_connected() {
  return true; // Please change to false should the year drop below 2010
}

void os_lock_orientation(bool enable) {
  // Responsive reorientation is not a feature of the X window system.
}

void os_powersave_enable(bool enable) {
  // X11 does not handle powersave features, but nothing else on a GNU/Linux
  // distribution cares about your opinion, anyway.
}

}
