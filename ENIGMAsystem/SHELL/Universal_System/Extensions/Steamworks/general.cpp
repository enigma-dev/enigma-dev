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

#include "general.h"

#include "game_client/c_stats_and_achievements.h"

bool general_pre_checks(const std::string& script_name) {
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

bool steam_initialised() { return steamworks::c_main::is_initialised(); }

bool steam_stats_ready() {
  if (!general_pre_checks("steam_stats_ready")) return false;

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid();
}

unsigned steam_get_app_id() {
  if (!general_pre_checks("steam_get_app_id")) return 0;

  return steamworks::c_main::get_c_game_client()->get_steam_app_id();
}

unsigned steam_get_user_account_id() {
  if (!general_pre_checks("steam_get_user_account_id")) return 0;

  return steamworks::c_main::get_c_game_client()->get_c_steam_id_local_user().GetAccountID();
}

unsigned long long steam_get_user_steam_id() {
  if (!general_pre_checks("steam_get_user_steam_id")) return 0;

  return steamworks::c_main::get_c_game_client()->get_c_steam_id_local_user().ConvertToUint64();
}

std::string steam_get_persona_name() {
  if (!general_pre_checks("steam_get_persona_name")) return "-1";

  return steamworks::c_game_client::get_steam_persona_name();
}

std::string steam_get_user_persona_name(CSteamID user_persona_name) {
  if (!general_pre_checks("steam_get_user_persona_name")) return "-1";

  return steamworks::c_game_client::get_steam_user_persona_name(user_persona_name);
}

bool steam_is_user_logged_on() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_user_logged_on failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  return steamworks::c_game_client::is_user_logged_on();
}

std::string steam_current_game_language() {
  if (!general_pre_checks("steam_current_game_language")) return "-1";

  return steamworks::c_main::get_c_game_client()->get_current_game_language();
}

std::string steam_available_languages() {
  if (!general_pre_checks("steam_available_languages")) return "-1";

  return steamworks::c_main::get_c_game_client()->get_available_game_languages();
}

bool steam_is_subscribed() {
  if (!general_pre_checks("steam_is_subscribed")) return false;

  return steamworks::c_game_client::is_subscribed();
}

void steam_set_warning_message_hook() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_set_warning_message_hook failed. Make sure that the API is initialized correctly.",
                  M_ERROR);
    return;
  }

  steamworks::c_main::set_warning_message_hook();
}

}  // namespace enigma_user
