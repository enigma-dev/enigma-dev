#include "management.h"

namespace enigma_user {

void steam_init() {
  steamworks::steam_client::init( k_uAppIdInvalid );  // Replace "k_uAppIdInvalid" with your app id
}

void steam_update() { SteamAPI_RunCallbacks(); }

void steam_shutdown() { steamworks::steam_client::shutdown(); }

}  // namespace enigma_user
