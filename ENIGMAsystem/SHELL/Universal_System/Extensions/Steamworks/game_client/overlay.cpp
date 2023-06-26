#include "overlay.h"

namespace steamworks {

coverlay::coverlay()
    : is_overlay_activated_(false), m_CallbackGameOverlayActivated(this, &coverlay::on_game_overlay_activated) {}

void coverlay::activate_overlay(const std::string& dialog) { SteamFriends()->ActivateGameOverlay(dialog.c_str()); }

void coverlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    coverlay::is_overlay_activated_ = true;
    DEBUG_MESSAGE("Overlay activated", M_INFO);
  } else {
    coverlay::is_overlay_activated_ = false;
    DEBUG_MESSAGE("Overlay deactivated", M_INFO);
  }
}

bool coverlay::is_overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }
bool coverlay::is_overlay_activated() { return coverlay::is_overlay_activated_; }

}  // namespace steamworks
