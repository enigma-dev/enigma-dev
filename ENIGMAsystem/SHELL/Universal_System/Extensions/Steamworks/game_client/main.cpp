#include "main.h"

namespace steamworks {

cgame_client* cmain::game_client_ = nullptr;

bool cmain::is_initialised_ = false;

bool cmain::init() {
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

  // cmain::game_client_ = new cgame_client(steam_user, steam_friends, steam_utils);
  cmain::game_client_ = new cgame_client();

  cmain::is_initialised_ = true;

  return true;
}

void cmain::shutdown() {
  cmain::is_initialised_ = false;

  delete cmain::game_client_;

  SteamAPI_Shutdown();
}

bool cmain::is_initialised() { return cmain::is_initialised_; }

cgame_client* cmain::get_game_client() { return cmain::game_client_; }

}  // namespace steamworks
