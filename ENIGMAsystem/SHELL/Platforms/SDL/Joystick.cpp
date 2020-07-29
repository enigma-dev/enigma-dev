/** Copyright (C) 2018-2020 Samuel Venable
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include "SDL2/SDL.h"
#include "Joystick.h"
#include "Platforms/General/PFjoystick.h"
#include <vector>
#include <string>

namespace {

size_t JOYSTICK_MAX = 0;

std::vector<SDL_Joystick *>(JOYSTICK_MAX);

static inline SDL_Joystick *joystick_get_handle(int id) {
  return (id < JOYSTICK_MAX) ? ((id < 1) ? joysticks[0] : joysticks[id - 1]) : joysticks[JOYSTICK_MAX - 1];
}

static inline void joysticks_open() {
  for (size_t i = 0; i < joysticks.size(); i++) {
    if (joysticks[i] == nullptr) {
      joysticks[i] = SDL_JoystickOpen(i);
    }
  }
}

static inline void joysticks_close() {
  for (size_t i = 0; i < joysticks.size(); i++) {
    if (joysticks[i] != nullptr) {
      SDL_JoystickClose(joysticks[i]);
      joysticks[i] = nullptr;
    }
  }
}

} // anonymous namespace

namespace enigma_user {

bool joystick_exists(int id) {
  return (SDL_JoystickGetAttached(joystick_get_handle(id)) == SDL_TRUE);
}

std::string joystick_name(int id) {
  const char *name = SDL_JoystickName(joystick_get_handle(id));
  return (name ? name : "");
}

int joystick_axes(int id) {
  SDL_Joystick *joystick = joystick_get_handle(id);
  int axes = SDL_JoystickNumAxes(joystick);
  return (axes > 0 ? axes : 0);
}

int joystick_buttons(int id) {
  SDL_Joystick *joystick = joystick_get_handle(id);
  int buttons = SDL_JoystickNumButtons(joystick);
  return (buttons > 0 ? buttons : 0);
}

bool joystick_button(int id, int numb) {
  return SDL_JoystickGetButton(joystick_get_handle(id), numb - 1);
}

double joystick_axis(int id, int numb) {
  double pos = 0;
  double axis = SDL_JoystickGetAxis(joystick_get_handle(id), numb - 1);
  if (axis != -256 && axis != 0) // don't ask why this shit is necessary; it just is...
    pos = (axis / 32768);
  else if (joystick_exists(id))
    pos = -0.01;
  return pos;
}

} // namespace enigma_user

namespace enigma {

bool joystick_init() {
  for (size_t i = 0; i < JOYSTICK_MAX; i++) {
    joysticks[i] = nullptr;
  }
  bool init = (SDL_InitSubSystem(SDL_INIT_JOYSTICK) > 0);
  joysticks_open();
  return init;
}

void joystick_uninit() {
  joysticks_close();
  SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void joystick_update() {
  size_t joystick_count = SDL_NumJoysticks();
  if (joystick_count <= 0) {
    joysticks_close();
  } else {
    if (joystick_count != JOYSTICK_MAX) {
      joysticks.resize(JOYSTICK_MAX, nullptr);
      JOYSTICK_MAX = joystick_count;
    }
    joysticks_open();
  }
  SDL_JoystickUpdate();
}

} // namespace enigma
