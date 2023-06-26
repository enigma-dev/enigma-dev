#include "management.h"

namespace enigma_user {

void steam_init() {
  if (steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_init but is already initialized", M_ERROR);
    return;
  }

  if (!steamworks::cmain::init()) {
    DEBUG_MESSAGE(
        "SteamApi_Init returned false. Steam isn't running, couldn't find Steam, App ID is ureleased, Don't own App "
        "ID.",
        M_ERROR);
    return;
  }
}

void steam_update() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_update but not initialized, consider calling steam_init first", M_ERROR);
    return;
  }
  SteamAPI_RunCallbacks();
}

void steam_shutdown() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_shutdown but not initialized, consider calling steam_init first", M_ERROR);
    return;
  }
  steamworks::cmain::shutdown();
}

}  // namespace enigma_user
