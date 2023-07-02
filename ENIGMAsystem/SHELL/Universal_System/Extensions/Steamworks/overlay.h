#ifndef OVERLAY_H
#define OVERLAY_H

#include <string>

namespace enigma_user {

const unsigned ov_friends = 0;
const unsigned ov_community = 1;
const unsigned ov_players = 2;
const unsigned ov_settings = 3;
const unsigned ov_gamegroup = 4;
const unsigned ov_achievements = 5;

const unsigned steam_overlay_notification_position_top_left = 6;
const unsigned steam_overlay_notification_position_top_right = 7;
const unsigned steam_overlay_notification_position_bottom_left = 8;
const unsigned steam_overlay_notification_position_bottom_right = 9;

bool steam_is_overlay_enabled();

bool steam_is_overlay_activated();

void steam_activate_overlay(const int overlay_type);

void steam_activate_overlay_browser(const std::string& url);

void steam_activate_overlay_store(const int app_id);

void steam_activate_overlay_user(const std::string& dialog_name, unsigned long long steamid);

bool steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset);

void steam_set_overlay_notification_position(const int position);

}  // namespace enigma_user

#endif  // OVERLAY_H
