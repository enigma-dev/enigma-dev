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
 * @brief The following set of functions are all for checking the availability of certain 
 *        aspects of the Steam client or server API. This means that these functions should be 
 *        used before any other Steam API function call to ensure that the client/server setup 
 *        is correct and communicating with your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#general for more 
 *      information.
 * 
 */

#ifndef GENERAL_H
#define GENERAL_H

#include "gameclient/gc_main.h"

namespace enigma_user {

/**
 * @brief This function can be called to check that the Steam client API has been initialised 
 *        correctly before any doing any further calls to Steam specific functions in your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_initialised for 
 *      more information.
 * 
 * @return true if the Steam client API has been initialised correctly.
 * @return false if the Steam client API has not been initialised correctly.
 */
bool steam_initialised();

/**
 * @brief This function can be called to check that the Steam client API has correctly initialised 
 *        the statistics for your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_stats_ready 
 *      for more information.
 * 
 * @return true If the Steam client API has correctly initialised the statistics for your game.
 * @return false If the Steam client API has not correctly initialised the statistics for your game.
 */
bool steam_stats_ready();

/**
 * @brief This function is used retrieve the unique app ID that Steam assigns for your game.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_app_id 
 *      for more information.
 * 
 * @return unsigned The unique app ID that Steam assigns for your game.
 */
unsigned steam_get_app_id();

/**
 * @brief This function is used retrieve the unique User ID that Steam assigns to each user.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_account_id 
 *      for more information.
 * 
 * @return unsigned The unique User ID that Steam assigns to each user.
 */
unsigned steam_get_user_account_id();

/**
 * @brief This function is used to return the unique Steam user id of the user currently logged into 
 *        the Steam client.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_steam_id 
 *      for more information.
 * 
 * @return unsigned long long The unique Steam user id of the user currently logged into the 
 *         Steam client.
 */
unsigned long long steam_get_user_steam_id();

/**
 * @brief this function to return the user name of the user currently logged into the Steam client.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_persona_name 
 *      for more information.
 * 
 * @return std::string The user name of the user currently logged into the Steam client.
 */
std::string steam_get_persona_name();

/**
 * @brief This function can be used to retrieve the user name (screen name) for any specific user ID. 
 *        This is an asynchronous function that will return an asynchronous id and trigger the Steam 
 *        Async Event when the task is finished.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_persona_name 
 *      for more information.
 * 
 * @todo Trigger the Steam Async Event when the task is finished.
 * 
 * @param steamID The unique Steam user id of the user to get the name for.
 * @return std::string The user name (screen name) for the given user ID.
 */
std::string steam_get_user_persona_name(const unsigned long long steamID);

/**
 * @brief This function will return true if the Steam client currently has a live connection to the 
 *        Steam servers. If it returns false, it means there is no active connection due to either a 
 *        networking issue on the local machine, or the Steam server being down or busy.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_is_user_logged_on 
 *      for more information.
 * 
 * @return true If the Steam client currently has a live connection to the Steam servers.
 * @return false If there is no active connection due to either a networking issue on the local
 */
bool steam_is_user_logged_on();

/**
 * @brief This function is used retrieve the current language that Steam is using (as a string), 
 *        for example "english".
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_current_game_language 
 *      for more information.
 * 
 * @return std::string 
 */
std::string steam_current_game_language();

/**
 * @brief This function can be used to retrieve a list of all available languages for Steam. The 
 *        returned value is simply a comma-separated list of languages.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_available_languages 
 *      for more information.
 * 
 * @return std::string 
 */
std::string steam_available_languages();

/**
 * @brief This function checks if the active user is subscribed to the current App ID. This will 
 *        always return true if you're using Steam DRM or calling 
 *        @c GCMain::restart_app_if_necessary() function.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_is_subscribed 
 *      for more information.
 * 
 * @return true If the active user is subscribed to the current App ID.
 * @return false If the active user is not subscribed to the current App ID.
 */
bool steam_is_subscribed();

/**
 * @brief This function sets a warning message hook to receive SteamAPI warnings and info messages in 
 *        the console. The console can be enables by closing Steam client and run ``./steam.exe -console``
 *        on Windows and ``./steam -console`` on Linux from terminal.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_set_warning_message_hook 
 *      for more information.
 * 
 * @todo This function isn't tested yet.
 * 
 */
void steam_set_warning_message_hook();

}  // namespace enigma_user

#endif  // GENERAL_H
