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

// TODO: this should be in namespace enigma? will check later.
// TODO: Is this the right place for this function?
extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}

namespace steamworks_gc {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

void GCMain::set_warning_message_hook() {
  if (steamworks_b::SteamBinder::ISteamUtils_SetWarningMessageHook == nullptr ||
      steamworks_b::SteamBinder::SteamUtils_vXXX == nullptr) {
    DEBUG_MESSAGE("GCMain::set_warning_message_hook() failed due to loading error.", M_ERROR);
    return;
  }

  steamworks_b::SteamBinder::ISteamUtils_SetWarningMessageHook(steamworks_b::SteamBinder::SteamUtils_vXXX(),
                                                          &SteamAPIDebugTextHook);
}

}  // namespace steamworks_gc
