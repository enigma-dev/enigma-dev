/** Copyright (C) 2023-2024 Saif Kandil (k0T0z)
***
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

#include "gc_main.h"

#ifndef ENIGMA_STEAMWORKS_API_MOCK
// TODO: this should be in namespace enigma? will check later.
// TODO: Is this the right place for this function?
extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}
#endif  // ENIGMA_STEAMWORKS_API_MOCK

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

void GCMain::set_warning_message_hook() {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  SteamUtils()->SetWarningMessageHook(&SteamAPIDebugTextHook);
#else
  SteamUtils()->SetWarningMessageHook();
#endif  // ENIGMA_STEAMWORKS_API_MOCK
}

}  // namespace steamworks_gc
