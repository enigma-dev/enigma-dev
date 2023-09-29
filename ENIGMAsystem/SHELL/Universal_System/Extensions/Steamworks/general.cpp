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

#include "general.h"

#include "gameclient/gc_statsandachievements.h"

bool general_pre_checks(const std::string& script_name) {
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

bool steam_initialised() { return steamworks_gc::GCMain::is_initialised(); }

bool steam_stats_ready() {
  if (!general_pre_checks("steam_stats_ready")) return false;

  return steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->stats_valid();
}

unsigned steam_get_app_id() {
  if (!general_pre_checks("steam_get_app_id")) return 0;

  return steamworks_gc::GCMain::get_gameclient()->get_steam_app_id();
}

unsigned steam_get_user_account_id() {
  if (!general_pre_checks("steam_get_user_account_id")) return 0;

  return steamworks_gc::GCMain::get_gameclient()->get_steam_id_local_user().GetAccountID();
}

unsigned long long steam_get_user_steam_id() {
  if (!general_pre_checks("steam_get_user_steam_id")) return 0;

  return steamworks_gc::GCMain::get_gameclient()->get_steam_id_local_user().ConvertToUint64();
}

std::string steam_get_persona_name() {
  std::string buffer;

  if (!general_pre_checks("steam_get_persona_name")) return buffer;

  steamworks_gc::GameClient::get_steam_persona_name(buffer);

  return buffer;
}

std::string steam_get_user_persona_name(const unsigned long long steamID) {
  std::string buffer;

  if (!general_pre_checks("steam_get_user_persona_name")) return buffer;

  steamworks_gc::GameClient::get_steam_user_persona_name(buffer, steamID);

  return buffer;
}

bool steam_is_user_logged_on() {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_user_logged_on failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  return steamworks_gc::GameClient::is_user_logged_on();
}

std::string steam_current_game_language() {
  std::string buffer;

  if (!general_pre_checks("steam_current_game_language")) return buffer;

  steamworks_gc::GCMain::get_gameclient()->get_current_game_language(buffer);

  return buffer;
}

std::string steam_available_languages() {
  std::string buffer;

  if (!general_pre_checks("steam_available_languages")) return buffer;

  steamworks_gc::GCMain::get_gameclient()->get_available_game_languages(buffer);

  return buffer;
}

bool steam_is_subscribed() {
  if (!general_pre_checks("steam_is_subscribed")) return false;

  return steamworks_gc::GameClient::is_subscribed();
}

void steam_set_warning_message_hook() {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_set_warning_message_hook failed. Make sure that the API is initialized correctly.",
                  M_ERROR);
    return;
  }

  steamworks_gc::GCMain::set_warning_message_hook();
}

}  // namespace enigma_user
