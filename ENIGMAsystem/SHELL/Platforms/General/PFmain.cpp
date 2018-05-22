#include "PFmain.h"

namespace enigma {
  
bool game_isending = false;
int game_return = 0;

} //namespace enigma

namespace enigma_user {

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void action_end_game() {
  return game_end();
}

} //namespace enigma_user
