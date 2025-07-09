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

#ifndef GAMECLIENT_H
#define GAMECLIENT_H

/**
 * @todo This documentation need to be improved when uploading a game to Steam Store. 
 * @todo Move the pre-checks here.
 * 
 */

#include "steambinder/steambinder.h"

#include <algorithm>
#include <string>

#define INVALID_APP_ID 0
#define INVALID_GAME_ID 0
#define INVALID_STEAM_ID 0

namespace steamworks_gc {

class GCOverlay;
class GCStatsAndAchievements;
class GCLeaderboards;
class GCRemoteStorage;

class GameClient {
 public:
  // Gameclient constructor.
  GameClient();

  // Gameclient destructor.
  ~GameClient();

  /**
   * @brief This function initializes the Steamworks Game Client attributes and
   *        inistantiates all the other clients such as Overlay, Stats and
   *        Achievements, Leaderboards, etc clients.
   * 
   */
  void init();

  // Returns the pointer to the Overlay object.
  GCOverlay* get_gc_overlay();

  // Returns the pointer to the Stats and Achievements object.
  GCStatsAndAchievements* get_gc_statsandachievements();

  // Returns the pointer to the Leaderboards object.
  GCLeaderboards* get_gc_leaderboards();

  // Returns the pointer to the Remote Storage object.
  GCRemoteStorage* get_gc_remotestorage();

  /**
   * @brief Gets the CSteamID of the account currently logged into the Steam client.
   * 
   * @note A CSteamID is a unique identifier for an account, and used to differentiate 
   *       users in all parts of the Steamworks API
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamUser#GetSteamID for more 
   *       information.
   * 
   * @return CSteamID, which is initialized in the constructor.
   */
  CSteamID get_steam_id_local_user();

  /**
   * @brief Gets the appID of the current process.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamUtils#GetAppID for more
   *       information.
   * 
   * @return uint32, which is initialized in the constructor.
   */
  uint32 get_steam_app_id();

  /**
   * @brief Gets the current language that the user has set. This falls back to the Steam 
   *        UI language if the user hasn't explicitly picked a language for the title.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamApps#GetCurrentGameLanguage for
   *       more information.
   * 
   * @param buffer which will be filled with the current language.
   */
  void get_current_game_language(std::string& buffer);

  /**
   * @brief Gets a comma separated list of the languages the current app supports. the list of 
   *        languages that the user has set using Steam client.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamApps#GetAvailableGameLanguages for
   *       more information.
   * 
   * @param buffer which will be filled with the list of languages separated by comma.
   */
  void get_available_game_languages(std::string& buffer);

  /**
   * @brief Checks if the current user's Steam client is connected to the Steam servers.
   * 
   * @note The Steam client will automatically be trying to recreate the connection as often 
   *       as possible.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn for more
   *       information.
   * 
   * @return true if the Steam client current has a live connection to the Steam servers.
   * @return false if there is no active connection due to either a networking issue on the 
   *         local machine, or the Steam server is down/busy.
   */
  static bool is_user_logged_on();

  /**
   * @brief Gets the current user's persona (display) name. This is the same name that is 
   *        displayed the user's community profile page.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#GetPersonaName for more
   *       information.
   * 
   * @param buffer which will be filled with the current user's persona name.
   */
  static void get_steam_persona_name(std::string& buffer);

  /**
   * @brief Gets the specified user's persona (display) name.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#GetFriendPersonaName for more
   *       information.
   * 
   * @param buffer which will be filled with the specified user's persona name.
   * @param steam_id which is the ID of the user to get the persona name for.
   */
  static void get_steam_user_persona_name(std::string& buffer, const uint64& steam_id);

  /**
   * @brief Checks if the active user is subscribed to the current App ID. This will always 
   *        return true if you're using Steam DRM or calling @c gc_main::restart_app_if_necessary()
   *        function.
   * 
   * @return true 
   * @return false 
   */
  static bool is_subscribed();

  /**
   * @brief Sets a Rich Presence key/value for the current user that is automatically shared to all 
   *        friends playing the same game. Each user can have up to 20 keys set as defined by 
   *        @c k_cchMaxRichPresenceKeys constant.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#SetRichPresence for more
   *       information.
   * 
   * @param key 
   * @param value 
   * @return true 
   * @return false 
   */
  static bool set_rich_presence(const std::string& key, const std::string& value);

  /**
   * @brief Clears all of the current user's Rich Presence key/values.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#ClearRichPresence for more
   *       information.
   * 
   */
  static void clear_rich_presence();

  /**
   * @brief Marks a target user as 'played with'.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note The current user must be in game with the other player for the association to work.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#SetPlayedWith for more
   *       information.
   * 
   * @param steam_id 
   */
  static void set_played_with(const uint64& steam_id);

  /**
   * @brief Gets the size of a Steam image handle. This must be called before calling 
   *        @c GameClient::get_image_rgba() to create an appropriately sized buffer that 
   *        will be filled with the raw image data.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamUtils#GetImageSize for more
   *       information.
   * 
   * @param image the id of the image to get the size of.
   * @param width the width of the image in pixels.
   * @param height the height of the image in pixels.
   * @return true 
   * @return false 
   */
  static bool get_image_size(const int& image, uint32& width, uint32& height);

