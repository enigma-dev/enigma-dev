/** Copyright (C) 2023-2024 Saif Kandil
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

#include "social.h"

bool social_pre_checks(const std::string& script_name) {
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

// Continue after leaderboards constants to prevent overlapping
const unsigned steam_user_avatar_size_small{28};
const unsigned steam_user_avatar_size_medium{29};
const unsigned steam_user_avatar_size_large{30};

bool steam_set_rich_presence(const std::string& key, const std::string& value) {
  if (!social_pre_checks("steam_set_rich_presence")) return false;

  if (!steamworks::c_game_client::set_rich_presence(key, value)) {
    DEBUG_MESSAGE(
        "Calling steam_set_rich_presence failed. This happens if key was longer than 256 or had a length of 0, "
        "value was longer than 256, and The user has reached the maximum amount of rich presence keys which is 20.",
        M_ERROR);
    return false;
  }

  return true;
}

void steam_clear_rich_presence() {
  if (!social_pre_checks("steam_clear_rich_presence")) return;

  steamworks::c_game_client::clear_rich_presence();
}

bool steam_user_set_played_with(const unsigned long long steam_id) {
  if (!social_pre_checks("steam_user_set_played_with")) return false;

  steamworks::c_game_client::set_played_with(steam_id);

  return true;
}

void steam_get_friends_game_info() {}

int steam_get_user_avatar(const unsigned long long userID, const unsigned avatar_size) {
  switch (avatar_size) {
    case enigma_user::steam_user_avatar_size_small:
      return steamworks::c_game_client::get_small_friend_avatar(userID);
      break;
    case enigma_user::steam_user_avatar_size_medium:
      return steamworks::c_game_client::get_medium_friend_avatar(userID);
      break;
    case enigma_user::steam_user_avatar_size_large:
      return steamworks::c_game_client::get_large_friend_avatar(userID);
      break;
    default:
      DEBUG_MESSAGE(
          "Calling steam_get_user_avatar failed. Invalid avatar size. The allowed options are: "
          "steam_user_avatar_size_small, steam_user_avatar_size_medium, and steam_user_avatar_size_large.",
          M_ERROR);
      break;
  }

  return -1;
}

// TODO: Check how ENIGMA compiler handles null values.
unsigned* steam_image_get_size(const long long steam_image_id) {
  unsigned image_size[2] = {0};

  if (!social_pre_checks("steam_image_get_size")) return image_size;

  if (!steamworks::c_game_client::get_image_size(steam_image_id, &image_size[0], &image_size[1])) {
    DEBUG_MESSAGE(
        "Calling steam_image_get_size failed. This happens if the image handle is not "
        "valid or the sizes weren't filled out",
        M_ERROR);
    return image_size;
  }

  return image_size;
}

bool steam_image_get_rgba(const long long steam_image_id, int buffer, int size) { return false; }

bool steam_image_get_bgra(const long long steam_image_id, int buffer, int size) { return false; }

}  // namespace enigma_user
