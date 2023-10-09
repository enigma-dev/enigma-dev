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

#ifndef GC_MAIN_H
#define GC_MAIN_H
#pragma once

#include "gameclient.h"

/**
 * @todo Move the pre-checks here.
 * 
 */

namespace steamworks_gc {

class GameClient;

class GCMain {
 public:
  /**
  * @brief This function performs the most important step in the Steamworks extension. It loads the Steamworks API 
  *        dynamically depending on which SDK is available on the user's machine. This approach is taken in order 
  *        for the CI to pass.
  * 
  * @note This must be called before initializing the Steamworks API.
  * 
  * @return true 
  * @return false 
  */
  inline static bool invoke_binder() { return steamworks_b::SteamBinder::bind(); }

  /*
     Calls SteamAPI_RestartAppIfNecessary(). 
    
  */
  /**
   * @brief Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't. 
   *        @c GCMain::init() will fail if you are running your game from the executable or debugger directly 
   *        and don't have steam_appid.txt in your game directory so make sure to remove the steam_appid.txt 
   *        file when uploading the game to your Steam depot!. Calling this function optional but highly 
   *        recommended as the Steam context associated with your application (including your App ID) will not 
   *        be set up if the user launches the executable directly. If this occurs then @c SteamBinder::Init() will fail 
   *        and you will be unable to use the Steamworks API. If you choose to use this then it should be the first 
   *        Steamworks function call you make, right before @c SteamBinder::Init() function.
   * 
   * @note Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RestartAppIfNecessary for more information.
   * 
   * @return false when success
   * @return true when fails
   */
  inline static bool restart_app_if_necessary() {
    return steamworks_b::SteamBinder::RestartAppIfNecessary(
        k_uAppIdInvalid);  // replace k_uAppIdInvalid with your AppID
  }

  /**
   * @brief This function performs the following steps:
   *         @c 1.  Calls @c GCMain::restart_app_if_necessary() function.
   *         @c 2.  Calls @c SteamBinder::Init() function. Initializes the Steamworks API. A Steam client must 
   *                be running in order for @c GCMain::init() to pass. If you are running If you're running your application 
   *                from the executable or debugger directly then you must have a steam_appid.txt in your game directory 
   *                next to the executable, with your app ID in it and nothing else.
   *         @c 3.  Calls @c GameClient::is_user_logged_on() function. Checks if the user has logged into Steam. We can't proceed 
   *                with anything if the user is not logged in right?!
   *         @c 4.  Instantiates the Game Client. This includes instantiating all the other clients.
   *         @c 5.  Sets @c GCMain::is_initialised_ to true if all the above steps succeed. This variable can be accessed by
   *                calling @c GCMain::is_initialised() function.
   * 
   * @note Maybe no need to call gameclient::is_user_logged_on() as advised by Steamworks here: 
   *       https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn.
   * 
   * @note Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Init for more information.
   * 
   * @return true 
   * @return false 
   */
  inline static bool init() {
    if (GCMain::restart_app_if_necessary()) {
      return false;
    }

    if (!steamworks_b::SteamBinder::Init()) {
      return false;
    }

    if (!GameClient::is_user_logged_on()) {
      return false;
    }

    GCMain::gameclient_ = new GameClient();

    GCMain::is_initialised_ = true;

    return true;
  }

  /**
   * @brief This function performs the following steps:
   *         @c 1. Sets @c GCMain::is_initialised_ to false.
   *         @c 2. Freeing up memory by deleting Game Client.
   *         @c 3. Calls @c SteamBinder::Shutdown() function. Shuts down the Steamworks API, releases pointers and 
   *               frees memory. You should call this during process shutdown if possible. This will not 
   *               unhook the Steam Overlay from your game as there's no guarantee that your rendering API 
   *               is done using it.
   * 
   * @note Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Shutdown for more information.
   * 
   */
  inline static void shutdown() {
    GCMain::is_initialised_ = false;

    steamworks_b::SteamBinder::Shutdown();

    if (nullptr != GCMain::gameclient_) delete GCMain::gameclient_;

    GCMain::gameclient_ = nullptr;
  }

  /**
   * @brief Returns @c GCMain::is_initialised_ function.
   * 
   * @return true if Steamworks API is initialized successfully.
   * @return false if something went wrong while initializing the Steamworks API.
   */
  inline static bool is_initialised() { return GCMain::is_initialised_; }

  /**
   * @brief This function calls @c SteamBinder::RunCallbacks() function. Dispatches callbacks and call results to 
   *        all of the registered listeners. As ENIGMA is single-threaded, no need to call 
   *        @c SteamBinder::ReleaseCurrentThreadMemory() function.
   * 
   * @note Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RunCallbacks for more information.
   * @note [OPTIONAL] Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_ReleaseCurrentThreadMemory 
   *       for more information.
   * 
   */
  inline static void run_callbacks() { steamworks_b::SteamBinder::RunCallbacks(); }

  /**
   * @brief Sets a warning message hook to receive SteamAPI warnings and info messages in a callback function.
   * 
   * @note Passing nullptr will unhook.
   * @note Check https://partner.steamgames.com/doc/api/ISteamUtils#SetWarningMessageHook for more information.
   * 
   * @todo Maybe need to be refactored to help game developers unhook the warning message hook.
   * 
   */
  static void set_warning_message_hook();

  /**
   * @brief This function returns a pointer to the Game Client. This pointer can be used to access 
   *        the Game Client's object pointers.
   * 
   * @return GameClient* 
   */
  static GameClient* get_gameclient() { return GCMain::gameclient_; }

 private:
  /**
   * @brief Pointer to Game Client.
   * 
   * @see @c GCMain::get_gameclient() function.
   * 
   */
  inline static GameClient* gameclient_{nullptr};

  /**
   * @brief Stores the initialization status of the Steamworks API.
   * 
   * @see @c GCMain::is_initialised() function.
   * 
   */
  inline static bool is_initialised_{false};
};
}  // namespace steamworks_gc

#endif  // GC_MAIN_H
