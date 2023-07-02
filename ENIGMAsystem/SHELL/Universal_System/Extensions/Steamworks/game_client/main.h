#ifndef GAMECLIENTMAIN_H
#define GAMECLIENTMAIN_H

#include "game_client.h"

namespace steamworks {

class c_game_client;

class c_main {
 public:
  static bool init();

  static void shutdown();

  static bool is_initialised();

  static c_game_client* get_c_game_client();

  static void run_callbacks();

  static void set_warning_message_hook();

 private:
  static c_game_client* c_game_client_;

  static bool is_initialised_;
};
}  // namespace steamworks

#endif  // GAMECLIENTMAIN_H
