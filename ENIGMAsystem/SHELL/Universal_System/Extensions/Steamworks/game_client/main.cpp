#include "main.h"

namespace steamworks {

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}

c_game_client* c_main::c_game_client_ = nullptr;

bool c_main::is_initialised_ = false;

bool c_main::init() {
  if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) {  // replace k_uAppIdInvalid with your AppID
    return false;
  }

  if (!SteamAPI_Init()) {
    return false;
  }

  if (!SteamUser()->BLoggedOn()) {
    return false;
  }

  // ISteamUser* steam_user = SteamUser();
  // ISteamFriends* steam_friends = SteamFriends();
  // ISteamUtils* steam_utils = SteamUtils();

  // c_main::c_game_client_ = new c_game_client(steam_user, steam_friends, steam_utils);
  c_main::c_game_client_ = new c_game_client();

  c_main::is_initialised_ = true;

  return true;
}

void c_main::shutdown() {
  c_main::is_initialised_ = false;

  delete c_main::c_game_client_;

  SteamAPI_Shutdown();
}

bool c_main::is_initialised() { return c_main::is_initialised_; }

c_game_client* c_main::get_c_game_client() { return c_main::c_game_client_; }

void c_main::update() { SteamAPI_RunCallbacks(); }

void c_main::set_warning_message_hook() { SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook); }

}  // namespace steamworks