  /**
   * @brief Gets the image bytes from an image handle. Prior to calling this you must get the 
   *        size of the image by calling @c GameClient::get_image_size() so that you can create your buffer with 
   *        an appropriate size. You can then allocate your buffer with the width and height 
   *        as: width * height * 4. The image is provided in RGBA format. This call can be somewhat 
   *        expensive as it converts from the compressed type (JPG, PNG, TGA) and provides no internal 
   *        caching of returned buffer, thus it is highly recommended to only call this once per image 
   *        handle and cache the result. This function is only used for Steam Avatars and Achievement 
   *        images and those are not expected to change mid game.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamUtils#GetImageRGBA for more
   *       information.
   * 
   * @code {.cpp}
   *     // You should first check if you have already cached this image using something like a dictionary/map
   *     // with iImage as the key and then return the texture handle associated with it if it exists.
   *     // If it doesn't exist, continue on, and add the texture to the map before returning at the end of the function.
   * 
   *     // If we have to check the size of the image.
   *     uint32 uAvatarWidth, uAvatarHeight;
   *     bool success = SteamUtils()->GetImageSize( iImage, &uAvatarWidth, &uAvatarHeight );
   *     if ( !success ) {
   *         // handle failure
   *     }
   * 
   *     const int uImageSizeInBytes = uAvatarWidth * uAvatarHeight * 4;
   *     uint8 *pAvatarRGBA = new uint8[uImageSizeInBytes];
   *     success = SteamUtils()->GetImageRGBA( iImage, pAvatarRGBA, uImageSizeInBytes );
   *     if ( !success ) {
   *         // handle failure
   *     }
   * 
   *     // Do something to convert the RGBA texture into your internal texture format for displaying.
   *     // And add the texture to the cache
   * 
   *     // Don't forget to free the memory!
   *     delete[] pAvatarRGBA;
   * @endcode
   * 
   * 
   * @see @c GameClient::get_image_size() function.
   * 
   * @param image the id of the image.
   * @param buffer the buffer to fill with the image data.
   * @param buffer_size the size of the buffer.
   * @return true 
   * @return false 
   */
  static bool get_image_rgba(const int& image, uint8* buffer, const int& buffer_size);

  /**
   * @brief Gets a handle to the small (32*32px) avatar for the specified user. You can pass 
   *        in @c GameClient::get_steam_id_local_user().ConvertToUint64() to get the current 
   *        user's avatar.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#GetSmallFriendAvatar 
   *       for more information.
   * 
   * @param steam_id_friend 
   * @return int32 
   */
  static int32 get_small_friend_avatar(const uint64& steam_id_friend);

  /**
   * @brief Gets a handle to the medium (64*64px) avatar for the specified user. You can pass in 
   *        @c GameClient::get_steam_id_local_user().ConvertToUint64() to get the current 
   *        user's avatar.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#GetMediumFriendAvatar for more
   *       information.
   * 
   * @param steam_id_friend 
   * @return int32 
   */
  static int32 get_medium_friend_avatar(const uint64& steam_id_friend);

  /**
   * @brief Gets a handle to the large (128*128px) avatar for the specified user. You can pass in 
   *        @c GameClient::get_steam_id_local_user().ConvertToUint64() to get the current user's avatar.
   * 
   * @note This is a wrapper function that doesn't require an object to be called.
   * 
   * @note check https://partner.steamgames.com/doc/api/ISteamFriends#GetLargeFriendAvatar for more
   *       information.
   * 
   * @param steam_id_friend 
   * @return int32 
   */
  static int32 get_large_friend_avatar(const uint64& steam_id_friend);

 private:
  // Pointer to the overlay client.
  GCOverlay* gc_overlay_;

  // Pointer to the stats and achievements client.
  GCStatsAndAchievements* gc_statsandachievements_;

  // Pointer to the leaderboards client.
  GCLeaderboards* gc_leaderboards_;

  // Pointer to the remote storage client.
  GCRemoteStorage* gc_remotestorage_;

  /**
   * @brief steamID of the account currently logged into the Steam client. This can be accessed using 
   *        @c GameClient::get_steam_id_local_user() function.
   * 
   * @see @c GameClient::get_steam_id_local_user() function.
   * 
   */
  CSteamID steam_id_local_user_;

  /**
   * @brief appID of the current process. This can be accessed using @c GameClient::get_steam_app_id() 
   *        function.
   * 
   * @see @c GameClient::get_steam_app_id() function.
   * 
   */
  uint32 steam_app_id_;

  /**
   * @brief Current language that the user has set. This can be accessed using 
   *        @c GameClient::get_current_game_language() function.
   * 
   * @see @c GameClient::get_current_game_language() function.
   * 
   */
  std::string current_game_language_;

  /**
   * @brief Comma separated list of the languages the current app supports. This can be accessed using 
   *        @c GameClient::get_available_game_languages() function.
   * 
   * @see @c GameClient::get_available_game_languages() function.
   * 
   */
  std::string available_game_languages_;
};

}  // namespace steamworks_gc

/**
 * @brief The official Steamworks API headers uses macros for generating callback and
 *        callresult functions as well as the public accessors. As ENIGMA doesn't link
 *        any library in the Steamworks/Makefile file, we need to change the macro to
 *        to call the functions we loaded using @c dlsym() and because we can't change
 *        anything inside the official headers, we need to redefine the macros here.
 * 
 * @param pCallback 
 * @param iCallback 
 */
void Custom_SteamAPI_RegisterCallback(class CCallbackBase* pCallback, int iCallback);

/**
 * @brief Calls @c SteamAPI_UnregisterCallback() which is loaded and binded by the binder.
 * 
 * @param pCallback 
 */
void Custom_SteamAPI_UnregisterCallback(class CCallbackBase* pCallback);

#define Custom_SteamAPI_RegisterCallback SteamAPI_RegisterCallback

#define Custom_SteamAPI_UnregisterCallback SteamAPI_UnregisterCallback

void Custom_SteamAPI_RegisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall);

void Custom_SteamAPI_UnregisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall);

#define Custom_SteamAPI_RegisterCallResult SteamAPI_RegisterCallResult

#define Custom_SteamAPI_UnregisterCallResult SteamAPI_UnregisterCallResult

#endif  // GAMECLIENT_H
