#if !defined(GAME_CLIENT_H)
#define GAME_CLIENT_H

#include <iostream>
#include <string>
#include "public/steam/steam_api.h"
#include "Widget_Systems/widgets_mandatory.h"

namespace steamworks {

class coverlay;

class cgame_client {
 private:
  coverlay* overlay;

  CSteamID steam_id_local_user_;
  unsigned steam_app_id_;

  std::string steam_persona_name_;

  std::string current_game_language_;
  std::string available_languages_;

  // ISteamUser* steam_user_;
  // ISteamFriends* steam_friends_;
  // ISteamUtils* steam_utils_;

 public:
  // cgame_client(ISteamUser* steam_user, ISteamFriends* steam_friends, ISteamUtils* steam_utils);
  cgame_client();
  ~cgame_client();

  void init();

  coverlay* get_overlay();

  bool is_user_logged_on();

  CSteamID get_steam_id_local_user();
  unsigned get_steam_app_id();

  std::string get_steam_persona_name();

  static std::string get_steam_user_persona_name(CSteamID user_persona_name);

  std::string get_current_game_language();
  std::string get_available_languages();

  static bool is_subscribed();
};
}  // namespace steamworks

#endif  // !GAME_CLIENT_H
