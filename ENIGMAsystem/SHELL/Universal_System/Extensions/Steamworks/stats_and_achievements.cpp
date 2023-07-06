#include "stats_and_achievements.h"

#include "game_client/c_stats_and_achievements.h"

namespace enigma_user {

void steam_set_achievement(const std::string& ach_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_set_achievement failed. Make sure that the API is initialized correctly.", M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_set_achievement failed. Make sure that the user is logged in.", M_ERROR);
    return;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE(
        "Calling steam_set_achievement failed. You must request the user's current stats and achievements from the "
        "server.",
        M_ERROR);
    return;
  }

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->set_achievement(ach_name);
}

bool steam_get_achievement(const std::string& ach_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_achievement failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_achievement failed. Make sure that the user is logged in.", M_ERROR);
    return false;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE(
        "Calling steam_get_achievement failed. You must request the user's current stats and achievements from the "
        "server.",
        M_ERROR);
    return false;
  }

  return steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->get_achievement(ach_name);
}

void steam_clear_achievement(const std::string& ach_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_clear_achievement failed. Make sure that the API is initialized correctly.", M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_clear_achievement failed. Make sure that the user is logged in.", M_ERROR);
    return;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE(
        "Calling steam_clear_achievement failed. You must request the user's current stats and achievements from the "
        "server.",
        M_ERROR);
    return;
  }

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->clear_achievement(ach_name);
}

void steam_set_stat_int(const std::string& stat_name, int value) {}

void steam_set_stat_float(const std::string& stat_name, float value) {}

void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length) {}

int steam_get_stat_int(const std::string& stat_name) { return 0; }

float steam_get_stat_float(const std::string& stat_name) { return 0.0f; }

float steam_get_stat_avg_rate(const std::string& stat_name) { return 0.0f; }

void steam_reset_all_stats() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_reset_all_stats failed. Make sure that the API is initialized correctly.", M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_reset_all_stats failed. Make sure that the user is logged in.", M_ERROR);
    return;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE(
        "Calling steam_reset_all_stats failed. You must request the user's current stats and achievements from the "
        "server.",
        M_ERROR);
    return;
  }

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->reset_all_stats();
}

void steam_reset_all_stats_achievements() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_reset_all_stats_achievements failed. Make sure that the API is initialized correctly.",
                  M_ERROR);
    return;
  }

  if (!steamworks::c_game_client::is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_reset_all_stats_achievements failed. Make sure that the user is logged in.", M_ERROR);
    return;
  }

  if (!steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->stats_valid()) {
    DEBUG_MESSAGE(
        "Calling steam_reset_all_stats_achievements failed. You must request the user's current stats and achievements "
        "from the server.",
        M_ERROR);
    return;
  }

  steamworks::c_main::get_c_game_client()->get_c_stats_and_achievements()->reset_all_stats_achievements();
}

}  // namespace enigma_user
