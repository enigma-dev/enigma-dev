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
    This function is used to tell the Steam API to clear (reset) a specific achievement. The achievement 
    should have been previously defined on the Steamworks control panel accounts page for your game and 
    the string that is passed to the function should match that used as the API Name on the control panel.
    Calls c_stats_and_achievements::clear_achievement().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Stats-and-Achievements#steam_clear_achievement
    for more information.
*/
void steam_clear_achievement(const std::string& ach_name);

/*
    
*/
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
