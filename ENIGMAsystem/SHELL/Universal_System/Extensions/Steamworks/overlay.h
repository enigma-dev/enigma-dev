#ifndef OVERLAY_H
#define OVERLAY_H

#include <string>

namespace enigma_user {

extern const unsigned ov_friends;
extern const unsigned ov_community;
extern const unsigned ov_players;
extern const unsigned ov_settings;
extern const unsigned ov_gamegroup;
extern const unsigned ov_stats;
extern const unsigned ov_achievements;
extern const unsigned ov_other;

extern const unsigned steam_overlay_notification_position_top_left;
extern const unsigned steam_overlay_notification_position_top_right;
extern const unsigned steam_overlay_notification_position_bottom_left;
extern const unsigned steam_overlay_notification_position_bottom_right;

extern const unsigned user_ov_steamid;
extern const unsigned user_ov_chat;
extern const unsigned user_ov_jointrade;
extern const unsigned user_ov_stats;
extern const unsigned user_ov_achievements;
extern const unsigned user_ov_friendadd;
extern const unsigned user_ov_friendremove;
extern const unsigned user_ov_friendrequestaccept;
extern const unsigned user_ov_friendrequestignore;

bool steam_is_overlay_enabled();

bool steam_is_overlay_activated();

void steam_activate_overlay(const int overlay_type);

void steam_activate_overlay_browser(const std::string& url);

void steam_activate_overlay_store(const int app_id);

void steam_activate_overlay_user(const unsigned dialog, const unsigned long long steamid);

void steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset);

void steam_set_overlay_notification_position(const int position);

}  // namespace enigma_user

#endif  // OVERLAY_H
