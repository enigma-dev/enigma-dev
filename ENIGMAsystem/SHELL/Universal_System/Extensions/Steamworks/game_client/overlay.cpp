#include "overlay.h"

namespace steamworks {

coverlay::coverlay()
    : is_overlay_activated_(false), m_CallbackGameOverlayActivated(this, &coverlay::on_game_overlay_activated) {}

void coverlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    coverlay::is_overlay_activated_ = true;
    DEBUG_MESSAGE("Overlay activated", M_INFO);
  } else {
    coverlay::is_overlay_activated_ = false;
    DEBUG_MESSAGE("Overlay deactivated", M_INFO);
  }
}

bool coverlay::is_overlay_activated() { return coverlay::is_overlay_activated_; }

void coverlay::activate_overlay(const std::string& dialog) { SteamFriends()->ActivateGameOverlay(dialog.c_str()); }

bool coverlay::is_overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }

void coverlay::activate_overlay_browser(const std::string& url) {
  SteamFriends()->ActivateGameOverlayToWebPage(url.c_str());
}

void coverlay::activate_overlay_user(const std::string& dialog_name, unsigned long long steamid) {
  CSteamID steam_id(steamid);
  SteamFriends()->ActivateGameOverlayToUser(dialog_name.c_str(), steam_id);
}

}  // namespace steamworks
