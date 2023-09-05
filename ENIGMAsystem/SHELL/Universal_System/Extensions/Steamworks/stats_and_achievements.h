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

/*
    NOTE:   You must wait until steam_stats_ready has returned true, before attempting to read or write 
            stats and achievements.

    The Steam Stats and Achievements API provides an easy way for your game to provide persistent, 
    roaming achievement and statistics tracking for your users. The user's data is associated with their 
    Steam account, and each user's achievements and statistics can be formatted and displayed in their 
    Steam Community Profile.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#stats-and-achievements
    for more information.
*/

#ifndef STATS_AND_ACHIEVEMENTS_H
#define STATS_AND_ACHIEVEMENTS_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client/c_main.h"

namespace enigma {

void steam_store_stats();
void steam_request_current_stats();

}  // namespace enigma

namespace enigma_user {

/*
    This function is used to tell the Steam API to award ("set") an achievement for the player. These 
    achievements should have been defined on the Steamworks control panel accounts page for your game 
    and the string that is passed to the function should match that used as the API Name on the control 
    panel. The Steam Game Overlay will display a notification panel to the user informing them of the 
    achievement that they have received, unless the achievement has already been awarded, in which case 
    nothing will happen. Calls c_stats_and_achievements::set_achievement().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_achievement
    for more information.
*/
void steam_set_achievement(const std::string& ach_name);

/*
    This function is used to check the Steam API to see if a specific achievement has been awarded. The 
    achievement should have been previously defined on the Steamworks control panel accounts page for your 
    game and the string that is passed to the function should match that used as the API Name on the control 
    panel. Calls c_stats_and_achievements::get_achievement().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_achievement
    for more information.
*/
bool steam_get_achievement(const std::string& ach_name);

/*
    NOTE:   This is primarily only ever used for testing.

    This function is used to tell the Steam API to clear (reset) a specific achievement. The achievement 
    should have been previously defined on the Steamworks control panel accounts page for your game and 
    the string that is passed to the function should match that used as the API Name on the control panel.
    Calls c_stats_and_achievements::clear_achievement().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_clear_achievement
    for more information.
*/
void steam_clear_achievement(const std::string& ach_name);

/*
    This function is used to set a specific statistic to a new, signed integer, value. The statistic should 
    have been previously defined on the Steamworks control panel accounts page for your game and the string 
    that is passed to the function should match that used as the API Name on the control panel. Calls
    c_stats_and_achievements::set_stat_int().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_int
    for more information.
*/
void steam_set_stat_int(const std::string& stat_name, int value);

/*
    This function is used to set a specific statistic to a new, floating point, value. The statistic should 
    have been previously defined on the Steamworks control panel accounts page for your game and the string 
    that is passed to the function should match that used as the API Name on the control panel. Calls
    c_stats_and_achievements::set_stat_float().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_float
    for more information.
*/
void steam_set_stat_float(const std::string& stat_name, float value);

/*
    This function permits you to set an average statistic type with a "sliding window" effect on the average. 
    The "session_count" value is the current value that you wish to average out, while the "session_length" is 
    the amount of game time since the last call to the function. Calls c_stats_and_achievements::set_stat_average_rate().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_set_stat_avg_rate
    for more information.
*/
void steam_set_stat_avg_rate(const std::string& stat_name, float session_count, float session_length);

/*
    This function is used to get the value of a specific signed integer statistic. The statistic should have 
    been previously defined on the Steamworks control panel accounts page for your game and the string that 
    is passed to the function should match that used as the API Name on the control panel. Calls
    c_stats_and_achievements::get_stat_int().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_int
    for more information.
*/
int steam_get_stat_int(const std::string& stat_name);

/*
    This function is used to get the value of a specific floating point statistic. The statistic should have 
    been previously defined on the Steamworks control panel accounts page for your game and the string that 
    is passed to the function should match that used as the API Name on the control panel. Calls
    c_stats_and_achievements::get_stat_float().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_float
    for more information.
*/
float steam_get_stat_float(const std::string& stat_name);

/*
    This function is used to get the value of a specific average statistic. The statistic should have been 
    previously defined on the Steamworks control panel accounts page for your game and the string that is 
    passed to the function should match that used as the API Name on the control panel. Calls
    c_stats_and_achievements::get_stat_average_rate().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_get_stat_avg_rate
    for more information.
*/
float steam_get_stat_avg_rate(const std::string& stat_name);

/*
    TIP:   It is recommended that you only use this function as a debug tool when developing your game.

    This function is used to reset all the statistics for the current user to their default values (as 
    defined in the Steamworks control panel for your game). If need to also reset the achievement to their 
    default values use the steam_reset_all_stats_achievements() instead. Calls 
    c_stats_and_achievements::reset_all_stats().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_reset_all_stats
    for more information.
*/
void steam_reset_all_stats();

/*
    TIP:   It is recommended that you only use this function as a debug tool when developing your game.

    This function is used to reset all the statistics and achievements for the current user to their default 
    values (as defined in the Steamworks control panel for your game). If you only need to reset the stats to 
    their default values use the steam_reset_all_stats() instead. Calls c_stats_and_achievements::reset_all_stats().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_reset_all_stats_achievements
    for more information.
*/
void steam_reset_all_stats_achievements();

}  // namespace enigma_user

#endif  // STATS_AND_ACHIEVEMENTS_H
