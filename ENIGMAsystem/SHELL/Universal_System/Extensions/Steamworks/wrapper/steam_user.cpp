#include "steam_user.h"
#include "steam_client.h"

namespace steamworks {

CSteamID steam_user::get_steam_id() {
  if (!steam_client::is_valid()) {
    DEBUG_MESSAGE("Calling steam_user::get_steam_id but not initialized, consider calling steam_client::init first",
                  M_ERROR);
    return CSteamID();
  }

  return SteamUser()->GetSteamID();
}

}  // namespace steamworks
