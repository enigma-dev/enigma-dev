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

#include "c_overlay.h"

namespace steamworks {

////////////////////////////////////////////////////////
// Public fields & functions
////////////////////////////////////////////////////////

c_overlay::c_overlay()
    : overlay_activated_(false), m_CallbackGameOverlayActivated(this, &c_overlay::on_game_overlay_activated) {}

void c_overlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    c_overlay::overlay_activated_ = true;
    DEBUG_MESSAGE("Overlay activated successfully.", M_INFO);
  } else {
    c_overlay::overlay_activated_ = false;
    DEBUG_MESSAGE("Overlay deactivated successfully.", M_INFO);
  }
}

bool c_overlay::overlay_activated() { return c_overlay::overlay_activated_; }

////////////////////////////////////////////////////////
// Static fields & functions (AKA Wrapper functions)
////////////////////////////////////////////////////////

bool c_overlay::overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }

void c_overlay::activate_overlay(const std::string& dialog) { SteamFriends()->ActivateGameOverlay(dialog.c_str()); }

void c_overlay::activate_overlay_browser(const std::string& url) {
  SteamFriends()->ActivateGameOverlayToWebPage(url.c_str());
}

void c_overlay::activate_overlay_user(const std::string& dialog_name, const uint64 steam_id) {
  CSteamID c_steam_id(steam_id);
  SteamFriends()->ActivateGameOverlayToUser(dialog_name.c_str(), c_steam_id);
}

void c_overlay::set_overlay_notification_inset(const int horizontal_inset, const int vertical_inset) {
  SteamUtils()->SetOverlayNotificationInset(horizontal_inset, vertical_inset);
}

void c_overlay::set_overlay_notification_position(const ENotificationPosition notification_position) {
  SteamUtils()->SetOverlayNotificationPosition(notification_position);
}

}  // namespace steamworks
