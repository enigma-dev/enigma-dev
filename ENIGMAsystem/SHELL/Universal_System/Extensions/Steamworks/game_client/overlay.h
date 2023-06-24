
#if !defined(GAME_CLIENT_OVERLAY_H)
#define GAME_CLIENT_OVERLAY_H

#include "game_client.h"

namespace steamworks {

class cgame_client;

class coverlay {
 private:
  bool is_overlay_activated_;

 public:
  coverlay();
  ~coverlay() = default;

  void activate_overlay(const std::string& dialog);

  STEAM_CALLBACK(coverlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  bool is_overlay_enabled();
  bool is_overlay_activated();
};
}  // namespace steamworks

#endif  // !GAME_CLIENT_OVERLAY_H
