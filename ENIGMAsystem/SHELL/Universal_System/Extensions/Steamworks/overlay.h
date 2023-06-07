#if !defined(GENERAL_H)
#define GENERAL_H

#include "wrapper/steam_client.h"

namespace enigma_user {

bool steam_is_overlay_enabled();

bool steam_is_overlay_activated();

void steam_activate_overlay(int overlay_type);

void steam_activate_overlay_browser(std::string url);

void steam_activate_overlay_store(int app_id);

void steam_activate_overlay_user(std::string dialog_name, int64 steamid);

bool steam_set_overlay_notification_inset(int hor_inset, int vert_inset);

void steam_set_overlay_notification_position(int position);

}

#endif  // !GENERAL_H
