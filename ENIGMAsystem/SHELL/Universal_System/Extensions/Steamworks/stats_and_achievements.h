#ifndef STATS_AND_ACHIEVEMENTS_H
#define STATS_AND_ACHIEVEMENTS_H

#include "game_client/main.h"

namespace enigma_user {

void steam_set_achievement(const std::string& ach_name);
bool steam_get_achievement(const std::string& ach_name);
void steam_clear_achievement(const std::string& ach_name);
void steam_set_stat_int(const std::string& stat_name, int value);
void steam_set_stat_float(const std::string& stat_name, float value);
void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length);
int steam_get_stat_int(const std::string& stat_name);
float steam_get_stat_float(const std::string& stat_name);
float steam_get_stat_avg_rate(const std::string& stat_name);
void steam_reset_all_stats();
void steam_reset_all_stats_achievements();

}  // namespace enigma_user

#endif  // STATS_AND_ACHIEVEMENTS_H
