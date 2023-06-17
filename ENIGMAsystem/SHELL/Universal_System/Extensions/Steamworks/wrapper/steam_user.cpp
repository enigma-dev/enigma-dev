#include "steam_user.h"
#include "steam_client.h"

namespace steamworks {

CSteamID steam_user::get_steam_id() {
  return SteamUser()->GetSteamID();
}

}  // namespace steamworks
