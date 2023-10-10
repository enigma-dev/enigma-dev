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

/**
 * @brief Steam Stats and Achievements provides an easy way for your game to provide persistent, roaming 
 *        achievement and statistics tracking for your users. The user's data is associated with their 
 *        Steam account, and each user's achievements and statistics can be formatted and displayed in 
 *        their Steam Community Profile.
 * 
 * @see https://partner.steamgames.com/doc/features/achievements for more information.
 * 
 */

#ifndef GC_STATSANDACHIEVEMENTS_H
#define GC_STATSANDACHIEVEMENTS_H
#pragma once

#include "gameclient.h"

namespace steamworks_gc {

class GameClient;

class GCStatsAndAchievements {
 public:
  /**
   * @brief GCStatsAndAchievements constructor.
   * 
   * @param app_id the appID of the current process. This is will be used for inistantiating
   *               a new CGameID object.
   */
  GCStatsAndAchievements(uint32 app_id);

  // GCStatsAndAchievements destructor.
  ~GCStatsAndAchievements() = default;

  /**
   * @brief Checks if the stats are valid.
   * 
   * @return true if stats received successfully.
   * @return false if stats not received successfully.
   */
  bool stats_valid();

  /**
   * @brief Calls @c SteamBinder::ISteamUserStats_SetAchievement() function. Unlocks an 
   *        achievement. You must have called @c GCStatsAndAchievements::request_current_stats() 
   *        and it needs to return successfully via its callback prior to calling this! You can 
   *        unlock an achievement multiple times so you don't need to worry about only setting 
   *        achievements that aren't already set. This call only modifies Steam's in-memory 
   *        state so it is quite cheap. To send the unlock status to the server and to trigger 
   *        the Steam overlay notification you must call @c GCStatsAndAchievements::store_stats()
   *        function.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#SetAchievement for more information.
   * 
   * @param achievement_name the name of the achievement to unlock.
   * @return true If the achievement was successfully unlocked.
   * @return false If the achievement was not successfully unlocked.
   */
  bool set_achievement(const std::string& achievement_name);

  /**
   * @brief Gets the unlock status of the Achievement.
   * 
   * @note the unlock status is stored in the @c achieved variable which is passed by reference.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#GetAchievement for more information.
   * 
   * @param achievement_name the name of the achievement to get the unlock status of.
   * @param achieved the unlock status of the achievement.
   * @return true If the achievement was successfully retrieved.
   * @return false If the achievement was not successfully retrieved.
   */
  bool get_achievement(const std::string& achievement_name, bool& achieved);

  /**
   * @brief Resets the unlock status of an achievement. You must have called 
   *        @c GCStatsAndAchievements::request_current_stats() and it needs to return successfully via its 
   *        callback prior to calling this! This call only modifies Steam's in-memory state so it is quite 
   *        cheap. To send the unlock status to the server and to trigger the Steam overlay notification 
   *        you must call @c GCStatsAndAchievements::store_stats() function.
   * 
   * @note This is primarily only ever used for testing.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#ClearAchievement for more 
   *      information.
   * 
   * @param achievement_name the name of the achievement to reset.
   * @return true If the achievement was successfully reset.
   * @return false If the achievement was not successfully reset.
   */
  bool clear_achievement(const std::string& achievement_name);

  /**
   * @brief Sets / updates the value of a given stat for the current user. You must have called 
   *        @c GCStatsAndAchievements::request_current_stats() and it needs to return successfully
   *        via its callback prior to calling this! This call only modifies Steam's in-memory 
   *        state and is very cheap. Doing so allows Steam to persist the changes even in the 
   *        event of a game crash or unexpected shutdown. To submit the stats to the server you 
   *        must call @c GCStatsAndAchievements::store_stats() function.
   * 
   * @note This is the @c int overload.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat for more information.
   * 
   * @param stat_name the name of the stat to set.
   * @param value the value to set the stat to.
   * @return true If the stat was successfully set.
   * @return false If the stat was not successfully set.
   */
  bool set_stat_int(const std::string& stat_name, const int32& value);

  /**
   * @brief Gets the current value of the a stat for the current user. You must have called 
   *        @c GCStatsAndAchievements::request_current_stats() and it needs to return successfully via its 
   *        callback prior to calling this.
   * 
   * @note This is the int overload.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat for more information.
   * 
   * @param stat_name the name of the stat to get the value of.
   * @param value the value of the stat.
   * @return true If the stat was successfully retrieved.
   * @return false If the stat was not successfully retrieved.
   */
  bool get_stat_int(const std::string& stat_name, int32& value);

  /**
   * @brief Sets / updates the value of a given stat for the current user. 
   *        You must have called @c GCStatsAndAchievements::request_current_stats() and it needs to return 
   *        successfully via its callback prior to calling this! This call only modifies Steam's in-memory 
   *        state and is very cheap. Doing so allows Steam to persist the changes even in the event of a 
   *        game crash or unexpected shutdown. To submit the stats to the server you must call 
   *        @c GCStatsAndAchievements::store_stats() function.
   * 
   * @note This is the float overload.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#SetStat for more information.
   * 
   * @param stat_name the name of the stat to set.
   * @param value the value to set the stat to.
   * @return true If the stat was successfully set.
   * @return false If the stat was not successfully set.
   */
  bool set_stat_float(const std::string& stat_name, const float& value);

