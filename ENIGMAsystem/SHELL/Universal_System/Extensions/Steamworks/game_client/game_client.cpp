#include "game_client.h"

#include "overlay.h"

namespace steamworks {

// cgame_client::cgame_client(ISteamUser* steam_user, ISteamFriends* steam_friends, ISteamUtils* steam_utils)
//     : steam_user_(steam_user),
//       steam_friends_(steam_friends),
//       steam_utils_(steam_utils),
//       steam_id_local_user_(steam_user->GetSteamID()),
//       steam_app_id_(steam_utils->GetAppID()) {
//   init();
// }

cgame_client::cgame_client()
    : steam_id_local_user_(SteamUser()->GetSteamID()),
      steam_app_id_(SteamUtils()->GetAppID()),
      steam_persona_name_(SteamFriends()->GetPersonaName()),
      current_game_language_(SteamApps()->GetCurrentGameLanguage()),
      available_languages_(SteamApps()->GetAvailableGameLanguages()) {
  init();
}

cgame_client::~cgame_client() {
  if (cgame_client::overlay != nullptr) delete cgame_client::overlay;
}

void cgame_client::init() { cgame_client::overlay = new coverlay(); }

coverlay* cgame_client::get_overlay() { return cgame_client::overlay; }

bool cgame_client::is_user_logged_on() { return SteamUser()->BLoggedOn(); }

CSteamID cgame_client::get_steam_id_local_user() { return cgame_client::steam_id_local_user_; }

unsigned cgame_client::get_steam_app_id() { return cgame_client::steam_app_id_; }

std::string cgame_client::get_steam_persona_name() { return cgame_client::steam_persona_name_; }

std::string cgame_client::get_steam_user_persona_name(CSteamID user_persona_name) {
  return SteamFriends()->GetFriendPersonaName(user_persona_name);
}

std::string cgame_client::get_current_game_language() { return cgame_client::current_game_language_; }

std::string cgame_client::get_available_languages() { return cgame_client::available_languages_; }

bool cgame_client::is_subscribed() { return SteamApps()->BIsSubscribed(); }

}  // namespace steamworks
