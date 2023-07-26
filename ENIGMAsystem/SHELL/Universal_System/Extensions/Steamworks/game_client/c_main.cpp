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

bool c_main::init() {
  if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) {  // replace k_uAppIdInvalid with your AppID
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

  if (NULL != c_main::c_game_client_) delete c_main::c_game_client_;

  SteamAPI_Shutdown();
}

bool c_main::is_initialised() { return c_main::is_initialised_; }

c_game_client* c_main::get_c_game_client() { return c_main::c_game_client_; }

void c_main::run_callbacks() { SteamAPI_RunCallbacks(); }

void c_main::set_warning_message_hook() { SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook); }

////////////////////////////////////////////////////////
// Private fields & functions
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// Static fields & functions
////////////////////////////////////////////////////////

c_game_client* c_main::c_game_client_{NULL};

bool c_main::is_initialised_{false};

}  // namespace steamworks