  /**
   * @brief Gets the current value of the a stat for the current user. You must have called 
   *        @c GCStatsAndAchievements::request_current_stats() and it needs to return successfully 
   *        via its callback prior to calling this.
   * 
   * @note This is the float overload.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#GetStat for more information.
   * 
   * @param stat_name the name of the stat to get the value of.
   * @param value the value of the stat.
   * @return true If the stat was successfully retrieved.
   * @return false If the stat was not successfully retrieved.
   */
  bool get_stat_float(const std::string& stat_name, float& value);

  /**
   * @brief Updates an AVGRATE stat with new values. You must have called 
   *        @c GCStatsAndAchievements::request_current_stats() and it needs to return successfully
   *        via its callback prior to calling this! This call only modifies Steam's in-memory 
   *        state and is very cheap. Doing so allows Steam to persist the changes even in the 
   *        event of a game crash or unexpected shutdown. To submit the stats to the server you 
   *        must call @c GCStatsAndAchievements::store_stats() function.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats()function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#UpdateAvgRateStat for more 
   *      information.
   * 
   * @param stat_name the name of the stat to set.
   * @param count_this_session The value accumulation since the last call to this function.
   * @param session_length The amount of time in seconds since the last call to this function.
   * @return true If the stat was successfully updated.
   * @return false If the stat was not successfully updated.
   */
  bool set_stat_average_rate(const std::string& stat_name, const float& count_this_session,
                             const double& session_length);

  /**
   * @brief Send the changed stats and achievements data to the server for permanent storage. If this 
   *        fails then nothing is sent to the server. It's advisable to keep trying until the call is 
   *        successful. If you have stats or achievements that you have saved locally but haven't uploaded 
   *        with this function when your application process ends then this function will automatically 
   *        be called. If the call is successful you will receive a @c UserStatsStored_t and 
   *        @c UserAchievementStored_t callbacks.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#StoreStats for more information.
   * 
   * @return true if the stats were successfully stored.
   * @return false if the stats were not successfully stored.
   */
  bool store_stats();

  /**
   * @brief Asynchronously request the user's current stats and achievements from the server. You must 
   *        always call this first to get the initial status of stats and achievements. Only after the 
   *        resulting callback comes back can you start calling the rest of the stats and achievement 
   *        functions for the current user. Triggers a @c UserStatsReceived_t callback.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#RequestCurrentStats for more information.
   * 
   * @return true If the request was successfully sent to the server.
   * @return false If the request was not successfully sent to the server.
   */
  bool request_current_stats();

  /**
   * @brief Construct a new steam callback object. This is a code generation macro for 
   *        listening to @c UserStatsReceived_t callback.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsReceived_t for 
   *      more information.
   * @see @c GCMain::run_callbacks() function.
   * 
   */
  STEAM_CALLBACK(GCStatsAndAchievements, on_user_stats_received, UserStatsReceived_t, m_CallbackUserStatsReceived);

  /**
   * @brief Construct a new steam callback object. This is a code generation macro for 
   *        listening to @c UserStatsStored_t callback.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#UserStatsStored_t for 
   *      more information.
   * @see @c GCMain::run_callbacks() function.
   * 
   */
  STEAM_CALLBACK(GCStatsAndAchievements, on_user_stats_stored, UserStatsStored_t, m_CallbackUserStatsStored);

  /**
   * @brief Construct a new steam callback object. This is a code generation macro for 
   *        listening to @c UserAchievementStored_t callback.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#UserAchievementStored_t for 
   *      more information.
   * @see @c GCMain::run_callbacks() function.
   * 
   */
  STEAM_CALLBACK(GCStatsAndAchievements, on_achievement_stored, UserAchievementStored_t, m_CallbackAchievementStored);

  /**
   * @brief Resets the current users stats and, optionally achievements. This automatically calls 
   *        @c GCStatsAndAchievements::store_stats() to persist the changes to the server. This should 
   *        typically only be used for testing purposes during development. Ensure that you sync up 
   *        your stats with the new default values provided by Steam after calling this by calling 
   *        @c GCStatsAndAchievements::request_current_stats() function.
   * 
   * @note This is primarily only ever used for testing.
   * 
   * @see @c GCStatsAndAchievements::request_current_stats() function.
   * @see @c GCStatsAndAchievements::store_stats() function.
   * @see https://partner.steamgames.com/doc/api/ISteamUserStats#ResetAllStats for more 
   *      information.
   * 
   * @param achievements_too if we should reset achievements as well.
   * @return true 
   * @return false 
   */
  bool reset_all_stats(const bool& achievements_too = false);

 private:
  /**
   * @brief Stores ID of the game.
   * 
   */
  CGameID game_id_;

  /**
   * @brief Stores stats status.
   * 
   * @see @c GCStatsAndAchievements::stats_valid() function.
   * 
   */
  bool stats_valid_;

  // Pointer to ISteamUser.
  ISteamUser* steam_user_;

  // Pointer to ISteamUserStats.
  ISteamUserStats* steam_user_stats_;
};

}  // namespace steamworks_gc

#endif  // GC_STATSANDACHIEVEMENTS_H
