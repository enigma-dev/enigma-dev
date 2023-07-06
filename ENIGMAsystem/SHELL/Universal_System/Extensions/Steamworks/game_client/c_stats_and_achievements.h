#ifndef GAMECLIENT_STATS_AND_ACHIEVEMENTS_H
#define GAMECLIENT_STATS_AND_ACHIEVEMENTS_H

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_stats_and_achievements {
 public:
  c_stats_and_achievements();
  ~c_stats_and_achievements() = default;

  bool stats_valid();
  void set_achievement(const std::string& achievement_name);
  bool get_achievement(const std::string& achievement_name);
  void clear_achievement(const std::string& achievement_name);
  void reset_all_stats();
  void reset_all_stats_achievements();

  STEAM_CALLBACK(c_stats_and_achievements, on_user_stats_received, UserStatsReceived_t, m_CallbackUserStatsReceived);
  STEAM_CALLBACK(c_stats_and_achievements, on_user_stats_stored, UserStatsStored_t, m_CallbackUserStatsStored);
  STEAM_CALLBACK(c_stats_and_achievements, on_achievement_stored, UserAchievementStored_t, m_CallbackAchievementStored);

 private:
  CGameID c_game_id_;

  bool stats_valid_;

  ISteamUser* steam_user_;
  ISteamUserStats* steam_user_stats_;

  void request_current_stats();
  void store_stats();
};
}  // namespace steamworks

#endif  // GAMECLIENT_STATS_AND_ACHIEVEMENTS_H
