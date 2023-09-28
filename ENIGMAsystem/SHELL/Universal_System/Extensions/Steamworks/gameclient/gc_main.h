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

// TODO: This documentation need to be improved when uploading a game to Steam Store.
// TODO: Move the pre-checks here.

namespace steamworks_gc {

class GameClient;

class GCMain {
 public:
  /**
  * @brief 
  * 
  * @return true 
  * @return false 
  */
  inline static bool invoke_binder() { return steamworks_b::SteamBinder::bind(); }

  /*
    Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't. init() will fail 
    if you are running your game from the executable or debugger directly and don't have steam_appid.txt in your game 
    directory so make sure to remove the steam_appid.txt file when uploading the game to your Steam depot!. Calling this 
    function optional but highly recommended as the Steam context associated with your application (including your App ID) 
    will not be set up if the user launches the executable directly. If this occurs then SteamAPI_Init will fail and you 
    will be unable to use the Steamworks API. If you choose to use this then it should be the first Steamworks function 
    call you make, right before SteamAPI_Init. Calls SteamAPI_RestartAppIfNecessary(). 
    Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RestartAppIfNecessary for more information.
  */
  /**
   * @brief 
   * 
   * @return false when success
   * @return true when fails
   */
  inline static bool restart_app_if_necessary() {
    return steamworks_b::SteamBinder::RestartAppIfNecessary(
        k_uAppIdInvalid);  // replace k_uAppIdInvalid with your AppID
  }

  /*
    This function performs the following steps:
      1.  Calls gc_main::restart_app_if_necessary().
      
      2.  Calls SteamAPI_Init(). Initializes the Steamworks API. A Steam client must be running in order for init() to pass.
          If you are running If you're running your application from the executable or debugger directly then you must have 
          a steam_appid.txt in your game directory next to the executable, with your app ID in it and nothing else.
          Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Init for more information.
  
      3.  Calls gameclient::is_user_logged_on(). Checks if the user has logged into Steam. We can't proceed with anything 
          if the user is not logged in right?!
  
      4.  Instantiates the Game Client. This includes instantiating all the other clients.  
  
      5.  Sets is_initialised_ to true if all the above steps succeed. This variable can be accessed by calling is_initialised().
  */
  // TODO: Maybe no need to call gameclient::is_user_logged_on() as advised by Steamworks here: https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn
  // TODO: The path here need to be inside an env variable called `STEAM_SDK_PATH`.
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

  /*
    This function performs the following steps:
      1.  Sets is_initialised_ to false.

      2.  Freeing up memory by deleting the Game Client and all of its object pointers.

      3.  Calls SteamAPI_Shutdown(). Shuts down the Steamworks API, releases pointers and frees memory. You should call this 
          during process shutdown if possible. This will not unhook the Steam Overlay from your game as there's no guarantee 
          that your rendering API is done using it.
          Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Shutdown for more information.
  */
  inline static void shutdown() {
    GCMain::is_initialised_ = false;

    if (steamworks_b::SteamBinder::Shutdown != nullptr)
      steamworks_b::SteamBinder::Shutdown();
    else
      DEBUG_MESSAGE("GCMain::shutdown() failed due to loading error.", M_ERROR);

    if (nullptr != GCMain::gameclient_) delete GCMain::gameclient_;
  }

  /*
    This function returns true if Steamworks API is initialized successfully. Otherwise, it returns false.
  */
  inline static bool is_initialised() { return GCMain::is_initialised_; }

  /*
    This function calls SteamAPI_RunCallbacks(). Dispatches callbacks and call results to all of the registered listeners. As
    ENIGMA is single-threaded, no need to call SteamAPI_ReleaseCurrentThreadMemory().
    Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RunCallbacks for more information.
    [OPTIONAL] Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_ReleaseCurrentThreadMemory for more information.
  */
  inline static void run_callbacks() { steamworks_b::SteamBinder::RunCallbacks(); }

  /*
    This function calls SteamUtils()->SetWarningMessageHook(&SteamAPIDebugTextHook). Sets a warning message hook to receive 
    SteamAPI warnings and info messages in a callback function. Passing nullptr will unhook.
    Check https://partner.steamgames.com/doc/api/ISteamUtils#SetWarningMessageHook for more information.
  */
  // TODO: Maybe need to be refactored to help game developers unhook the warning message hook.
  static void set_warning_message_hook();

  /*
    This function returns a pointer to the Game Client. This pointer can be used to access the Game Client's object pointers.
  */
  static GameClient* get_gameclient() { return GCMain::gameclient_; }

  inline static bool dynamic_path_exists() { return GCMain::dynamic_path_exists_; }

 private:
  /**
  * @brief A hnadle to a dynamic loading library.
  * 
  */
  inline static void* dynamic_handle_{nullptr};

  /**
   * @brief 
   * 
   */
  inline static bool dynamic_path_exists_{false};

  /*
    This variable is used to store a pointer to the Game Client.
  */
  inline static GameClient* gameclient_{nullptr};

  /*
    This variable is used to store the Steamworks API initialization status. It is set to true if the Steamworks API is
    initialized successfully. Otherwise, it is set to false. Can be accessed by calling is_initialised().
  */
  inline static bool is_initialised_{false};
};
}  // namespace steamworks_gc

#endif  // GC_MAIN_H
