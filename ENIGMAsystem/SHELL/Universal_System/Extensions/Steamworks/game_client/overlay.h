
#ifndef GAMECLIENTOVERLAY_H
#define GAMECLIENTOVERLAY_H

#include "game_client.h"

namespace steamworks {

class cgame_client;

class coverlay {
 private:
  bool is_overlay_activated_;

 public:
  coverlay();
  ~coverlay() = default;

  STEAM_CALLBACK(coverlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  bool is_overlay_activated();

  static bool is_overlay_enabled();
  static void activate_overlay(const std::string& dialog);

  static void activate_overlay_browser(const std::string& url);

  static void activate_overlay_user(const std::string& dialog_name, unsigned long long steamid);
};
}  // namespace steamworks

#endif  // GAMECLIENTOVERLAY_H
