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

#include "overlay.h"

#include "gameclient/gc_overlay.h"

bool overlay_pre_checks(const std::string& script_name) {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks_gc::GameClient::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

const unsigned ov_friends{0};
const unsigned ov_community{1};
const unsigned ov_players{2};
const unsigned ov_settings{3};
const unsigned ov_gamegroup{4};
const unsigned ov_stats{5};
const unsigned ov_achievements{6};
const unsigned ov_other{7};

const unsigned user_ov_steamid{8};
const unsigned user_ov_chat{9};
const unsigned user_ov_jointrade{10};
const unsigned user_ov_stats{11};
const unsigned user_ov_achievements{12};
const unsigned user_ov_friendadd{13};
const unsigned user_ov_friendremove{14};
const unsigned user_ov_friendrequestaccept{15};
const unsigned user_ov_friendrequestignore{16};

const unsigned steam_overlay_notification_position_invalid{17};
const unsigned steam_overlay_notification_position_top_left{18};
const unsigned steam_overlay_notification_position_top_right{19};
const unsigned steam_overlay_notification_position_bottom_left{20};
const unsigned steam_overlay_notification_position_bottom_right{21};

bool steam_is_overlay_enabled() {
  if (!overlay_pre_checks("steam_is_overlay_enabled")) return false;

  return steamworks_gc::GCOverlay::overlay_enabled();
}

bool steam_is_overlay_activated() {
  if (!overlay_pre_checks("steam_is_overlay_activated")) return false;

  return steamworks_gc::GCMain::get_gameclient()->get_gc_overlay()->overlay_activated();
}

void steam_activate_overlay(const int overlay_type) {
  if (!overlay_pre_checks("steam_activate_overlay")) return;

  switch (overlay_type) {
    case enigma_user::ov_friends:
      steamworks_gc::GCOverlay::activate_overlay("Friends");
      break;
    case enigma_user::ov_community:
      steamworks_gc::GCOverlay::activate_overlay("Community");
      break;
    case enigma_user::ov_players:
      steamworks_gc::GCOverlay::activate_overlay("Players");
      break;
    case enigma_user::ov_settings:
      steamworks_gc::GCOverlay::activate_overlay("Settings");
      break;
    case enigma_user::ov_gamegroup:
      steamworks_gc::GCOverlay::activate_overlay("OfficialGameGroup");
      break;
    case enigma_user::ov_stats:
      steamworks_gc::GCOverlay::activate_overlay("Stats");
      break;
    case enigma_user::ov_achievements:
      steamworks_gc::GCOverlay::activate_overlay("Achievements");
      break;
    case enigma_user::ov_other:
      steamworks_gc::GCOverlay::activate_overlay("chatroomgroup/nnnn");
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_activate_overlay failed. Invalid overlay type. The allowed options are: ov_friends, "
          "ov_community, ov_players, ov_settings, ov_gamegroup, ov_stats, ov_achievements, and ov_other.",
          M_ERROR);
      break;
  }
}

// TODO: There is a problem with this macro here.
void steam_deactivate_overlay() {
#ifdef ENIGMA_FAKE_STEAMWORKS_API
  if (!overlay_pre_checks("steam_deactivate_overlay")) return;

  steamworks_gc::GCOverlay::deactivate_overlay();
#endif  // ENIGMA_FAKE_STEAMWORKS_API
}

void steam_activate_overlay_browser(const std::string& url) {
  if (!overlay_pre_checks("steam_activate_overlay_browser")) return;

  steamworks_gc::GCOverlay::activate_overlay_browser(url);
}

void steam_activate_overlay_store(const int app_id) {
  if (!overlay_pre_checks("steam_activate_overlay_store")) return;

  steamworks_gc::GCOverlay::activate_overlay_browser("https://store.steampowered.com/app/" + std::to_string(app_id));
}

void steam_activate_overlay_user(const unsigned dialog, const unsigned long long steamid) {
  if (!overlay_pre_checks("steam_activate_overlay_user")) return;

  switch (dialog) {
    case enigma_user::user_ov_steamid:
      steamworks_gc::GCOverlay::activate_overlay_user("steamid", steamid);
      break;
    case enigma_user::user_ov_chat:
      steamworks_gc::GCOverlay::activate_overlay_user("chat", steamid);
      break;
    case enigma_user::user_ov_jointrade:
      steamworks_gc::GCOverlay::activate_overlay_user("jointrade", steamid);
      break;
    case enigma_user::user_ov_stats:
      steamworks_gc::GCOverlay::activate_overlay_user("stats", steamid);
      break;
    case enigma_user::user_ov_achievements:
      steamworks_gc::GCOverlay::activate_overlay_user("achievements", steamid);
      break;
    case enigma_user::user_ov_friendadd:
      steamworks_gc::GCOverlay::activate_overlay_user("friendadd", steamid);
      break;
    case enigma_user::user_ov_friendremove:
      steamworks_gc::GCOverlay::activate_overlay_user("friendremove", steamid);
      break;
    case enigma_user::user_ov_friendrequestaccept:
      steamworks_gc::GCOverlay::activate_overlay_user("friendrequestaccept", steamid);
      break;
    case enigma_user::user_ov_friendrequestignore:
      steamworks_gc::GCOverlay::activate_overlay_user("friendrequestignore", steamid);
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

bool steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset) {
  if (!overlay_pre_checks("steam_set_overlay_notification_inset")) return false;

  steamworks_gc::GCOverlay::set_overlay_notification_inset(hor_inset, vert_inset);

  return true;
}

void steam_set_overlay_notification_position(const unsigned position) {
  if (!overlay_pre_checks("steam_set_overlay_notification_position")) return;

  switch (position) {
    case enigma_user::steam_overlay_notification_position_top_left:
      steamworks_gc::GCOverlay::set_overlay_notification_position(ENotificationPosition::k_EPositionTopLeft);
      break;
    case enigma_user::steam_overlay_notification_position_top_right:
      steamworks_gc::GCOverlay::set_overlay_notification_position(ENotificationPosition::k_EPositionTopRight);
      break;
    case enigma_user::steam_overlay_notification_position_bottom_left:
      steamworks_gc::GCOverlay::set_overlay_notification_position(ENotificationPosition::k_EPositionBottomLeft);
      break;
    case enigma_user::steam_overlay_notification_position_bottom_right:
      steamworks_gc::GCOverlay::set_overlay_notification_position(ENotificationPosition::k_EPositionBottomRight);
      break;
    case enigma_user::steam_overlay_notification_position_invalid:
      steamworks_gc::GCOverlay::set_overlay_notification_position(ENotificationPosition::k_EPositionInvalid);
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_set_overlay_notification_position failed. Invalid position. The allowed options are: "
          "steam_overlay_notification_position_top_left, steam_overlay_notification_position_top_right, "
          "steam_overlay_notification_position_bottom_left, steam_overlay_notification_position_bottom_right, and "
          "steam_overlay_notification_position_invalid.",
          M_ERROR);
      break;
  }
}

}  // namespace enigma_user
