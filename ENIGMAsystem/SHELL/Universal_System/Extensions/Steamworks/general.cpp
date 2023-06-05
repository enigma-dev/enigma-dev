#include "general.h"

namespace enigma_user {

bool steam_initialised() {
  return steamworks::steam_client::init(k_uAppIdInvalid);  // Replace "k_uAppIdInvalid" with your app id
}

}  // namespace enigma_user
