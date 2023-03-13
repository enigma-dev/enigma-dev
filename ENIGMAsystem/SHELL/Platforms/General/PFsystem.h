/** Copyright (C) 2014 Robert B. Colton
*** Copyright (C) 2013 forthevin
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

#ifndef ENIGMA_PLATFORM_SYSTEM_H
#define ENIGMA_PLATFORM_SYSTEM_H

#include <string>
using std::string;

namespace enigma_user {

enum {
  os_unknown = -1,
  os_windows = 0,
  os_win32 = 0,
  os_macosx = 1,
  os_psp = 2,
  os_ios = 3,
  os_android = 4,
  os_symbian = 5,
  os_linux = 6,
  os_winphone = 7,
  os_tizen = 8,
  os_win8native = 9,
  os_bsd = 10,
};

enum {
  browser_not_a_browser = -1,
  browser_unknown = 0,
  browser_ie = 1,
  browser_firefox = 2,
  browser_chrome = 3,
  browser_safari = 4,
  browser_opera = 5,
  browser_safari_mobile = 6,
  browser_windows_store = 7,
};

extern const int os_browser;

string os_get_config();
int os_get_info();
string os_get_language();
string os_get_region();
bool os_is_network_connected();
bool os_is_paused();
void os_lock_orientation(bool enable);
void os_powersave_enable(bool enable);

}

#endif //ENIGMA_PLATFORM_SYSTEM_H
