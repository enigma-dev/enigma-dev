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

/**
 * @brief The Steam Overlay is the visual display that can be brought up to display the 
 *        Steam interface to the user. This is normally done by the user themselves using 
 *        a combination of keys, but you also have the possibility of doing it from within 
 *        your game, so that you can map a button or an event to show the overlay.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#overlay for 
 *      more information.
 * 
 */

#ifndef OVERLAY_H
#define OVERLAY_H

#include "gameclient/gc_main.h"

namespace enigma_user {

/**
 * @brief These constants specify the type of overlay to be activated when using the function 
 *        @c steam_activate_overlay() function.
 * 
 * @see @c steam_activate_overlay() function.
 * 
 */
extern const unsigned ov_friends;
extern const unsigned ov_community;
extern const unsigned ov_players;
extern const unsigned ov_settings;
extern const unsigned ov_gamegroup;
extern const unsigned ov_stats;
extern const unsigned ov_achievements;
extern const unsigned ov_other;

/**
 * @brief These constants specify the dialog to be activated when using the function 
 *        @c steam_activate_overlay_user() function.
 * 
 * @note These constants are not existing in GMS's.
 * 
 * @see @c steam_activate_overlay_user() function.
 * 
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

/**
 * @brief These constants specify the position of the notification overlay 
 *        onscreen an should be used with the function 
 *        @c steam_set_overlay_notification_position() function.
 * 
 * @see @c steam_set_overlay_notification_position() function.
 * 
 */
extern const unsigned steam_overlay_notification_position_invalid;
extern const unsigned steam_overlay_notification_position_top_left;
extern const unsigned steam_overlay_notification_position_top_right;
extern const unsigned steam_overlay_notification_position_bottom_left;
extern const unsigned steam_overlay_notification_position_bottom_right;

/**
 * @brief This function can be called to check that the Steam client API has 
 *        the overlay functionality enabled.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_is_overlay_enabled 
 *      for more information.
 * 
 * @note On Linux, you need to add @c gameoverlayrenderer.so to the @c LD_PRELOAD
 *       in order for this function to return true and the overlay to work. This
 *       explained very well in the @c Steamworks\README.md file.
 * 
 * @see https://partner.steamgames.com/doc/store/application/platforms/linux#FAQ
 *      for more information.
 * 
 * @note On Windows, you don't have to do anything.
 * 
 * @note macOS is not supported yet.
 * 
 * @return true if overlay is accessible.
 * @return false if overlay is not accessible.
 */
bool steam_is_overlay_enabled();

/**
 * @brief This function can be used to find out if the user has the Steam Overlay active or not. If 
 *        the overlay is active and visible to the user the function will return true, and if it is 
 *        not, then it will return false.
 * 
 * @note @c steam_is_overlay_enabled() must return true in order for this function 
 *       to work.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_is_overlay_activated 
 *      for more information.
 * 
 * @return true 
 * @return false 
 */
bool steam_is_overlay_activated();

/**
 * @brief You can use this function to active the overlay programmatically. The user can active the 
 *        overlay by pressing @c SHIFT+TAB on the kayboard anyway.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay 
 *      for more information.
 * 
 * @param overlay_type @see constants above.
 */
void steam_activate_overlay(const int overlay_type);

/**
 * @brief This function will be used to simulate the Steam Overlay deactivation using keyboard 
 *        shortcuts or close button on the top right corner.
 * 
 * @note This function is not part of the Overlay API. It is only used for testing purposes.
 * 
 */
void steam_deactivate_overlay();

/**
 * @brief This function is used to open the Steam game overlay to its web browser and then have it load 
 *        the specified URL. you need to use the full URL as a string for this to resolve correctly, 
 *        for example: &quot;http://www.steampowered.com&quot;.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_browser 
 *      for more information.
 * 
 * @param url This is the url used on the Steam Overlay browser.
 */
void steam_activate_overlay_browser(const std::string& url);

/**
 * @brief This function is used to open the Steam overlay on the store page for a game so that users can 
 *        buy or download DLC (for example). You need to supply the unique App ID for the game or DLC 
 *        which you would get from the Steam dashboard when you set it up.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_store 
 *      for more information.
 * 
 * @param app_id This is the unique App ID for the game or DLC to open the store page for.
 */
void steam_activate_overlay_store(const int app_id);

/**
 * @brief This function will open the Steam overlay to one of the chosen dialogues relating to 
 *        the user ID given.
 * 
 * @note This function is different from GMS's as it takes an int instead of a string for the 
 *       dialog.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_activate_overlay_user 
 *      for more information.
 * 
 * @param dialog @see constants above.
 * @param steamid The unique Steam ID of the user to open the overlay for.
 */
void steam_activate_overlay_user(const unsigned dialog, const unsigned long long steamid);

/**
 * @brief This function sets the inset of the overlay notification from the corner specified by 
 *        @c steam_set_overlay_notification_position() function.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_set_overlay_notification_inset 
 *      for more information.
 * 
 * @param hor_inset The horizontal inset from the corner.
 * @param vert_inset The vertical inset from the corner.
 * @return true If the inset was set correctly.
 * @return false If the inset was not set correctly.
 */
bool steam_set_overlay_notification_inset(const int hor_inset, const int vert_inset);

/**
 * @brief This function changes the corner in which the overlay notifications will appear.
 * 
 * @see https://github.com/YoYoGames/GMEXT-Steamworks/wiki/Overlay#steam_set_overlay_notification_position 
 *      for more information.
 * 
 * @param position @see constants above.
 */
void steam_set_overlay_notification_position(const unsigned position);

}  // namespace enigma_user

#endif  // OVERLAY_H
