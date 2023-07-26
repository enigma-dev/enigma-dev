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
    The following set of functions are all for checking the availability of certain 
    aspects of the Steam client or server API. This means that these functions should 
    be used before any other Steam API function call to ensure that the client/server 
    setup is correct and communicating with your game.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#general for
    more information.
*/

#ifndef GENERAL_H
#define GENERAL_H

#include "game_client/c_main.h"

namespace enigma_user {

/*
    This function can be called to check that the Steam client API has been initialised 
    correctly before any doing any further calls to Steam specific functions in your game.
    Calls c_main::is_initialised().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_initialised
    for more information.
*/
bool steam_initialised();

/*
    This function can be called to check that the Steam client API has correctly initialised 
    the statistics for your game. Calls c_stats_and_achievements::stats_valid().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_stats_ready
    for more information.
*/
bool steam_stats_ready();

/*
    This function is used retrieve the unique app ID that Steam assigns for your game.
    Calls c_game_client::get_steam_app_id().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_app_id
    for more information.
*/
unsigned steam_get_app_id();

/*
    This function is used retrieve the unique User ID that Steam assigns to each user.
    Calls c_game_client::get_c_steam_id_local_user().GetAccountID().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_account_id
    for more information.
*/
unsigned steam_get_user_account_id();

/*
    This function is used to return the unique Steam user id of the user currently logged into 
    the Steam client. Calls c_game_client::get_c_steam_id_local_user().ConvertToUint64().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_steam_id
    for more information.
*/
unsigned long long steam_get_user_steam_id();

/*
    this function to return the user name of the user currently logged into the Steam client.
    Calls c_game_client::get_steam_persona_name().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_persona_name
    for more information.
*/
std::string steam_get_persona_name();

/*
    This function can be used to retrieve the user name (screen name) for any specific user ID. 
    This is an asynchronous function that will return an asynchronous id and trigger the Steam 
    Async Event when the task is finished. Calls c_game_client::get_steam_user_persona_name().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_get_user_persona_name
    for more information.
*/
// TODO: Trigger the Steam Async Event when the task is finished.
std::string steam_get_user_persona_name(CSteamID user_persona_name);

/*
    This function will return true if the Steam client currently has a live connection to the 
    Steam servers. If it returns false, it means there is no active connection due to either a 
    networking issue on the local machine, or the Steam server being down or busy. Calls 
    c_game_client::is_user_logged_on().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_is_user_logged_on
    for more information.
*/
bool steam_is_user_logged_on();

/*
    This function is used retrieve the current language that Steam is using (as a string), 
    for example "english". Calls c_game_client::get_current_game_language().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_current_game_language
    for more information.
*/
std::string steam_current_game_language();

/*
    This function can be used to retrieve a list of all available languages for Steam. The 
    returned value is simply a comma-separated list of languages. Calls
    c_game_client::get_available_languages().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_available_languages
    for more information.
*/
std::string steam_available_languages();

/*
    This function checks if the active user is subscribed to the current App ID. This will 
    always return true if you're using Steam DRM or calling c_main::restart_app_if_necessary().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_is_subscribed
    for more information.
*/
bool steam_is_subscribed();

/*  
    This function sets a warning message hook to receive SteamAPI warnings and info messages in 
    the console. The console can be enables by closing Steam client and run ``./steam.exe -console``
    on Windows and ``./steam -console`` on Linux from terminal. Calls c_main::set_warning_message_hook().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/General#steam_set_warning_message_hook
    for more information.
*/
void steam_set_warning_message_hook();

}  // namespace enigma_user

#endif  // GENERAL_H
