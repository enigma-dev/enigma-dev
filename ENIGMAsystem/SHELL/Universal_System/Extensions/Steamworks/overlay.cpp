#include "overlay.h"

#include "game_client/c_overlay.h"
#include "game_client/main.h"

bool overlay_pre_checks(const std::string& script_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

bool steam_is_overlay_enabled() {
  if (!overlay_pre_checks("steam_is_overlay_enabled")) return false;

  return steamworks::c_overlay::overlay_enabled();
}

bool steam_is_overlay_activated() {
  if (!overlay_pre_checks("steam_is_overlay_activated")) return false;

  return steamworks::c_main::get_c_game_client()->get_c_overlay()->overlay_activated();
}

void steam_activate_overlay(const int overlay_type) {
  if (!overlay_pre_checks("steam_activate_overlay")) return;

  switch (overlay_type) {
    case enigma_user::ov_friends:
      steamworks::c_overlay::activate_overlay("Friends");
      break;
    case enigma_user::ov_community:
      steamworks::c_overlay::activate_overlay("Community");
      break;
    case enigma_user::ov_players:
      steamworks::c_overlay::activate_overlay("Players");
      break;
    case enigma_user::ov_settings:
      steamworks::c_overlay::activate_overlay("Settings");
      break;
    case enigma_user::ov_gamegroup:
      steamworks::c_overlay::activate_overlay("OfficialGameGroup");
      break;
    case enigma_user::ov_stats:
      steamworks::c_overlay::activate_overlay("Stats");
      break;
    case enigma_user::ov_achievements:
      steamworks::c_overlay::activate_overlay("Achievements");
      break;
    case enigma_user::ov_other:
      steamworks::c_overlay::activate_overlay("chatroomgroup/nnnn");
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_activate_overlay failed. Invalid overlay type. The allowed options are: ov_friends, "
          "ov_community, ov_players, ov_settings, ov_gamegroup, ov_stats, ov_achievements, and ov_other.",
          M_ERROR);
      break;
  }
}

void steam_activate_overlay_browser(const std::string& url) {
  if (!overlay_pre_checks("steam_activate_overlay_browser")) return;

  steamworks::c_overlay::activate_overlay_browser(url);
}

void steam_activate_overlay_store(const int app_id) {
  if (!overlay_pre_checks("steam_activate_overlay_store")) return;

  steamworks::c_overlay::activate_overlay_browser("https://store.steampowered.com/app/" + std::to_string(app_id));
}

void steam_activate_overlay_user(const unsigned dialog, const unsigned long long steamid) {
  if (!overlay_pre_checks("steam_activate_overlay_user")) return;

  switch (dialog) {
    case enigma_user::usr_ov_steamid:
      steamworks::c_overlay::activate_overlay_user("steamid", steamid);
      break;
    case enigma_user::usr_ov_chat:
      steamworks::c_overlay::activate_overlay_user("chat", steamid);
      break;
    case enigma_user::usr_ov_jointrade:
      steamworks::c_overlay::activate_overlay_user("jointrade", steamid);
      break;
    case enigma_user::usr_ov_stats:
      steamworks::c_overlay::activate_overlay_user("stats", steamid);
      break;
    case enigma_user::usr_ov_achievements:
      steamworks::c_overlay::activate_overlay_user("achievements", steamid);
      break;
    case enigma_user::usr_ov_friendadd:
      steamworks::c_overlay::activate_overlay_user("friendadd", steamid);
      break;
    case enigma_user::usr_ov_friendremove:
      steamworks::c_overlay::activate_overlay_user("friendremove", steamid);
      break;
    case enigma_user::usr_ov_friendrequestaccept:
      steamworks::c_overlay::activate_overlay_user("friendrequestaccept", steamid);
      break;
    case enigma_user::usr_ov_friendrequestignore:
      steamworks::c_overlay::activate_overlay_user("friendrequestignore", steamid);
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_activate_overlay_user failed. Invalid overlay user dialog. The allowed options are: "
          "user_ov_steamid, user_ov_chat, user_ov_jointrade, user_ov_stats, user_ov_achievements, user_ov_friendadd, "
          "user_ov_friendremove, user_ov_friendrequestaccept, and user_ov_friendrequestignore.",
          M_ERROR);
      break;
  }
}

void steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset) {
  if (!overlay_pre_checks("steam_set_overlay_notification_inset")) return;

  steamworks::c_overlay::set_overlay_notification_inset(hor_inset, vert_inset);
}

void steam_set_overlay_notification_position(const int position) {
  if (!overlay_pre_checks("steam_set_overlay_notification_position")) return;

  switch (position) {
    case enigma_user::steam_overlay_notification_position_top_left:
      steamworks::c_overlay::set_overlay_notification_position(k_EPositionTopLeft);
      break;
    case enigma_user::steam_overlay_notification_position_top_right:
      steamworks::c_overlay::set_overlay_notification_position(k_EPositionTopRight);
      break;
    case enigma_user::steam_overlay_notification_position_bottom_left:
      steamworks::c_overlay::set_overlay_notification_position(k_EPositionBottomLeft);
      break;
    case enigma_user::steam_overlay_notification_position_bottom_right:
      steamworks::c_overlay::set_overlay_notification_position(k_EPositionBottomRight);
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_set_overlay_notification_position failed. Invalid position. The allowed options are: "
          "steam_overlay_notification_position_top_left, steam_overlay_notification_position_top_right, "
          "steam_overlay_notification_position_bottom_left, and steam_overlay_notification_position_bottom_right.",
          M_ERROR);
      break;
  }
}

}  // namespace enigma_user
