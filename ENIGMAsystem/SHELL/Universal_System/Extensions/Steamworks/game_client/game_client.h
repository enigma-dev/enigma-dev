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

#ifndef GAMECLIENT_H
#define GAMECLIENT_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include <iostream>
#include <string>

/*
    This include the only special include that game_client layer uses. DON'T include any
    other includes in game_client layer that are outside Steamworks extension.
*/
#include "Widget_Systems/widgets_mandatory.h"

/*
    This is the lowest layer that game_client layer mainly depends on.
*/
#include "steam/steam_api.h"

namespace steamworks {

class c_overlay;
class c_stats_and_achievements;
class c_leaderboards;

class c_game_client {
 public:
  /*
    Game client constructor.
  */
  c_game_client();

  /*
    Game client destructor.
  */
  ~c_game_client();

  /*
    This function inistantiates all the other clients such as Overlay, Stats and 
    Achievements, Leaderboards, etc clients.
  */
  void init();

  /*
    Returns the pointer to the Overlay object.
  */
  c_overlay* get_c_overlay();

  /*
    Returns the pointer to the Stats and Achievements object.
  */
  c_stats_and_achievements* get_c_stats_and_achievements();

  /*
    Returns the pointer to the Leaderboards object.
  */
  c_leaderboards* get_c_leaderboards();

  /*
    Returns the Steam ID of the account currently logged into the Steam client. This 
    is commonly called the 'current user', or 'local user'. the Steam ID of the local 
    user is stored in c_steam_id_local_user_ private variable. When the contructor is 
    called, the Steam ID of the local user is fetched using SteamUser()->GetSteamID().
    check https://partner.steamgames.com/doc/api/ISteamUser#GetSteamID for more 
    information.
  */
  CSteamID get_c_steam_id_local_user();

  /*
    Returns the App ID of the current process. The App ID of the game is stored in 
    steam_app_id_ private variable. When the contructor is called, the Steam App ID 
    of the game is fetched using SteamUtils()->GetAppID().
    check https://partner.steamgames.com/doc/api/ISteamUtils#GetAppID for more 
    information.
  */
  unsigned get_steam_app_id();

  /*
    Returns the current language that the user has set. This falls back to the Steam 
    UI language if the user hasn't explicitly picked a language for the title. The current 
    language is stored in current_game_language_ private variable. When the contructor 
    is called, the current language is fetched using SteamApps()->GetCurrentGameLanguage().
    check https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentGameLanguage for 
    more information.
  */
  std::string get_current_game_language();

  /*
    Returns a comma separated list of the languages the current app supports. the list of 
    languages that the user has set using Steam client. The list of languages is stored in 
    available_game_languages_ private variable. When the contructor is called, the list of 
    languages is fetched using SteamApps()->GetAvailableGameLanguages().
    check https://partner.steamgames.com/doc/api/ISteamApps#GetAvailableGameLanguages for 
    more information.
  */
  std::string get_available_game_languages();

  /*
    Checks if the current user's Steam client is connected to the Steam servers.
    Check https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn for more 
    information.
  */
  static bool is_user_logged_on();

  /*
    Returns the current user's persona (display) name. This is the same name that is 
    displayed the user's community profile page. Calls SteamFriends()->GetPersonaName().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GetPersonaName for more 
    information.
  */
  static std::string get_steam_persona_name();

  /*
    Returns the specified user's persona (display) name. This will only be known to 
    the current user if the other user is in their friends list, on the same game server, 
    in a chat room or lobby, or in a small Steam group with the local user. Calls 
    SteamFriends()->GetFriendPersonaName().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendPersonaName for 
    more information.
  */
  static std::string get_steam_user_persona_name(CSteamID c_steam_id);

  /*
    Checks if the active user is subscribed to the current App ID. This will always 
    return true if you're using Steam DRM or calling c_main::restart_app_if_necessary().
    Check https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribed for more 
    information.
  */
  static bool is_subscribed();

 private:
  /*
    Pointer to the Overlay object.
  */
  c_overlay* c_overlay_;

  /*
    Pointer to the Stats and Achievements object.
  */
  c_stats_and_achievements* c_stats_and_achievements_;

  /*
    Pointer to the Leaderboards object.
  */
  c_leaderboards* c_leaderboards_;

  /*
    Steam ID of the account currently logged into the Steam client. This can be
    accessed using get_c_steam_id_local_user() function.
  */
  CSteamID c_steam_id_local_user_;

  /*
    App ID of the current process. This can be accessed using get_steam_app_id() 
    function.
  */
  unsigned steam_app_id_;

  /*
    Current language that the user has set. This can be accessed using 
    get_current_game_language() function.
  */
  std::string current_game_language_;

  /*
    Comma separated list of the languages the current app supports. This can be 
    accessed using get_available_game_languages() function.
  */
  std::string available_game_languages_;
};
}  // namespace steamworks

#endif  // GAMECLIENT_H
