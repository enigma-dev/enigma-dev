#include "overlay.h"

#include "game_client/main.h"
#include "game_client/overlay.h"

namespace enigma_user {

const int ov_friends = 0;
const int ov_community = 1;
const int ov_players = 2;
const int ov_settings = 3;
const int ov_gamegroup = 4;
const int ov_achievements = 5;

const int steam_overlay_notification_position_top_left = 0;
const int steam_overlay_notification_position_top_right = 1;
const int steam_overlay_notification_position_bottom_left = 2;
const int steam_overlay_notification_position_bottom_right = 3;

bool steam_is_overlay_enabled() { return steamworks::cmain::get_game_client()->get_overlay()->is_overlay_enabled(); }

bool steam_is_overlay_activated() {
  return steamworks::cmain::get_game_client()->get_overlay()->is_overlay_activated();
}

void steam_activate_overlay(int overlay_type) {
  switch (overlay_type) {
    case ov_friends:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("friends");
      break;
    case ov_community:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("community");
      break;
    case ov_players:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("players");
      break;
    case ov_settings:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("settings");
      break;
    case ov_gamegroup:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("gamegroup");
      break;
    case ov_achievements:
      steamworks::cmain::get_game_client()->get_overlay()->activate_overlay("achievements");
      break;
    default:
      DEBUG_MESSAGE("Invalid overlay type", M_ERROR);
      break;
  }
}

}  // namespace enigma_user
