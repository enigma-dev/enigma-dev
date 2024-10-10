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

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

#include <wininet.h>
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"

namespace enigma_user {

int os_get_info()         { return {}; }
string os_get_config()    { return {}; }
string os_get_language()  { return {}; }
string os_get_region()    { return {}; }
void os_lock_orientation(bool enable) {}
void os_powersave_enable(bool enable) {}

bool os_is_network_connected() {
	DWORD dwFlags = 0;
	return InternetGetConnectedState(&dwFlags, 0);
}

}  // namespace enigma_user
