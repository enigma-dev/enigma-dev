/** Copyright (C) 2023-2024 Saif Kandil
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
  The Steam overlay is a piece of the Steam user interface that can be activated over the top of 
  almost any game launched through Steam. It lets the user access the friends list, web browser, 
  chat, and in-game DLC purchasing.
  Check https://partner.steamgames.com/doc/features/overlay for more information.
*/

#ifndef GAMECLIENT_OVERLAY_H
#define GAMECLIENT_OVERLAY_H

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_overlay {
 public:
  /*
    Overlay client constructor.
  */
  c_overlay();

  /*
    Overlay client destructor.
  */
  ~c_overlay() = default;

  /*
    Macro for listening to GameOverlayActivated_t callback. Callbacks are dispatched by
    calling c_main::run_callbacks().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GameOverlayActivated_t for
    more information.
  */
  STEAM_CALLBACK(c_overlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  /*
    Checks Overlay's activation status. Returns true if the Overlay is activated. Otherwise, 
    it returns false. Overlay's activation status is stored in overlay_activated_ private
    variable.
  */
  bool overlay_activated();

  /*
    Checks if the Steam Overlay is running & the user can access it. The overlay process 
    could take a few seconds to start & hook the game process, so this function will 
    initially return false while the overlay is loading. Calls SteamUtils()->IsOverlayEnabled().
    Check https://partner.steamgames.com/doc/api/ISteamUtils#IsOverlayEnabled for more 
    information.
  */
  static bool overlay_enabled();

  /*
    Activates the Steam Overlay to a specific dialog. Valid dialog options are: 
      - friends
      - community
      - players
      - settings
      - officialgamegroup
      - stats
      - achievements

    Calls SteamFriends()->ActivateGameOverlay().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlay for more 
    information.
  */
  static void activate_overlay(const std::string& dialog);

  /*
    Activates Steam Overlay web browser directly to the specified URL. A fully qualified 
    address with the protocol is required, e.g. "http://www.steampowered.com". Calls 
    SteamFriends()->ActivateGameOverlayToWebPage().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToWebPage
    for more information.
  */
  static void activate_overlay_browser(const std::string& url);

  /*
    Activates Steam Overlay to a specific dialog. Valid dialog_name options are:
      - steamid - Opens the overlay web browser to the specified user or groups profile.
      - chat - Opens a chat window to the specified user, or joins the group chat.
      - jointrade - Opens a window to a Steam Trading session that was started with the 
        ISteamEconomy/StartTrade Web API.
      - stats - Opens the overlay web browser to the specified user's stats.
      - achievements - Opens the overlay web browser to the specified user's achievements.
      - friendadd - Opens the overlay in minimal mode prompting the user to add the target 
        user as a friend.
      - friendremove - Opens the overlay in minimal mode prompting the user to remove the 
        target friend.
      - friendrequestaccept - Opens the overlay in minimal mode prompting the user to accept 
        an incoming friend invite.
      - friendrequestignore - Opens the overlay in minimal mode prompting the user to ignore 
        an incoming friend invite.
    
    Calls SteamFriends()->ActivateGameOverlayToUser().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToUser
    for more information.
  */
  static void activate_overlay_user(const std::string& dialog_name, const uint64 steam_id);

  /*
    Sets the inset of the overlay notification from the corner specified by 
    c_overlay::set_overlay_notification_position(). A value of (0, 0) resets the position into the 
    corner. This position is per-game and is reset each launch. Calls 
    SteamUtils()->SetOverlayNotificationInset(). The insets units are in pixels.
    Check https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationInset
    for more information.
  */
  static void set_overlay_notification_inset(const int horizontal_inset, const int vertical_inset);

  /*
    Sets which corner the Steam overlay notification popup should display itself in. You 
    can also set the distance from the specified corner by using
    c_overlay::set_overlay_notification_inset(). This position is per-game and is reset each 
    launch. Valid notification_position options are:
      - k_EPositionTopLeft
      - k_EPositionTopRight
      - k_EPositionBottomLeft
      - k_EPositionBottomRight
    
    Calls SteamUtils()->SetOverlayNotificationPosition().
    Check https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationPosition
    for more information.
  */
  static void set_overlay_notification_position(const ENotificationPosition notification_position);

 private:
  /*
    Stores the Overlay's activation status.
  */
  bool overlay_activated_;
};
}  // namespace steamworks

#endif  // GAMECLIENT_OVERLAY_H
