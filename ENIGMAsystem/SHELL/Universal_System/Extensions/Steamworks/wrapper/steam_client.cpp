#include "steam_client.h"

namespace steamworks {

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}

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

std::string steam_client::name() { return SteamFriends()->GetPersonaName(); }

std::string steam_client::user_name(CSteamID user_persona_name) {
  return SteamFriends()->GetFriendPersonaName(user_persona_name);
}

EPersonaState steam_client::state() { return SteamFriends()->GetPersonaState(); }

bool steam_client::restart_app_if_necessary(uint32 appid) { return steam_main::restart_app_if_necessary(appid); }

void steam_client::enable_warning_message_hook() { SteamUtils()->SetWarningMessageHook(&SteamAPIDebugTextHook); }

}  // namespace steamworks
