#include "general.h"

namespace enigma_user {

bool steam_initialised() { return steamworks::steam_client::is_valid(); }

}  // namespace enigma_user
