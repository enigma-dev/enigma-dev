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
// TODO: Move the pre-checks here.
// TODO: Separate the wrapper functions from the main architecture.

/*
    This is the lowest layer that game_client layer mainly depends on.
*/
#include "steam/steam_api.h"

/*
    This include is the only special include that game_client layer uses. DON'T include any
    other includes in game_client layer that are outside Steamworks extension.
*/
#include "Widget_Systems/widgets_mandatory.h"

#include <algorithm>
#include <string>

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
  uint32 get_steam_app_id();

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
  static std::string get_steam_user_persona_name(const uint64 steam_id);

  /*
    Checks if the active user is subscribed to the current App ID. This will always 
    return true if you're using Steam DRM or calling c_main::restart_app_if_necessary().
    Check https://partner.steamgames.com/doc/api/ISteamApps#BIsSubscribed for more 
    information.
  */
  static bool is_subscribed();

  /*
    Sets a Rich Presence key/value for the current user that is automatically shared to 
    all friends playing the same game. Each user can have up to 20 keys set as defined 
    by `k_cchMaxRichPresenceKeys`. Calls SteamFriends()->SetRichPresence().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#SetRichPresence for more 
    information.
  */
  static bool set_rich_presence(const std::string& key, const std::string& value);

  /*
    Clears all of the current user's Rich Presence key/values. Calls 
    SteamFriends()->ClearRichPresence().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#ClearRichPresence for more 
    information.
  */
  static void clear_rich_presence();

  /*
    NOTE: The current user must be in game with the other player for the association to work.

    Marks a target user as 'played with'. Calls SteamFriends()->SetPlayedWith().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#SetPlayedWith for more
    information.
  */
  static void set_played_with(const uint64 steam_id);

  /*
    Gets the size of a Steam image handle. This must be called before calling 
    steamworks::c_game_client::get_image_rgba() to create an appropriately sized buffer 
    that will be filled with the raw image data. Calls SteamUtils()->GetImageSize(). 
    Check https://partner.steamgames.com/doc/api/ISteamUtils#GetImageSize for more
    information.
  */
  static bool get_image_size(const int image, uint32* width, uint32* height);

  /*
    Gets the image bytes from an image handle. Prior to calling this you must get the 
    size of the image by calling GetImageSize so that you can create your buffer with 
    an appropriate size. You can then allocate your buffer with the width and height as: 
    width * height * 4. The image is provided in RGBA format. This call can be somewhat 
    expensive as it converts from the compressed type (JPG, PNG, TGA) and provides no 
    internal caching of returned buffer, thus it is highly recommended to only call this 
    once per image handle and cache the result. This function is only used for Steam Avatars 
    and Achievement images and those are not expected to change mid game. Calls
    SteamUtils()->GetImageRGBA().
    Check https://partner.steamgames.com/doc/api/ISteamUtils#GetImageRGBA for more
    information.
  */
  static bool get_image_rgba(const int image, uint8* buffer, int buffer_size);

  /*
    Gets a handle to the small (32*32px) avatar for the specified user. You can pass in 
    ISteamUser::GetSteamID to get the current user's avatar. Calls SteamFriends()->GetSmallFriendAvatar().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GetSmallFriendAvatar for more
    information.
  */
  static int32 get_small_friend_avatar(const uint64 steam_id_friend);

  /*
    Gets a handle to the medium (64*64px) avatar for the specified user. You can pass in 
    ISteamUser::GetSteamID to get the current user's avatar. Calls SteamFriends()->GetMediumFriendAvatar().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GetMediumFriendAvatar for more
    information.
  */
  static int32 get_medium_friend_avatar(const uint64 steam_id_friend);

  /*
    Gets a handle to the large (128*128px) avatar for the specified user. You can pass in 
    ISteamUser::GetSteamID to get the current user's avatar. Calls SteamFriends()->GetLargeFriendAvatar().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GetLargeFriendAvatar for more
    information.
  */
  static int32 get_large_friend_avatar(const uint64 steam_id_friend);

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
  uint32 steam_app_id_;

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
