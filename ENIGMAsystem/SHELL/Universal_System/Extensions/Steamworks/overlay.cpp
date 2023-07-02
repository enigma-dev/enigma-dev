#include "overlay.h"

#include "game_client/main.h"
#include "game_client/overlay.h"

namespace enigma_user {

bool steam_is_overlay_enabled() { return steamworks::coverlay::is_overlay_enabled(); }

bool steam_is_overlay_activated() {
  return steamworks::cmain::get_game_client()->get_overlay()->is_overlay_activated();
}

void steam_activate_overlay(const int overlay_type) {
  switch (overlay_type) {
    case enigma_user::ov_friends:
      steamworks::coverlay::activate_overlay("friends");
      break;
    case enigma_user::ov_community:
      steamworks::coverlay::activate_overlay("community");
      break;
    case enigma_user::ov_players:
      steamworks::coverlay::activate_overlay("players");
      break;
    case enigma_user::ov_settings:
      steamworks::coverlay::activate_overlay("settings");
      break;
    case enigma_user::ov_gamegroup:
      steamworks::coverlay::activate_overlay("gamegroup");
      break;
    case enigma_user::ov_achievements:
      steamworks::coverlay::activate_overlay("achievements");
      break;
    default:
      DEBUG_MESSAGE("Invalid overlay type", M_ERROR);
      break;
  }
}

void steam_activate_overlay_browser(const std::string& url) { steamworks::coverlay::activate_overlay_browser(url); }

void steam_activate_overlay_store(const int app_id) {
  steamworks::coverlay::activate_overlay_browser("https://store.steampowered.com/app/" + std::to_string(app_id));
}

void steam_activate_overlay_user(const std::string& dialog_name, unsigned long long steamid) {
  steamworks::coverlay::activate_overlay_user(dialog_name, steamid);
}

}  // namespace enigma_user
