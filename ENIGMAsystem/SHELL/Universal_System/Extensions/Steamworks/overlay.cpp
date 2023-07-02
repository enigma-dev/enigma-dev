#include "overlay.h"

#include "game_client/main.h"
#include "game_client/overlay.h"

namespace enigma_user {

bool steam_is_overlay_enabled() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_overlay_enabled but not initialized, consider calling steam_init first.", M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_is_overlay_enabled but not logged in, please log into Steam first.", M_ERROR);
    return false;
  }

  return steamworks::c_overlay::is_overlay_enabled();
}

bool steam_is_overlay_activated() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_overlay_activated but not initialized, consider calling steam_init first.",
                  M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_is_overlay_activated but not logged in, please log into Steam first.", M_ERROR);
    return false;
  }

  return steamworks::c_main::get_c_game_client()->get_overlay()->is_overlay_activated();
}

void steam_activate_overlay(const int overlay_type) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay but not initialized, consider calling steam_init first.", M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay but not logged in, please log into Steam first.", M_ERROR);
    return;
  }

  switch (overlay_type) {
    case enigma_user::ov_friends:
      steamworks::c_overlay::activate_overlay("friends");
      break;
    case enigma_user::ov_community:
      steamworks::c_overlay::activate_overlay("community");
      break;
    case enigma_user::ov_players:
      steamworks::c_overlay::activate_overlay("players");
      break;
    case enigma_user::ov_settings:
      steamworks::c_overlay::activate_overlay("settings");
      break;
    case enigma_user::ov_gamegroup:
      steamworks::c_overlay::activate_overlay("gamegroup");
      break;
    case enigma_user::ov_achievements:
      steamworks::c_overlay::activate_overlay("achievements");
      break;
    default:
      DEBUG_MESSAGE(
          "Invalid overlay type. The allowed options are: ov_friends, ov_community, ov_players, ov_settings, "
          "ov_gamegroup, and ov_achievements.",
          M_ERROR);
      break;
  }
}

void steam_activate_overlay_browser(const std::string& url) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_browser but not initialized, consider calling steam_init first.",
                  M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_browser but not logged in, please log into Steam first.", M_ERROR);
    return;
  }

  steamworks::c_overlay::activate_overlay_browser(url);
}

void steam_activate_overlay_store(const int app_id) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_store but not initialized, consider calling steam_init first.",
                  M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_store but not logged in, please log into Steam first.", M_ERROR);
    return;
  }

  steamworks::c_overlay::activate_overlay_browser("https://store.steampowered.com/app/" + std::to_string(app_id));
}

void steam_activate_overlay_user(const std::string& dialog_name, unsigned long long steamid) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_user but not initialized, consider calling steam_init first.",
                  M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_activate_overlay_user but not logged in, please log into Steam first.", M_ERROR);
    return;
  }

  steamworks::c_overlay::activate_overlay_user(dialog_name, steamid);
}

}  // namespace enigma_user
