#if !defined(GENERAL_H)
#define GENERAL_H

#include "wrapper/steam_client.h"
#include "wrapper/steam_utils.h"
#include "wrapper/steam_user.h"
#include <iostream>

namespace enigma_user {

bool steam_initialised();

bool steam_stats_ready();

int steam_get_app_id();

int steam_get_user_account_id();

long long steam_get_user_steam_id();

std::string steam_get_persona_name();

std::string steam_get_user_persona_name(CSteamID user_persona_name);

bool steam_is_user_logged_on();

std::string steam_current_game_language();

std::string steam_available_languages();

bool steam_is_subscribed();

void steam_set_warning_message_hook();

}  // namespace enigma_user

#endif  // !GENERAL_H
