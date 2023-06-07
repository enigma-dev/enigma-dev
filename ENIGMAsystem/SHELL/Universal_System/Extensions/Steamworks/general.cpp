#include "general.h"

namespace enigma_user {

bool steam_initialised() { return steamworks::steam_client::is_valid(); }

bool steam_stats_ready() {
    return false;
}

int steam_get_app_id(){
    return -1;
}

uint32 steam_get_user_account_id(){
    return steamworks::steam_client::steam_id().GetAccountID();
}

uint64 steam_get_user_steam_id(){
    return steamworks::steam_client::steam_id().ConvertToUint64();
}

std::string steam_get_persona_name(){
    return steamworks::steam_client::name();
}

std::string steam_get_user_persona_name(CSteamID user_persona_name) {
    return steamworks::steam_client::user_name(user_persona_name);
}

bool steam_is_user_logged_on() {
    return steamworks::steam_client::is_logged_on();
}

std::string steam_current_game_language() {
    return "";
}

std::string steam_available_languages() {
    return "";
}

bool steam_is_subscribed() {
    return false;
}

void steam_set_warning_message_hook() {
    steamworks::steam_client::enable_warning_message_hook();
}

}  // namespace enigma_user
