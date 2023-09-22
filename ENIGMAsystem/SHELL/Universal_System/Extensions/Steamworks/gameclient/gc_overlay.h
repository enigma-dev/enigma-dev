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
  The Steam overlay is a piece of the Steam user interface that can be activated over the top of 
  almost any game launched through Steam. It lets the user access the friends list, web browser, 
  chat, and in-game DLC purchasing.
  Check https://partner.steamgames.com/doc/features/overlay for more information.
*/

#ifndef GC_OVERLAY_H
#define GC_OVERLAY_H
#pragma once

// TODO: This documentation need to be improved when uploading a game to Steam Store.

#include "gameclient.h"

#ifdef ENIGMA_STEAMWORKS_API_MOCK
typedef GameOverlayActivatedMock GameOverlayActivated_t;
#endif  // ENIGMA_STEAMWORKS_API_MOCK

namespace steamworks_gc {

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCActivateGameOverlayToWebPageMode {
  k_GCActivateGameOverlayToWebPageMode_Default = 0,
  k_GCActivateGameOverlayToWebPageMode_Modal = 1
};

/**
 * @brief This enum is created to prevent usage of API's types in the extension.
 * 
 */
enum GCNotificationPosition {
  k_GCPosition_Invalid = -1,
  k_GCPosition_TopLeft = 0,
  k_GCPosition_TopRight = 1,
  k_GCPosition_BottomLeft = 2,
  k_GCPosition_BottomRight = 3,
};

class GameClient;

class GCOverlay {
 public:
  /*
    Overlay client constructor.
  */
  GCOverlay();

  /*
    Overlay client destructor.
  */
  ~GCOverlay() = default;

#ifndef ENIGMA_STEAMWORKS_API_MOCK
  /*
    Macro for listening to GameOverlayActivated_t callback. Callbacks are dispatched by
    calling gc_main::run_callbacks().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#GameOverlayActivated_t for
    more information.
  */
  STEAM_CALLBACK(GCOverlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);
#else
  /**
   * @brief This function is used to test the overlay activation.
   */
  static void on_game_overlay_activated(GameOverlayActivated_t* pCallback);
#endif  // ENIGMA_STEAMWORKS_API_MOCK

/*
    Checks Overlay's activation status. Returns true if the Overlay is activated. Otherwise, 
    it returns false. Overlay's activation status is stored in overlay_activated_ private
    variable.
  */
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  bool overlay_activated();
#else
  static bool overlay_activated();
#endif  // ENIGMA_STEAMWORKS_API_MOCK

  /*
    Checks if the Steam Overlay is running & the user can access it. The overlay process 
    could take a few seconds to start & hook the game process, so this function will 
    initially return false while the overlay is loading. Calls SteamUtils()->IsOverlayEnabled().
    Check https://partner.steamgames.com/doc/api/ISteamUtils#IsOverlayEnabled for more 
    information.
  */
  inline static bool overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }

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
  inline static void activate_overlay(const std::string& dialog) {
    SteamFriends()->ActivateGameOverlay(dialog.c_str());
#ifdef ENIGMA_STEAMWORKS_API_MOCK
    GameOverlayActivated_t pCallback;
    pCallback.m_bActive = !GCOverlay::overlay_activated_;
    pCallback.m_bUserInitiated = true;
    pCallback.m_nAppID = 480;  // Spacewar's AppID

    GCOverlay::on_game_overlay_activated(&pCallback);
#endif  // ENIGMA_STEAMWORKS_API_MOCK
  }

