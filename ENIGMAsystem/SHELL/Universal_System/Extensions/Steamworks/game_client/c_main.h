#ifndef GAMECLIENT_MAIN_H
#define GAMECLIENT_MAIN_H

#include "game_client.h"

// TODO: This documentation need to be improved when uploading a game to Steam Store.

namespace steamworks {

class c_game_client;

class c_main {
 public:
  /*
    This function performs the following steps:
      1.  Calls SteamAPI_RestartAppIfNecessary(). Checks if your executable was launched through Steam and relaunches
          it through Steam if it wasn't. init() will fail if you are running your game from the executable or 
          debugger directly and don't have steam_appid.txt in your game directory so make sure to remove the steam_appid.txt 
          file when uploading the game to your Steam depot!. Calling this function optional but highly recommended as the 
          Steam context associated with your application (including your App ID) will not be set up if the user launches the 
          executable directly. If this occurs then SteamAPI_Init will fail and you will be unable to use the Steamworks API.
          If you choose to use this then it should be the first Steamworks function call you make, right before SteamAPI_Init.
          Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RestartAppIfNecessary for more information.

      2.  Calls SteamAPI_Init(). Initializes the Steamworks API. A Steam client must be running in order for init() to pass.
          If you are running If you're running your application from the executable or debugger directly then you must have 
          a steam_appid.txt in your game directory next to the executable, with your app ID in it and nothing else.
          Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Init for more information.
  
      3.  Calls c_game_client::is_user_logged_on(). Checks if the user has logged into Steam. We can't proceed with anything 
          if the user is not logged in right?!
  
      4.  Instantiates the Game Client. This includes instantiating all the other clients.  
  
      5.  Sets is_initialised_ to true if all the above steps succeed. This variable can be accessed by calling is_initialised().
  */
  // TODO: Maybe no need to call c_game_client::is_user_logged_on() as advised by Steamworks here: https://partner.steamgames.com/doc/api/ISteamUser#BLoggedOn
  static bool init();

  /*
    This function performs the following steps:
      1.  Sets is_initialised_ to false.

      2.  Freeing up memory by deleting the Game Client and all of its object pointers.

      3.  Calls SteamAPI_Shutdown(). Shuts down the Steamworks API, releases pointers and frees memory. You should call this 
          during process shutdown if possible. This will not unhook the Steam Overlay from your game as there's no guarantee 
          that your rendering API is done using it.
          Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_Shutdown for more information.
  */
  static void shutdown();

  /*
    This function returns true if Steamworks API is initialized successfully. Otherwise, it returns false.
  */
  static bool is_initialised();

  /*
    This function returns a pointer to the Game Client. This pointer can be used to access the Game Client's object pointers.
  */
  static c_game_client* get_c_game_client();

  /*
    This function calls SteamAPI_RunCallbacks(). Dispatches callbacks and call results to all of the registered listeners. As
    ENIGMA is single-threaded, no need to call SteamAPI_ReleaseCurrentThreadMemory().
    Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_RunCallbacks for more information.
    [OPTIONAL] Check https://partner.steamgames.com/doc/api/steam_api#SteamAPI_ReleaseCurrentThreadMemory for more information.
  */
  static void run_callbacks();

  /*
    This function calls SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook). Sets a warning message hook to receive 
    SteamAPI warnings and info messages in a callback function. Passing NULL will unhook.
    Check https://partner.steamgames.com/doc/api/ISteamClient#SetWarningMessageHook for more information.
  */
  // TODO: Maybe need to be refactored to help game developers unhook the warning message hook.
  static void set_warning_message_hook();

 private:
  /*
    This variable is used to store a pointer to the Game Client.
  */
  static c_game_client* c_game_client_;

  /*
    This variable is used to store the Steamworks API initialization status. It is set to true if the Steamworks API is
    initialized successfully. Otherwise, it is set to false. Can be accessed by calling is_initialised().
  */
  static bool is_initialised_;
};
}  // namespace steamworks

#endif  // GAMECLIENT_MAIN_H
