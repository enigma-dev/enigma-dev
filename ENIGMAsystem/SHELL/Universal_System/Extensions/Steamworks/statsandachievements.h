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
 * @brief The Steam Stats and Achievements API provides an easy way for your game to provide persistent, 
 *        roaming achievement and statistics tracking for your users. The user's data is associated with their 
 *        Steam account, and each user's achievements and statistics can be formatted and displayed in their 
 *        Steam Community Profile.
 * 
 * @note You must wait until steam_stats_ready has returned true, before attempting to read or write 
 *       stats and achievements.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#stats-and-achievements 
 *      for more information.
 * 
 */

#ifndef STATSANDACHIEVEMENTS_H
#define STATSANDACHIEVEMENTS_H

#include "gameclient/gc_main.h"

namespace enigma {

/**
 * @brief 
 * 
 */
void steam_store_stats();

/**
 * @brief 
 * 
 */
void steam_request_current_stats();

}  // namespace enigma

namespace enigma_user {

/**
 * @brief This function is used to tell the Steam API to award ("set") an achievement for the player. These 
 *        achievements should have been defined on the Steamworks control panel accounts page for your game 
 *        and the string that is passed to the function should match that used as the API Name on the control 
 *        panel. The Steam Game Overlay will display a notification panel to the user informing them of the 
 *        achievement that they have received, unless the achievement has already been awarded, in which case 
 *        nothing will happen.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_achievement 
 *      for more information.
 * 
 * @param ach_name The name of the achievement to be awarded.
 */
void steam_set_achievement(const std::string& ach_name);

/**
 * @brief This function is used to check the Steam API to see if a specific achievement has been awarded. The 
 *        achievement should have been previously defined on the Steamworks control panel accounts page for your 
 *        game and the string that is passed to the function should match that used as the API Name on the control 
 *        panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_achievement 
 *      for more information.
 * 
 * @param ach_name The name of the achievement to be checked.
 * @return true If the achievement has been awarded.
 * @return false If the achievement has not been awarded.
 */
bool steam_get_achievement(const std::string& ach_name);

/**
 * @brief This function is used to tell the Steam API to clear (reset) a specific achievement. The achievement 
 *        should have been previously defined on the Steamworks control panel accounts page for your game and 
 *        the string that is passed to the function should match that used as the API Name on the control panel.
 * 
 * @note This is primarily only ever used for testing.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_clear_achievement 
 *      for more information.
 * 
 * @param ach_name The name of the achievement to be cleared.
 */
void steam_clear_achievement(const std::string& ach_name);

/**
 * @brief This function is used to set a specific statistic to a new, signed integer, value. The statistic should 
 *        have been previously defined on the Steamworks control panel accounts page for your game and the string 
 *        that is passed to the function should match that used as the API Name on the control panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_int 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be set.
 * @param value The value to set the statistic to.
 */
void steam_set_stat_int(const std::string& stat_name, int value);

/**
 * @brief This function is used to set a specific statistic to a new, floating point, value. The statistic should 
 *        have been previously defined on the Steamworks control panel accounts page for your game and the string 
 *        that is passed to the function should match that used as the API Name on the control panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_float 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be set.
 * @param value The value to set the statistic to.
 */
void steam_set_stat_float(const std::string& stat_name, float value);

/**
 * @brief This function permits you to set an average statistic type with a "sliding window" effect on the average. 
 *        The "session_count" value is the current value that you wish to average out, while the "session_length" is 
 *        the amount of game time since the last call to the function.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_avg_rate 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be set.
 * @param session_count The current value to be averaged.
 * @param session_length The amount of game time since the last call to the function.
 */
void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length);

/**
 * @brief This function is used to get the value of a specific signed integer statistic. The statistic should have 
 *        been previously defined on the Steamworks control panel accounts page for your game and the string that 
 *        is passed to the function should match that used as the API Name on the control panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_int 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be retrieved.
 * @return int The value of the statistic.
 */
int steam_get_stat_int(const std::string& stat_name);

/**
 * @brief This function is used to get the value of a specific floating point statistic. The statistic should have 
 *        been previously defined on the Steamworks control panel accounts page for your game and the string that 
 *        is passed to the function should match that used as the API Name on the control panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_float 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be retrieved.
 * @return float The value of the statistic.
 */
float steam_get_stat_float(const std::string& stat_name);

/**
 * @brief This function is used to get the value of a specific average statistic. The statistic should have been 
 *        previously defined on the Steamworks control panel accounts page for your game and the string that is 
 *        passed to the function should match that used as the API Name on the control panel.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_avg_rate 
 *      for more information.
 * 
 * @param stat_name The name of the statistic to be retrieved.
 * @return float The value of the statistic.
 */
float steam_get_stat_avg_rate(const std::string& stat_name);

/**
 * @brief This function is used to reset all the statistics for the current user to their default values (as 
 *        defined in the Steamworks control panel for your game). If need to also reset the achievement to their 
 *        default values use the @c steam_reset_all_stats_achievements() instead.
 * 
 * @note It is recommended that you only use this function as a debug tool when developing 
 *       your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_reset_all_stats 
 *      for more information.
 * 
 */
void steam_reset_all_stats();

/**
 * @brief This function is used to reset all the statistics and achievements for the current user to their default 
 *        values (as defined in the Steamworks control panel for your game). If you only need to reset the stats to 
 *        their default values use the @c steam_reset_all_stats() instead.
 * 
 * @note It is recommended that you only use this function as a debug tool when developing 
 *       your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_reset_all_stats_achievements 
 *      for more information.
 * 
 */
void steam_reset_all_stats_achievements();

}  // namespace enigma_user

#endif  // STATSANDACHIEVEMENTS_H