  /*
    Activates Steam Overlay web browser directly to the specified URL. A fully qualified 
    address with the protocol is required, e.g. "http://www.steampowered.com". Calls 
    SteamFriends()->ActivateGameOverlayToWebPage().
    Check https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToWebPage
    for more information.
  */
  inline static void activate_overlay_browser(const std::string& url) {
#ifndef ENIGMA_STEAMWORKS_API_MOCK
    SteamFriends()->ActivateGameOverlayToWebPage(
        url.c_str(), EActivateGameOverlayToWebPageMode::k_EActivateGameOverlayToWebPageMode_Default);
#else
    SteamFriends()->ActivateGameOverlayToWebPage(
        url.c_str(), (int)GCActivateGameOverlayToWebPageMode::k_GCActivateGameOverlayToWebPageMode_Default);

    GameOverlayActivated_t pCallback;
    pCallback.m_bActive = !GCOverlay::overlay_activated_;
    pCallback.m_bUserInitiated = true;
    pCallback.m_nAppID = 480;  // Spacewar's AppID

    GCOverlay::on_game_overlay_activated(&pCallback);
#endif  // ENIGMA_STEAMWORKS_API_MOCK
  }

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
  inline static void activate_overlay_user(const std::string& dialog_name, const uint64& steam_id) {
    CSteamID c_steam_id(steam_id);
    SteamFriends()->ActivateGameOverlayToUser(dialog_name.c_str(), c_steam_id);
#ifdef ENIGMA_STEAMWORKS_API_MOCK
    GameOverlayActivated_t pCallback;
    pCallback.m_bActive = !GCOverlay::overlay_activated_;
    pCallback.m_bUserInitiated = true;
    pCallback.m_nAppID = 480;  // Spacewar's AppID

    GCOverlay::on_game_overlay_activated(&pCallback);
#endif  // ENIGMA_STEAMWORKS_API_MOCK
  }

  /*
    Sets the inset of the overlay notification from the corner specified by 
    gc_overlay::set_overlay_notification_position(). A value of (0, 0) resets the position into the 
    corner. This position is per-game and is reset each launch. Calls 
    SteamUtils()->SetOverlayNotificationInset(). The insets units are in pixels.
    Check https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationInset
    for more information.
  */
  inline static void set_overlay_notification_inset(const int& horizontal_inset, const int& vertical_inset) {
    SteamUtils()->SetOverlayNotificationInset(horizontal_inset, vertical_inset);
  }

/*
    Sets which corner the Steam overlay notification popup should display itself in. You 
    can also set the distance from the specified corner by using
    gc_overlay::set_overlay_notification_inset(). This position is per-game and is reset each 
    launch. Valid notification_position options are:
      - k_EPositionTopLeft
      - k_EPositionTopRight
      - k_EPositionBottomLeft
      - k_EPositionBottomRight
    
    Calls SteamUtils()->SetOverlayNotificationPosition().
    Check https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationPosition
    for more information.
  */
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  inline static void set_overlay_notification_position(
      const GCNotificationPosition& gc_notification_position = GCNotificationPosition::k_GCPosition_Invalid) {
    ENotificationPosition notification_position{ENotificationPosition::k_EPositionInvalid};

    switch (gc_notification_position) {
      case GCNotificationPosition::k_GCPosition_TopLeft:
        notification_position = ENotificationPosition::k_EPositionTopLeft;
        break;
      case GCNotificationPosition::k_GCPosition_TopRight:
        notification_position = ENotificationPosition::k_EPositionTopRight;
        break;
      case GCNotificationPosition::k_GCPosition_BottomLeft:
        notification_position = ENotificationPosition::k_EPositionBottomLeft;
        break;
      case GCNotificationPosition::k_GCPosition_BottomRight:
        notification_position = ENotificationPosition::k_EPositionBottomRight;
        break;
      default:
        notification_position = ENotificationPosition::k_EPositionInvalid;
        break;
    }

    SteamUtils()->SetOverlayNotificationPosition(notification_position);
  }
#else
  inline static void set_overlay_notification_position(
      const GCNotificationPosition gc_notification_position = GCNotificationPosition::k_GCPosition_Invalid) {
    SteamUtils()->SetOverlayNotificationPosition((int)GCNotificationPosition::k_GCPosition_Invalid);
  }
#endif  // ENIGMA_STEAMWORKS_API_MOCK

 private:
  /*
    Stores the Overlay's activation status.
  */
#ifndef ENIGMA_STEAMWORKS_API_MOCK
  bool overlay_activated_;
#else
  static bool overlay_activated_;
#endif  // ENIGMA_STEAMWORKS_API_MOCK
};
}  // namespace steamworks_gc

#endif  // GC_OVERLAY_H
