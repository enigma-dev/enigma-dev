#if !defined(STEAM_CLIENT_H)
#define STEAM_CLIENT_H

#include "Widget_Systems/widgets_mandatory.h"
#include "wrapper/classes/steam_main.h"

#include <stdexcept>
// #include <exception>
#include <string>

namespace steamworks {

class steam_client {
 private:
  static bool initialised_;

 public:

  /// <summary>
  /// Initialize the steam client.
  /// If <paramref name="asyncCallbacks"/> is false you need to call <see cref="RunCallbacks"/> manually every frame.
  /// </summary>
  static void init(unsigned int appid);

  /// <summary>
  /// Shuts down the steam client.
  /// </summary>
  static void shutdown();

  /// <summary>
  /// Check if Steam is loaded and accessible.
  /// </summary>
  static bool is_valid();

  /// <summary>
  /// Checks if the current user's Steam client is connected to the Steam servers.
  /// <para>
  /// If it's not, no real-time services provided by the Steamworks API will be enabled. The Steam 
  /// client will automatically be trying to recreate the connection as often as possible. When the 
  /// connection is restored a SteamServersConnected_t callback will be posted.
  /// You usually don't need to check for this yourself. All of the API calls that rely on this will 
  /// check internally. Forcefully disabling stuff when the player loses access is usually not a 
  /// very good experience for the player and you could be preventing them from accessing APIs that do not 
  /// need a live connection to Steam.
  /// </para>
  /// </summary>
  static bool is_logged_on();

  /// <summary>
  /// Gets the Steam ID of the account currently logged into the Steam client. This is 
  /// commonly called the 'current user', or 'local user'.
  /// A Steam ID is a unique identifier for a Steam accounts, Steam groups, Lobbies and Chat 
  /// rooms, and used to differentiate users in all parts of the Steamworks API.
  /// </summary>
  static CSteamID steam_id();

  // <summary>
	/// returns the local players name - guaranteed to not be <see langword="null"/>.
	/// This is the same name as on the user's community profile page.
	/// </summary>
	static const char* name();

  /// <summary>
	/// Gets the status of the current user.
	/// </summary>
	static EPersonaState state();

  /// <summary>
  /// Checks if your executable was launched through Steam and relaunches it through Steam if it wasn't.
  /// <para>
  ///  This returns true then it starts the Steam client if required and launches your game again through it, 
  ///  and you should quit your process as soon as possible. This effectively runs steam://run/AppId so it 
  ///  may not relaunch the exact executable that called it, as it will always relaunch from the version 
  ///  installed in your Steam library folder/
  ///  Note that during development, when not launching via Steam, this might always return true.
  ///  </para>
  /// </summary>
  static bool restart_app_if_necessary( unsigned int appid );
};

}  // namespace steamworks

#endif  // !STEAM_CLIENT_H
