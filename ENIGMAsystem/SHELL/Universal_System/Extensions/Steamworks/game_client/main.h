#if !defined(GAME_CLIENT_MAIN_H)
#define GAME_CLIENT_MAIN_H

#include "game_client.h"

namespace steamworks {

class cgame_client;

class cmain {
 private:
  static cgame_client* game_client_;

  static bool is_initialised_;

 public:
  static bool init();

  static void shutdown();

  static bool is_initialised();

  static cgame_client* get_game_client();
};
}  // namespace steamworks

#endif  // !GAME_CLIENT_MAIN_H
