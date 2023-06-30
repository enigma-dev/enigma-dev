#include "management.h"

namespace enigma_user {

void steam_init() {
  enigma::extension_steamworks_init();
}

void steam_update() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_update but not initialized, consider calling steam_init first", M_ERROR);
    return;
  }
  steamworks::cmain::update();
}

void steam_shutdown() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_shutdown but not initialized, consider calling steam_init first", M_ERROR);
    return;
  }
  steamworks::cmain::shutdown();
}

}  // namespace enigma_user
