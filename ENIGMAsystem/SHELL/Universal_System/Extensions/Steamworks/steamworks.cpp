#include "steamworks.h"

namespace enigma {

void extension_steamworks_init() {
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

}  // namespace enigma
