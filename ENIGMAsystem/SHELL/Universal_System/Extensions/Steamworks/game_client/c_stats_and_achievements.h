#ifndef GAMECLIENT_STATS_AND_ACHIEVEMENTS_H
#define GAMECLIENT_STATS_AND_ACHIEVEMENTS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_stats_and_achievements {
 public:
  /*
    Stats and Achievements client constructor.
  */
  c_stats_and_achievements();

  /*
    Stats and Achievements client destructor.
  */
  ~c_stats_and_achievements() = default;

  /*
    Checks if the stats are valid. This function returns stats_valid_ variable.
  */
  bool stats_valid();

  /*
    This function has two important steps:
      1.  Calls SetAchievement() function. Unlocks an achievement. You must have called 
      c_stats_and_achievements::request_current_stats() and it needs to return successfully 
      via its callback prior to calling this! You can unlock an achievement multiple times so you 
      don't need to worry about only setting achievements that aren't already set. This call only 
      modifies Steam's in-memory state so it is quite cheap. To send the unlock status to the server 
      and to trigger the Steam overlay notification you must call c_stats_and_achievements::store_stats(). 
      Check https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement for more information.

      2.  Calls c_stats_and_achievements::store_stats() function to store the stats.
  */
  void set_achievement(const std::string& achievement_name);

  /*
    Gets the unlock status of the Achievement. Calls GetAchievement() function. 
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement for more information.
  */
  bool get_achievement(const std::string& achievement_name);

  /*
    NOTE:   This is primarily only ever used for testing.

    Resets the unlock status of an achievement. You must have called 
    c_stats_and_achievements::request_current_stats() and it needs to return successfully via its 
    callback prior to calling this! This call only modifies Steam's in-memory state so it is quite 
    cheap. To send the unlock status to the server and to trigger the Steam overlay notification 
    you must call c_stats_and_achievements::store_stats(). Calls ClearAchievement() function. 
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement for more information.
  */
  void clear_achievement(const std::string& achievement_name);

  /*
    NOTE:   This is the int overload.

    This function has two important steps:
      1.  Calls SetStat() function. Sets / updates the value of a given stat for the current user. 
          You must have called c_stats_and_achievements::request_current_stats() and it needs to return 
          successfully via its callback prior to calling this! This call only modifies Steam's in-memory 
          state and is very cheap. Doing so allows Steam to persist the changes even in the event of a 
          game crash or unexpected shutdown. To submit the stats to the server you must call 
          c_stats_and_achievements::store_stats(). 
          Check https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat for more information.

      2.  Calls c_stats_and_achievements::store_stats() function to store the stats.
  */
  void set_stat_int(const std::string& stat_name, const int value);

  /*
    NOTE:   This is the int overload.

    Gets the current value of the a stat for the current user. You must have called 
    c_stats_and_achievements::request_current_stats() and it needs to return successfully via its 
    callback prior to calling this. Calls GetStat() function. 
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat for more information.
  */
  int get_stat_int(const std::string& stat_name);

  /*
    NOTE:   This is the float overload.

    This function has two important steps:
      1.  Calls SetStat() function. Sets / updates the value of a given stat for the current user. 
          You must have called c_stats_and_achievements::request_current_stats() and it needs to return 
          successfully via its callback prior to calling this! This call only modifies Steam's in-memory 
          state and is very cheap. Doing so allows Steam to persist the changes even in the event of a 
          game crash or unexpected shutdown. To submit the stats to the server you must call 
          c_stats_and_achievements::store_stats(). 
          Check https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat for more information.

      2.  Calls c_stats_and_achievements::store_stats() function to store the stats.
  */
  void set_stat_float(const std::string& stat_name, const float value);

  /*
    NOTE:   This is the float overload.

    Gets the current value of the a stat for the current user. You must have called 
    c_stats_and_achievements::request_current_stats() and it needs to return successfully via its 
    callback prior to calling this. Calls GetStat() function. 
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat for more information.
  */
  float get_stat_float(const std::string& stat_name);

  /*
    This function has two important steps:
      1.  Calls UpdateAvgRateStat() function. Updates an AVGRATE stat with new values. You must 
          have called c_stats_and_achievements::request_current_stats() and it needs to return 
          successfully via its callback prior to calling this! This call only modifies Steam's in-memory 
          state and is very cheap. Doing so allows Steam to persist the changes even in the event of a 
          game crash or unexpected shutdown. To submit the stats to the server you must call 
          c_stats_and_achievements::store_stats(). 
          Check https://partner.steamgames.com/doc/api/ISteamUserStats#UpdateAvgRateStat for more 
          information.

      2.  Calls c_stats_and_achievements::store_stats() function to store the stats.
  */
  void set_stat_average_rate(const std::string& stat_name, const float count_this_session, const double session_length);

  /*
    Gets the current value of the a stat for the current user. You must have called 
    c_stats_and_achievements::request_current_stats() and it needs to return successfully via its 
    callback prior to calling this. Calls GetStat() function. 
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat for more information.
  */
  float get_stat_average_rate(const std::string& stat_name);

  /*
    Macro for listening to UserStatsReceived_t callback. Callbacks are dispatched by
    calling c_main::run_callbacks().
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t for
    more information.
  */
  STEAM_CALLBACK(c_stats_and_achievements, on_user_stats_received, UserStatsReceived_t, m_CallbackUserStatsReceived);

  /*
    Macro for listening to UserStatsStored_t callback. Callbacks are dispatched by
    calling c_main::run_callbacks().
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsStored_t for
    more information.
  */
  STEAM_CALLBACK(c_stats_and_achievements, on_user_stats_stored, UserStatsStored_t, m_CallbackUserStatsStored);

  /*
    Macro for listening to UserAchievementStored_t callback. Callbacks are dispatched by
    calling c_main::run_callbacks().
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#UserAchievementStored_t for
    more information.
  */
  STEAM_CALLBACK(c_stats_and_achievements, on_achievement_stored, UserAchievementStored_t, m_CallbackAchievementStored);

  /*
    Resets the current users stats and, optionally achievements. This automatically calls 
    c_stats_and_achievements::store_stats() to persist the changes to the server. This should 
    typically only be used for testing purposes during development. Ensure that you sync up 
    your stats with the new default values provided by Steam after calling this by calling 
    c_stats_and_achievements::request_current_stats(). Calls ResetAllStats() function.
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats for more information.
  */
  void reset_all_stats();

  /*
    See c_stats_and_achievements::reset_all_stats() above for more information.
  */
  void reset_all_stats_achievements();

 private:
  /*
    Stores AppID of the game.
  */
  CGameID c_game_id_;

  /*
    Stores stats status.
  */
  bool stats_valid_;

  /*
    Pointer to ISteamUser.
  */
  ISteamUser* steam_user_;

  /*
    Pointer to ISteamUserStats.
  */
  ISteamUserStats* steam_user_stats_;

  /*
    Asynchronously request the user's current stats and achievements from the server. You must 
    always call this first to get the initial status of stats and achievements. Only after the 
    resulting callback comes back can you start calling the rest of the stats and achievement 
    functions for the current user. Triggers a UserStatsReceived_t callback. Calls 
    RequestCurrentStats() function.
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#RequestCurrentStats for more information.
  */
  void request_current_stats();

  /*
    Send the changed stats and achievements data to the server for permanent storage. If this 
    fails then nothing is sent to the server. It's advisable to keep trying until the call is 
    successful. If you have stats or achievements that you have saved locally but haven't uploaded 
    with this function when your application process ends then this function will automatically 
    be called. If the call is successful you will receive a UserStatsStored_t and 
    UserAchievementStored_t callbacks. Calls StoreStats() function.
    Check https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats for more information.
  */
  void store_stats();
};
}  // namespace steamworks

#endif  // GAMECLIENT_STATS_AND_ACHIEVEMENTS_H
