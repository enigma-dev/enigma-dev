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

#include <dlfcn.h>
#include <filesystem>
// #include <cstdlib>

typedef bool (*RestartAppIfNecessary_t)(AppId_t);
typedef bool (*Init_t)();
typedef void (*Shutdown_t)();
typedef void (*RunCallbacks_t)();

typedef ISteamUser* (*SteamUser_t)();
typedef bool (*BLoggedOn_t)(ISteamUser*);

typedef ISteamUserStats* (*SteamUserStats_t)();

typedef ISteamFriends* (*SteamFriends_t)();
typedef const char* (*GetPersonaName_t)(ISteamFriends*);

typedef ISteamApps* (*SteamApps_t)();

typedef ISteamUtils* (*SteamUtils_t)();

namespace fs = std::filesystem;

// TODO: This documentation need to be improved when uploading a game to Steam Store.
// TODO: Move the pre-checks here.

namespace steamworks_gc {

class GameClient;

class GCMain {
 public:
  /**
  * @brief 
  * 
  * @note Check sdk/public/steam/steam_api_flat.h for more information about namings.
  * 
  * @return true 
  * @return false 
  */
  inline static bool load_library_if_exists() {
    // const char* steam_sdk_path = std::getenv("STEAM_SDK_PATH");

    fs::path dynamic_path_{
        "/home/saif/Desktop/enigma-dev/ENIGMAsystem/SHELL/Universal_System/Extensions/Steamworks/gameclient/Steamv157/"
        "sdk/redistributable_bin/linux64/libsteam_api.so"};

    bool path_exists{fs::exists(dynamic_path_)};

    if (!path_exists) {
      DEBUG_MESSAGE("Steamworks SDK not found.", M_WARNING);
      return false;
    }

    void* handle{dlopen(dynamic_path_.c_str(), RTLD_LAZY)};

    if (nullptr == handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::handle_ = handle;

    SteamUser_t steam_user_handle = reinterpret_cast<SteamUser_t>(dlsym(GCMain::handle_, "SteamAPI_SteamUser_v023"));
    if (nullptr == steam_user_handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::steam_user_ = steam_user_handle();

    SteamFriends_t steam_friends_handle =
        reinterpret_cast<SteamFriends_t>(dlsym(GCMain::handle_, "SteamAPI_SteamFriends_v017"));
    if (nullptr == steam_friends_handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::steam_friends_ = steam_friends_handle();

    SteamApps_t steam_apps_handle = reinterpret_cast<SteamApps_t>(dlsym(GCMain::handle_, "SteamAPI_SteamApps_v008"));
    if (nullptr == steam_apps_handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::steam_apps_ = steam_apps_handle();

    SteamUserStats_t steam_user_stats_handle =
        reinterpret_cast<SteamUserStats_t>(dlsym(GCMain::handle_, "SteamAPI_SteamUserStats_v012"));
    if (nullptr == steam_user_stats_handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::steam_user_stats_ = steam_user_stats_handle();

    SteamUtils_t steam_utils_handle =
        reinterpret_cast<SteamUtils_t>(dlsym(GCMain::handle_, "SteamAPI_SteamUtils_v010"));
    if (nullptr == steam_utils_handle) {
      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return false;
    }

    GCMain::steam_utils_ = steam_utils_handle();

    return true;
  }

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
    if (GCMain::handle_valid()) {
      RestartAppIfNecessary_t f =
          reinterpret_cast<RestartAppIfNecessary_t>(dlsym(GCMain::handle_, "SteamAPI_RestartAppIfNecessary"));
      if (f != nullptr) return f(k_uAppIdInvalid);  // replace k_uAppIdInvalid with your AppID

      DEBUG_MESSAGE(dlerror(), M_ERROR);
      return true;
    }

    return SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid);
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

    if (GCMain::handle_valid()) {
      Init_t f = reinterpret_cast<Init_t>(dlsym(GCMain::handle_, "SteamAPI_Init"));
      if (f != nullptr) {
        if (!f()) {
          return false;
        }
      } else {
        DEBUG_MESSAGE(dlerror(), M_ERROR);
        return false;
      }
    } else {
      if (!SteamAPI_Init()) {
        return false;
      }
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

    if (GCMain::handle_valid()) {
      Shutdown_t f = reinterpret_cast<Shutdown_t>(dlsym(GCMain::handle_, "SteamAPI_Shutdown"));
      if (f != nullptr) {
        f();
      } else
        DEBUG_MESSAGE(dlerror(), M_ERROR);
    } else {
      SteamAPI_Shutdown();
    }

    if (nullptr != GCMain::gameclient_) delete GCMain::gameclient_;

    dlclose(GCMain::handle_);
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
  inline static void run_callbacks() {
    if (GCMain::handle_valid()) {
      RunCallbacks_t f = reinterpret_cast<RunCallbacks_t>(dlsym(GCMain::handle_, "SteamAPI_RunCallbacks"));
      if (f != nullptr) {
        f();
      } else
        DEBUG_MESSAGE(dlerror(), M_ERROR);
      return;
    }

    SteamAPI_RunCallbacks();
  }

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

  inline static void* get_handle() { return GCMain::handle_; }

  inline static bool handle_valid() { return GCMain::handle_ != nullptr; }

  inline static ISteamUser* get_steam_user() { return GCMain::steam_user_; }
  inline static bool steam_user_valid() { return GCMain::steam_user_ != nullptr; }

  inline static ISteamFriends* get_steam_friends() { return GCMain::steam_friends_; }
  inline static bool steam_friends_valid() { return GCMain::steam_friends_ != nullptr; }

  inline static ISteamUserStats* get_steam_user_stats() { return GCMain::steam_user_stats_; }
  inline static bool steam_user_stats_valid() { return GCMain::steam_user_stats_ != nullptr; }

  inline static ISteamApps* get_steam_apps() { return GCMain::steam_apps_; }
  inline static bool steam_apps_valid() { return GCMain::steam_apps_ != nullptr; }

  inline static ISteamUtils* get_steam_utils() { return GCMain::steam_utils_; }
  inline static bool steam_utils_valid() { return GCMain::steam_utils_ != nullptr; }

 private:
  /**
  * @brief A hnadle to a dynamic loading library.
  * 
  */
  inline static void* handle_{nullptr};

  inline static ISteamUser* steam_user_{nullptr};
  inline static ISteamFriends* steam_friends_{nullptr};
  inline static ISteamUserStats* steam_user_stats_{nullptr};
  inline static ISteamApps* steam_apps_{nullptr};
  inline static ISteamUtils* steam_utils_{nullptr};

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
