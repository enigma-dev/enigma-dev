#ifndef GAMECLIENTOVERLAY_H
#define GAMECLIENTOVERLAY_H

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_overlay {
 public:
  c_overlay();
  ~c_overlay() = default;

  STEAM_CALLBACK(c_overlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  bool is_overlay_activated();

  static bool is_overlay_enabled();
  static void activate_overlay(const std::string& dialog);

  static void activate_overlay_browser(const std::string& url);

  static void activate_overlay_user(const std::string& dialog_name, unsigned long long steam_id);

 private:
  bool is_overlay_activated_;
};
}  // namespace steamworks

#endif  // GAMECLIENTOVERLAY_H
