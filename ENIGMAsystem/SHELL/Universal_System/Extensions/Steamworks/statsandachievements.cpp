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

#include "statsandachievements.h"

#include "gameclient/gc_statsandachievements.h"

bool stats_and_achievements_pre_checks(const std::string& script_name) {
  if (!steamworks_gc::GCMain::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks_gc::GameClient::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->stats_valid()) {
    DEBUG_MESSAGE("Calling " + script_name +
                      " failed. You must request the user's current stats and achievements from the "
                      "server.",
                  M_ERROR);
    return false;
  }

  return true;
}

namespace enigma {

void steam_store_stats() {
  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->store_stats()) {
    DEBUG_MESSAGE(
        "Calling StoreStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback and the current game has stats associated with it in the Steamworks Partner backend, and those stats "
        "are published.",
        M_WARNING);
  }
}

void steam_request_current_stats() {
  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->request_current_stats()) {
    DEBUG_MESSAGE(
        "Calling RequestCurrentStats failed. Only returns false if there is no user logged in; otherwise, true.",
        M_WARNING);
  }
}

}  // namespace enigma

namespace enigma_user {

void steam_set_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_set_achievement")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->set_achievement(ach_name)) {
    DEBUG_MESSAGE("Calling SetAchievement failed for '" + ach_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return;
  }

  enigma::steam_store_stats();
}

bool steam_get_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_get_achievement")) return false;

  bool achieved{false};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->get_achievement(ach_name, achieved)) {
    DEBUG_MESSAGE("Calling GetAchievement failed for '" + ach_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return false;
  }

  return achieved;
}

void steam_clear_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_clear_achievement")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->clear_achievement(ach_name)) {
    DEBUG_MESSAGE("Calling ClearAchievement failed for '" + ach_name +
                      "'. Make sure that RequestCurrentStats has completed and successfully returned its callback and "
                      "the API Name of the specified achievement exists in App Admin on the Steamworks website, and "
                      "the changes are published.",
                  M_ERROR);
    return;
  }

  enigma::steam_store_stats();
}

void steam_set_stat_int(const std::string& stat_name, int value) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_int")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->set_stat_int(stat_name, value)) {
    DEBUG_MESSAGE(
        "Calling SetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return;
  }

  enigma::steam_store_stats();
}

void steam_set_stat_float(const std::string& stat_name, float value) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_float")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->set_stat_float(stat_name, value)) {
    DEBUG_MESSAGE(
        "Calling SetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return;
  }
}

void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_avg_rate")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->set_stat_average_rate(
          stat_name, session_count, session_length)) {
    DEBUG_MESSAGE(
        "Calling UpdateAvgRateStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type must be "
            "AVGRATE in the Steamworks Partner backend.",
        M_ERROR);
    return;
  }

  enigma::steam_store_stats();
}

int steam_get_stat_int(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_int")) return -1;

  int32 value{-1};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->get_stat_int(stat_name, value)) {
    DEBUG_MESSAGE(
        "Calling GetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return -1;
  }

  return value;
}

float steam_get_stat_float(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_float")) return -1.0f;

  float value{-1.0f};

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->get_stat_float(stat_name, value)) {
    DEBUG_MESSAGE(
        "Calling GetStat failed for '" + stat_name +
            "'. Make sure that RequestCurrentStats has completed and successfully returned its callback, the specified "
            "stat exists in App Admin on the Steamworks website, and the changes are published, and the type passed to "
            "this function must match the type listed in the App Admin panel of the Steamworks website.",
        M_ERROR);
    return -1;
  }

  return value;
}

float steam_get_stat_avg_rate(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_avg_rate")) return -1.0f;

  return enigma_user::steam_get_stat_float(stat_name);
}

void steam_reset_all_stats() {
  if (!stats_and_achievements_pre_checks("steam_reset_all_stats")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->reset_all_stats()) {
    DEBUG_MESSAGE(
        "Calling ResetAllStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback.",
        M_ERROR);
    return;
  }

  enigma::steam_request_current_stats();
}

void steam_reset_all_stats_achievements() {
  if (!stats_and_achievements_pre_checks("steam_reset_all_stats_achievements")) return;

  if (!steamworks_gc::GCMain::get_gameclient()->get_gc_statsandachievements()->reset_all_stats(true)) {
    DEBUG_MESSAGE(
        "Calling ResetAllStats failed. Make sure that RequestCurrentStats has completed and successfully returned its "
        "callback.",
        M_ERROR);
    return;
  }

  enigma::steam_request_current_stats();
}

}  // namespace enigma_user
