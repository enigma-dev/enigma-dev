#include "general.h"

namespace enigma_user {

bool steam_initialised() { return steamworks::steam_client::is_valid(); }

bool steam_stats_ready() { return false; }

int steam_get_app_id() { return (int)steamworks::steam_utils::get_app_id(); }

int steam_get_user_account_id() {
  return -1;
  // return steamworks::steam_client::steam_id().GetAccountID();
}

int steam_get_user_steam_id() {
  // return -1;
  std::cout << "steam_get_user_steam_id" << std::endl;
  CSteamID steam_id = SteamUser()->GetSteamID();
  std::cout << "steam_id" << std::endl;
  uint64 steam_id_64 = steam_id.ConvertToUint64();
  std::cout << "steam_id_64: " << steam_id_64 << std::endl;
  int steam_id_32 = (int)steam_id_64;
  std::cout << "steam_id_32: " << steam_id_32 << std::endl;
  return steam_id_32;
}

std::string steam_get_persona_name() { return steamworks::steam_client::name(); }

std::string steam_get_user_persona_name(CSteamID user_persona_name) {
  return steamworks::steam_client::user_name(user_persona_name);
}

bool steam_is_user_logged_on() { return steamworks::steam_client::is_logged_on(); }

std::string steam_current_game_language() { return std::to_string(-1); }

std::string steam_available_languages() { return std::to_string(-1); }

bool steam_is_subscribed() { return false; }

void steam_set_warning_message_hook() { steamworks::steam_client::enable_warning_message_hook(); }

}  // namespace enigma_user
