/** Copyright (C) 2023-2024 Saif Kandil (k0T0z)
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

/*
    The Steam Overlay is the visual display that can be brought up to display the 
    Steam interface to the user. This is normally done by the user themselves using 
    a combination of keys, but you also have the possibility of doing it from within 
    your game, so that you can map a button or an event to show the overlay.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#overlay for
    more information.
*/

#ifndef OVERLAY_H
#define OVERLAY_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "gameclient/gc_main.h"

namespace enigma_user {

/*
    These constants specify the type of overlay to be activated when using the function 
    steam_activate_overlay().
*/
extern const unsigned ov_friends;
extern const unsigned ov_community;
extern const unsigned ov_players;
extern const unsigned ov_settings;
extern const unsigned ov_gamegroup;
extern const unsigned ov_stats;
extern const unsigned ov_achievements;
extern const unsigned ov_other;

/*
    NOTE:   These constants are not existing in GMS's.

    These constants specify the dialog to be activated when using the function 
    steam_activate_overlay_user().
*/
extern const unsigned user_ov_steamid;
extern const unsigned user_ov_chat;
extern const unsigned user_ov_jointrade;
extern const unsigned user_ov_stats;
extern const unsigned user_ov_achievements;
extern const unsigned user_ov_friendadd;
extern const unsigned user_ov_friendremove;
extern const unsigned user_ov_friendrequestaccept;
extern const unsigned user_ov_friendrequestignore;

/*
    These constants specify the position of the notification overlay onscreen an should be 
    used with the function steam_set_overlay_notification_position().
*/
extern const unsigned steam_overlay_notification_position_invalid;
extern const unsigned steam_overlay_notification_position_top_left;
extern const unsigned steam_overlay_notification_position_top_right;
extern const unsigned steam_overlay_notification_position_bottom_left;
extern const unsigned steam_overlay_notification_position_bottom_right;

/*
    This function can be called to check that the Steam client API has the overlay functionality 
    enabled. Calls gc_overlay::overlay_enabled().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_is_overlay_enabled
    for more information.
*/
bool steam_is_overlay_enabled();

/*
    This function can be used to find out if the user has the Steam Overlay active or not. If 
    the overlay is active and visible to the user the function will return true, and if it is 
    not, then it will return false. Calls gc_overlay::overlay_activated().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_is_overlay_activated
    for more information.
*/
bool steam_is_overlay_activated();

/*
    You can use this function to active the overlay programmatically. The user can active the
    overlay by pressing SHIFT + TAB on the kayboard anyway. Calls gc_overlay::activate_overlay().
    Check above the overlay_type options.
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay 
    for more information.
*/
void steam_activate_overlay(const int overlay_type);

/*
    This function is used to open the Steam game overlay to its web browser and then have it load 
    the specified URL. you need to use the full URL as a string for this to resolve correctly, 
    for example: &quot;http://www.steampowered.com&quot;. Calls gc_overlay::activate_overlay_browser().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_browser
    for more information.
*/
void steam_activate_overlay_browser(const std::string& url);

/*
    This function is used to open the Steam overlay on the store page for a game so that users can 
    buy or download DLC (for example). You need to supply the unique App ID for the game or DLC 
    which you would get from the Steam dashboard when you set it up. Calls 
    gc_overlay::activate_overlay_browser().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_store
    for more information.
*/
void steam_activate_overlay_store(const int app_id);

/*
    NOTE:   This function is different from GMS's as it takes an int instead of a string for the
            dialog.

    This function will open the Steam overlay to one of the chosen dialogues relating to the user ID 
    given. Check above dialog options. Calls gc_overlay::activate_overlay_user().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_user
    for more information.
*/
void steam_activate_overlay_user(const unsigned dialog, const unsigned long long steamid);

/*
    This function sets the inset of the overlay notification from the corner specified by 
    steam_set_overlay_notification_position(). Calls gc_overlay::set_overlay_notification_inset().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_set_overlay_notification_inset
    for more information.
*/
bool steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset);

/*
    This function changes the corner in which the overlay notifications will appear. Check above
    position options. Calls gc_overlay::set_overlay_notification_position().
    Check https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_set_overlay_notification_position
    for more information.
*/
void steam_set_overlay_notification_position(const unsigned position);

}  // namespace enigma_user

#endif  // OVERLAY_H
