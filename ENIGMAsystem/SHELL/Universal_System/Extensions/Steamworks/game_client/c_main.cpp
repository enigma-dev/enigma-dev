/** Copyright (C) 2023-2024 Saif Kandil
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

#include "c_main.h"

// TODO: this should be in namespace enigma? will check later.
// TODO: Is this the right place for this function?
extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}

namespace steamworks {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Static fields & functions
////////////////////////////////////////////////////////

bool c_main::restart_app_if_necessary() {
  return SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid);  // replace k_uAppIdInvalid with your AppID
}

bool c_main::init() {
  if (c_main::restart_app_if_necessary()) {
    return false;
  }

  if (!SteamAPI_Init()) {
    return false;
  }

  if (!c_game_client::is_user_logged_on()) {
    return false;
  }

  c_main::c_game_client_ = new c_game_client();

  c_main::is_initialised_ = true;

  return true;
}

void c_main::shutdown() {
  c_main::is_initialised_ = false;

  SteamAPI_Shutdown();

  if (NULL != c_main::c_game_client_) delete c_main::c_game_client_;
}

bool c_main::is_initialised() { return c_main::is_initialised_; }

void c_main::run_callbacks() { SteamAPI_RunCallbacks(); }

void c_main::set_warning_message_hook() { SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook); }

c_game_client* c_main::get_c_game_client() { return c_main::c_game_client_; }

////////////////////////////////////////////////////////
// Private fields & functions
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Static fields & functions
////////////////////////////////////////////////////////

c_game_client* c_main::c_game_client_{NULL};

bool c_main::is_initialised_{false};

}  // namespace steamworks
