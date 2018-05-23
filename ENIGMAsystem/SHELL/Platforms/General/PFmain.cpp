#include "PFmain.h"

namespace enigma {

bool game_isending = false;
int game_return = 0;

long clamp(long value, long min, long max) {
  if (value < min) return min;
  if (value > max) return max;
  return value;
}

}  //namespace enigma

namespace enigma_user {

void game_end(int ret) {
  enigma::game_isending = true;
  enigma::game_return = ret;
}

void action_end_game() { return game_end(); }

}  //namespace enigma_user