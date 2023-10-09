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
 * @brief The Steam overlay is a piece of the Steam user interface that can be activated 
 *        over the top of almost any game launched through Steam. It lets the user access 
 *        the friends list, web browser, chat, and in-game DLC purchasing.
 * 
 * @note Check https://partner.steamgames.com/doc/features/overlay for more information.
 * 
 */

#ifndef GC_OVERLAY_H
#define GC_OVERLAY_H
#pragma once

#include "gameclient.h"

namespace steamworks_gc {

class GameClient;

class GCOverlay {
 public:
  // GCOverlay constructor.
  GCOverlay();

  // GCOverlay destructor.
  ~GCOverlay() = default;

  /**
   * @brief Construct a new steam callback object. This is a code generation macro for 
   *        listening to @c GameOverlayActivated_t callback.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamFriends#GameOverlayActivated_t 
   *      for more information.
   * @see @c GCMain::run_callbacks() function.
   * 
   */
  STEAM_CALLBACK(GCOverlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  /**
   * @brief Checks Overlay's activation status.
   * 
   * @return true if the Overlay is activated.
   * @return false if the Overlay is not activated.
   */
  bool overlay_activated();

  /**
   * @brief Checks if the Steam Overlay is running & the user can access it. The 
   *        overlay process could take a few seconds to start & hook the game process, 
   *        so this function will initially return false while the overlay is loading.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUtils#IsOverlayEnabled for more 
   *      information.
   * 
   * @return true 
   * @return false 
   */
  inline static bool overlay_enabled() {
    return steamworks_b::SteamBinder::ISteamUtils_IsOverlayEnabled(steamworks_b::SteamBinder::SteamUtils_vXXX());
  }

  /**
   * @brief Activates the Steam Overlay to a specific dialog. Valid dialog options are: 
   *         - @c friends
   *         - @c community
   *         - @c players
   *         - @c settings
   *         - @c officialgamegroup
   *         - @c stats
   *         - @c achievements
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlay for 
   *      more information.
   * 
   * @param dialog 
   */
  inline static void activate_overlay(const std::string& dialog) {
    steamworks_b::SteamBinder::ISteamFriends_ActivateGameOverlay(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                 dialog.c_str());
  }

#ifdef ENIGMA_FAKE_STEAMWORKS_API
  /**
   * @brief This function is used to deactivate the Steam Overlay.
   * 
   * @note this function is only for testing purposes.
   * 
   */
  inline static void deactivate_overlay() {
    steamworks_b::SteamBinder::ISteamFriends_DeactivateGameOverlay(steamworks_b::SteamBinder::SteamFriends_vXXX());
  }
#endif  // ENIGMA_FAKE_STEAMWORKS_API

  /**
   * @brief Activates Steam Overlay web browser directly to the specified URL. A fully 
   *        qualified address with the protocol is required, e.g. "http://www.steampowered.com".
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToWebPage 
   *      for more information.
   * 
   * @param url 
   */
  inline static void activate_overlay_browser(const std::string& url) {
    steamworks_b::SteamBinder::ISteamFriends_ActivateGameOverlayToWebPage(
        steamworks_b::SteamBinder::SteamFriends_vXXX(), url.c_str(),
        EActivateGameOverlayToWebPageMode::k_EActivateGameOverlayToWebPageMode_Default);
  }

  /**
   * @brief Activates Steam Overlay to a specific dialog. Valid dialog_name options are:
   *         - @c steamid - Opens the overlay web browser to the specified user or groups profile.
   *         - @c chat - Opens a chat window to the specified user, or joins the group chat.
   *         - @c jointrade - Opens a window to a Steam Trading session that was started with the 
   *              ISteamEconomy/StartTrade Web API.
   *         - @c stats - Opens the overlay web browser to the specified user's stats.
   *         - @c achievements - Opens the overlay web browser to the specified user's achievements.
   *         - @c friendadd - Opens the overlay in minimal mode prompting the user to add the target 
   *              user as a friend.
   *         - @c friendremove - Opens the overlay in minimal mode prompting the user to remove the 
   *              target friend.
   *         - @c friendrequestaccept - Opens the overlay in minimal mode prompting the user to accept 
   *              an incoming friend invite.
   *         - @c friendrequestignore - Opens the overlay in minimal mode prompting the user to ignore 
   *              an incoming friend invite.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamFriends#ActivateGameOverlayToUser for 
   *      more information.
   * 
   * @param dialog_name 
   * @param steam_id 
   */
  inline static void activate_overlay_user(const std::string& dialog_name, const uint64& steam_id) {
    steamworks_b::SteamBinder::ISteamFriends_ActivateGameOverlayToUser(steamworks_b::SteamBinder::SteamFriends_vXXX(),
                                                                       dialog_name.c_str(), steam_id);
  }

  /**
   * @brief Sets the inset of the overlay notification from the corner specified by 
   *        @c GCOverlay::set_overlay_notification_position() function. A value of (0, 0) 
   *        resets the position into the corner. This position is per-game and is 
   *        reset each launch.
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationInset for more 
   *      information.
   * 
   * @param horizontal_inset 
   * @param vertical_inset 
   */
  inline static void set_overlay_notification_inset(const int& horizontal_inset, const int& vertical_inset) {
    steamworks_b::SteamBinder::ISteamUtils_SetOverlayNotificationInset(steamworks_b::SteamBinder::SteamUtils_vXXX(),
                                                                       horizontal_inset, vertical_inset);
  }

  /**
   * @brief Sets which corner the Steam overlay notification popup should display itself in. You 
   *        can also set the distance from the specified corner by using
   *        @c GCOverlay::set_overlay_notification_inset() function. This position is per-game and is reset each 
   *        launch. Valid notification_position options are:
   *         - @c k_EPositionTopLeft
   *         - @c k_EPositionTopRight
   *         - @c k_EPositionBottomLeft
   *         - @c k_EPositionBottomRight
   * 
   * @see https://partner.steamgames.com/doc/api/ISteamUtils#SetOverlayNotificationPosition for more 
   *      information.
   * 
   * @param notification_position 
   */
  inline static void set_overlay_notification_position(
      const ENotificationPosition& notification_position = ENotificationPosition::k_EPositionInvalid) {
    steamworks_b::SteamBinder::ISteamUtils_SetOverlayNotificationPosition(steamworks_b::SteamBinder::SteamUtils_vXXX(),
                                                                          notification_position);
  }

 private:
  /**
   * @brief Stores the Overlay's activation status.
   * 
   * @see @c GCOverlay::overlay_activated() function.
   * 
   */
  bool overlay_activated_;
};
}  // namespace steamworks_gc

#endif  // GC_OVERLAY_H
