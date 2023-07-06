#include "management.h"

bool managements_pre_checks(const std::string& script_name) {
  if (!steamworks::c_main::is_initialised()) {
    DEBUG_MESSAGE("Calling " + script_name + " failed. Make sure that the API is initialized correctly.", M_ERROR);
    return false;
  }

  return true;
}

namespace enigma_user {

void steam_init() { enigma::extension_steamworks_init(); }

void steam_update() {
  if (!managements_pre_checks("steam_update")) return;

  steamworks::c_main::run_callbacks();
}

void steam_shutdown() {
  if (!managements_pre_checks("steam_shutdown")) return;

  steamworks::c_main::shutdown();
}

}  // namespace enigma_user
