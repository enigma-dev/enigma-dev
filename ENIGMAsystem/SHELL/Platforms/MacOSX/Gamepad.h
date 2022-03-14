#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "Platforms/General/PFgamepad.h"

#include <SDL2/SDL.h>

#include <vector>

namespace enigma {

struct GamepadState {
  bool lastButtonStatus[enigma_user::gp_padr];
  bool buttonStatus[enigma_user::gp_padr];
};

struct Gamepad {
  SDL_GameController *controller;
  SDL_Haptic *haptic;
  GamepadState state;
  float thresh = 0.05f; //FIXME: these do nothing atm because I have no idea how gm behaves
  float deadzone = 0.0f;
  void clear();
  void push();
};

extern std::vector<Gamepad> gamepads;

void initGamepads();
void cleanupGamepads();
void addGamepad(unsigned i);
void removeGamepad(unsigned i);
void pushGamepads();
void setGamepadButton(int gamepad, int btn, bool pressed);

}  // namespace enigma

#endif
