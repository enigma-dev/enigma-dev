#include "management.h"

namespace enigma_user {

void steam_init() { enigma::extension_steamworks_init(); }

void steam_update() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_update failed. Make sure that the API is initialized correctly.", M_ERROR);
    return;
  }

  steamworks::c_main::run_callbacks();
}

void steam_shutdown() {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_shutdown failed. Make sure that the API is initialized correctly.", M_ERROR);
    return;
  }

  steamworks::c_main::shutdown();
}

}  // namespace enigma_user
