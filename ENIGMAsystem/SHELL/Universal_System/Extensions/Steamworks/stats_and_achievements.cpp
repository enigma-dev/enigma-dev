#include "stats_and_achievements.h"

#include "game_client/c_stats_and_achievements.h"

bool stats_and_achievements_pre_checks(const std::string& script_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE("Calling " + script_name +
                      " failed. You must request the user's current stats and achievements from the "
                      "server.",
                  M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

void steam_set_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_set_achievement")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->set_achievement(ach_name);
}

bool steam_get_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_get_achievement")) return false;

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->get_achievement(ach_name);
}

void steam_clear_achievement(const std::string& ach_name) {
  if (!stats_and_achievements_pre_checks("steam_clear_achievement")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->clear_achievement(ach_name);
}

void steam_set_stat_int(const std::string& stat_name, int value) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_int")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->set_stat_int(stat_name, value);
}

int steam_get_stat_int(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_int")) return -1;

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->get_stat_int(stat_name);
}

void steam_set_stat_float(const std::string& stat_name, float value) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_float")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->set_stat_float(stat_name, value);
}

float steam_get_stat_float(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_float")) return -1.0f;

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->get_stat_float(stat_name);
}

void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length) {
  if (!stats_and_achievements_pre_checks("steam_set_stat_avg_rate")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->set_stat_average_rate(
      stat_name, session_count, session_length);
}

float steam_get_stat_avg_rate(const std::string& stat_name) {
  if (!stats_and_achievements_pre_checks("steam_get_stat_avg_rate")) return -1.0f;

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->get_stat_average_rate(stat_name);
}

void steam_reset_all_stats() {
  if (!stats_and_achievements_pre_checks("steam_reset_all_stats")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->reset_all_stats();
}

void steam_reset_all_stats_achievements() {
  if (!stats_and_achievements_pre_checks("steam_reset_all_stats_achievements")) return;

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->reset_all_stats_achievements();
}

}  // namespace enigma_user
