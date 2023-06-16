#if !defined(WRAPPER_STEAM_USER_H)
#define WRAPPER_STEAM_USER_H

#include "Widget_Systems/widgets_mandatory.h"
#include "public/steam/steam_api.h"

namespace steamworks {

class steam_user {
 public:
  /// <summary>
  /// Gets the Steam ID of the account currently logged into the Steam client. This is
  /// commonly called the 'current user', or 'local user'.
  /// A Steam ID is a unique identifier for a Steam accounts, Steam groups, Lobbies and Chat
  /// rooms, and used to differentiate users in all parts of the Steamworks API.
  /// </summary>
  static CSteamID get_steam_id();
};

}  // namespace steamworks

#endif  // !WRAPPER_STEAM_USER_H
