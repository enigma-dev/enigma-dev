#include "general.h"

namespace enigma_user {

bool steam_initialised() {
  try {
    Steamworks::SteamClient::init( k_uAppIdInvalid ); // Replace "k_uAppIdInvalid" with your app id
    return true;
  } catch (const std::exception& e) {
    DEBUG_MESSAGE(e.what(), M_ERROR);
  }
  return false;
}

}  // namespace enigma_user
