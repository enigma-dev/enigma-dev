#if !defined(OVERLAY_H)
#define OVERLAY_H

#include <string>

namespace enigma_user {

bool steam_is_overlay_enabled();

bool steam_is_overlay_activated();

void steam_activate_overlay(const int overlay_type);

void steam_activate_overlay_browser(const std::string& url);

void steam_activate_overlay_store(const int app_id);

void steam_activate_overlay_user(const std::string& dialog_name, unsigned long long steamid);

bool steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset);

void steam_set_overlay_notification_position(const int position);

}  // namespace enigma_user

#endif  // !OVERLAY_H
