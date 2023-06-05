#include "steam_client.h"

namespace steamworks {

bool steam_client::initialised_{false};  // TODO: Why this line is nessesary?

void steam_client::init(unsigned int appid) {
  if (steam_client::initialised_) {
    DEBUG_MESSAGE("Calling steam_client::init but is already initialized", M_ERROR);
    return;
  }

  if (!steam_main::init()) {
    DEBUG_MESSAGE(
        "SteamApi_Init returned false. Steam isn't running, couldn't find Steam, App ID is ureleased, Don't own App "
        "ID.",
        M_ERROR);
    return;
  }

  steam_client::initialised_ = true;
}

void steam_client::shutdown() {
  if (!steam_client::is_valid()) {
    DEBUG_MESSAGE("Calling steam_client::shutdown but not initialized, consider calling steam_client::init first",
                  M_ERROR);
    return;
  }

  steam_client::initialised_ = false;

  steam_main::shutdown();
}

bool steam_client::is_valid() { return steam_client::initialised_; }

bool steam_client::is_logged_on() { return SteamUser()->BLoggedOn(); }

CSteamID steam_client::steam_id() { return SteamUser()->GetSteamID(); }

const char* steam_client::name() { return SteamFriends()->GetPersonaName(); }

EPersonaState steam_client::state() { return SteamFriends()->GetPersonaState(); }

bool steam_client::restart_app_if_necessary(unsigned int appid) { return steam_main::restart_app_if_necessary(appid); }

}  // namespace steamworks
