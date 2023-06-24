#include "general.h"

#include "game_client/game_client.h"

namespace enigma_user {

extern "C" void __cdecl SteamAPIDebugTextHook(int nSeverity, const char* pchDebugText) {
  DEBUG_MESSAGE(pchDebugText, M_INFO);

  if (nSeverity >= 1) {
    int x = 3;
    (void)x;
  }
}

bool steam_initialised() { return steamworks::cmain::is_initialised(); }

// Not implemented
bool steam_stats_ready() { return false; }

unsigned steam_get_app_id() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_app_id but not initialized, consider calling steam_init first", M_ERROR);
    return -1;
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_app_id but not logged in, please log into Steam first", M_ERROR);
    return -1;
  }

  return steamworks::cmain::get_game_client()->get_steam_app_id();
}

unsigned steam_get_user_account_id() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_user_account_id but not initialized, consider calling steam_init first", M_ERROR);
    return -1;
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_user_account_id but not logged in, please log into Steam first", M_ERROR);
    return -1;
  }

  return steamworks::cmain::get_game_client()->get_steam_id_local_user().GetAccountID();
}

unsigned long long steam_get_user_steam_id() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_user_steam_id but not initialized, consider calling steam_init first", M_ERROR);
    return -1;
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_user_steam_id but not logged in, please log into Steam first", M_ERROR);
    return -1;
  }

  return steamworks::cmain::get_game_client()->get_steam_id_local_user().ConvertToUint64();
}

std::string steam_get_persona_name() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_persona_name but not initialized, consider calling steam_init first", M_ERROR);
    return std::to_string(-1);
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_persona_name but not logged in, please log into Steam first", M_ERROR);
    return std::to_string(-1);
  }

  return steamworks::cmain::get_game_client()->get_steam_persona_name();
}

std::string steam_get_user_persona_name(CSteamID user_persona_name) {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_get_user_persona_name but not initialized, consider calling steam_init first",
                  M_ERROR);
    return std::to_string(-1);
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_get_user_persona_name but not logged in, please log into Steam first", M_ERROR);
    return std::to_string(-1);
  }

  return steamworks::cgame_client::get_steam_user_persona_name(user_persona_name);
}

bool steam_is_user_logged_on() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_user_logged_on but not initialized, consider calling steam_init first", M_ERROR);
    return false;
  }

  return steamworks::cmain::get_game_client()->is_user_logged_on();
}

std::string steam_current_game_language() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_current_game_language but not initialized, consider calling steam_init first",
                  M_ERROR);
    return std::to_string(-1);
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_current_game_language but not logged in, please log into Steam first", M_ERROR);
    return std::to_string(-1);
  }

  return steamworks::cmain::get_game_client()->get_current_game_language();
}

std::string steam_available_languages() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_available_languages but not initialized, consider calling steam_init first", M_ERROR);
    return std::to_string(-1);
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_available_languages but not logged in, please log into Steam first", M_ERROR);
    return std::to_string(-1);
  }

  return steamworks::cmain::get_game_client()->get_available_languages();
}

bool steam_is_subscribed() {
  if (!steamworks::cmain::is_initialised()) {
    DEBUG_MESSAGE("Calling steam_is_subscribed but not initialized, consider calling steam_init first", M_ERROR);
    return false;
  }

  if (!steamworks::cmain::get_game_client()->is_user_logged_on()) {
    DEBUG_MESSAGE("Calling steam_is_subscribed but not logged in, please log into Steam first", M_ERROR);
    return false;
  }

  return steamworks::cgame_client::is_subscribed();
}

void steam_set_warning_message_hook() { SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook); }

}  // namespace enigma_user
