#ifndef GAMECLIENT_OVERLAY_H
#define GAMECLIENT_OVERLAY_H

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_overlay {
 public:
  c_overlay();
  ~c_overlay() = default;

  STEAM_CALLBACK(c_overlay, on_game_overlay_activated, GameOverlayActivated_t, m_CallbackGameOverlayActivated);

  bool overlay_activated();

  static bool overlay_enabled();
  static void activate_overlay(const std::string& dialog);

  static void activate_overlay_browser(const std::string& url);

  static void activate_overlay_user(const std::string& dialog_name, const unsigned long long steam_id);

  static void set_overlay_notification_inset(const int horizontal_inset, const int vertical_inset);

  static void set_overlay_notification_position(const ENotificationPosition notification_position);

 private:
  bool overlay_activated_;
};
}  // namespace steamworks

#endif  // GAMECLIENT_OVERLAY_H
