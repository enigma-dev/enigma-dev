#include "overlay.h"

namespace steamworks {

coverlay::coverlay()
    : is_overlay_activated_(false), m_CallbackGameOverlayActivated(this, &coverlay::on_game_overlay_activated) {}

void coverlay::activate_overlay(const std::string& dialog) {
  SteamFriends()->ActivateGameOverlay(dialog.c_str());
}

void coverlay::on_game_overlay_activated(GameOverlayActivated_t* pCallback) {
  if (pCallback->m_bActive) {
    coverlay::is_overlay_activated_ = true;
    std::cout << "yaaaaaaaaaaaaaaaaaaaaaaaaaaaaaay!!!!!!!!!!" << std::endl;
  } else {
    coverlay::is_overlay_activated_ = false;
    std::cout << "nooooooooooooooooooooooooooooooo!!!!!!!!!!" << std::endl;
  }
}

bool coverlay::is_overlay_enabled() { return SteamUtils()->IsOverlayEnabled(); }
bool coverlay::is_overlay_activated() { return coverlay::is_overlay_activated_; }

}  // namespace steamworks
