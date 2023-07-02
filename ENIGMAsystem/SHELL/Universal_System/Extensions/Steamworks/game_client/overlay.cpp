#include "overlay.h"

namespace steamworks {

c_overlay::c_overlay()
    : is_overlay_activated_(false), m_CallbackGameOverlayActivated(this, &c_overlay::on_game_overlay_activated) {}

void c_overlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    c_overlay::is_overlay_activated_ = true;
    DEBUG_MESSAGE("Overlay activated", M_INFO);
  } else {
    c_overlay::is_overlay_activated_ = false;
    DEBUG_MESSAGE("Overlay deactivated", M_INFO);
  }
}

bool c_overlay::is_overlay_activated() { return c_overlay::is_overlay_activated_; }

void c_overlay::activate_overlay(const std::string& dialog) { SteamFriends()->ActivateGameOverlay(dialog.c_str()); }

bool c_overlay::is_overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }

void c_overlay::activate_overlay_browser(const std::string& url) {
  SteamFriends()->ActivateGameOverlayToWebPage(url.c_str());
}

void c_overlay::activate_overlay_user(const std::string& dialog_name, unsigned long long steam_id) {
  CSteamID c_steam_id(steam_id);
  SteamFriends()->ActivateGameOverlayToUser(dialog_name.c_str(), c_steam_id);
}

}  // namespace steamworks
