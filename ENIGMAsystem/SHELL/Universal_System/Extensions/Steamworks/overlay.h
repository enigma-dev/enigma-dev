#if !defined(OVERLAY_H)
#define OVERLAY_H

#include <string>

namespace enigma_user {

bool steam_is_overlay_enabled();

bool steam_is_overlay_activated();

void steam_activate_overlay(int overlay_type);

void steam_activate_overlay_browser(std::string url);

void steam_activate_overlay_store(int app_id);

void steam_activate_overlay_user(std::string dialog_name, unsigned long long steamid);

bool steam_set_overlay_notification_inset(int hor_inset, int vert_inset);

void steam_set_overlay_notification_position(int position);

}  // namespace enigma_user

#endif  // !OVERLAY_H
