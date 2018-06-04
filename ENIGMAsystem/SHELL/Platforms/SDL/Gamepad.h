#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <vector>

namespace enigma_user {
enum {

  gp_any = -1,
  gp_none = -2,

  gp_face1 = 0,
  gp_face2 = 1,
  gp_face3 = 2,
  gp_face4 = 3,
  gp_shoulderl = 4,
  gp_shoulderr = 5,
  gp_shoulderlb = 6,
  gp_shoulderrb = 7,
  gp_select = 8,
  gp_start = 9,
  gp_stickl = 10,
  gp_stickr = 11,
  gp_padu = 12,
  gp_padd = 13,
  gp_padl = 14,
  gp_padr = 15,
  gp_axislh = 16,
  gp_axislv = 17,
  gp_axisrh = 18,
  gp_axisrv = 19,

  gp_empty = 20,
  gp_low = 21,
  gp_medium = 22,
  gp_full = 23,

  gp_disconnected = 24,
  gp_wired = 25,
  gp_alkaline = 26,
  gp_nimh = 27,
  gp_unknown = 28
};
}

namespace enigma {
struct GamepadState {
  bool lastButtonStatus[enigma_user::gp_padr];
  bool buttonStatus[enigma_user::gp_padr];
};

extern std::vector<GamepadState> gamepads;
}  // namespace enigma

#endif