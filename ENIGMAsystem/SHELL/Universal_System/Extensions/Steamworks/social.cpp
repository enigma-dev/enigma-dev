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

bool social_pre_checks(const std::string& script_name) { return true; }

namespace enigma_user {

void steam_set_rich_presence(const std::string& key, const std::string& value) {}

void steam_clear_rich_presence() {}

bool steam_user_set_played_with(long long steam_id) { return false; }

void steam_get_friends_game_info() {}

int steam_get_user_avatar(long long userID, int avatar_size) { return -1; }

void steam_image_get_size(long long steam_image_id) {}

bool steam_image_get_rgba(long long steam_image_id, int buffer, int size) { return false; }

bool steam_image_get_bgra(long long steam_image_id, int buffer, int size) { return false; }

}  // namespace enigma_user
