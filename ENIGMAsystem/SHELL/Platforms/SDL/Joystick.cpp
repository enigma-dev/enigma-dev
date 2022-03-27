/** Copyright (C) 2018 Samuel Venable
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

#include <SDL.h>
#include "Joystick.h"
#include "Platforms/General/PFjoystick.h"
#include <string>

namespace {

SDL_Joystick *joystick1 = NULL;
SDL_Joystick *joystick2 = NULL;

static inline SDL_Joystick* joystick_get_handle(int id) {
  return (id - 1 < 0.5) ? joystick1 : joystick2;
}

} // anonymous namespace

namespace enigma_user {

bool joystick_exists(int id) {
  return (SDL_JoystickGetAttached(joystick_get_handle(id)) == SDL_TRUE);
}

std::string joystick_name(int id) {
  const char *name;
  name = SDL_JoystickName(joystick_get_handle(id));
  return (name ? name : "");
}

int joystick_axes(int id) {
  int axes;
  SDL_Joystick *joystick = joystick_get_handle(id);
  axes = SDL_JoystickNumAxes(joystick);
  return (axes > 0 ? axes : 0);
}

int joystick_buttons(int id) {
  int buttons;
  SDL_Joystick *joystick = joystick_get_handle(id);
  buttons = SDL_JoystickNumButtons(joystick);
  return (buttons > 0 ? buttons : 0);
}

bool joystick_button(int id, int numb) {
  return SDL_JoystickGetButton(joystick_get_handle(id), numb - 1);
}

double joystick_axis(int id, int numb) {
  double axis;
  double pos = 0;
  axis = SDL_JoystickGetAxis(joystick_get_handle(id), numb - 1);
  if (axis != -256 && axis != 0) // don't ask why this shit is necessary; it just is...
    pos = (axis / 32768);
  else if (joystick_exists(id))
    pos = -0.01;
  return pos;
}

} // namespace enigma_user

namespace enigma {

bool joystick_init() {
  double init;
  init = (SDL_InitSubSystem(SDL_INIT_JOYSTICK) > 0);
  if (joystick1 == NULL)
    joystick1 = SDL_JoystickOpen(0);

  if (joystick2 == NULL)
    joystick2 = SDL_JoystickOpen(1);

  return init;
}

void joystick_uninit() {
  if (joystick1 != NULL)
    SDL_JoystickClose(joystick1);

  if (joystick2 != NULL)
    SDL_JoystickClose(joystick2);

  SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void joystick_update() {
  int joystick_count = SDL_NumJoysticks();
  if (joystick_count <= 0) {
    if (joystick1 != NULL) {
      SDL_JoystickClose(joystick1);
      joystick1 = NULL;
    }

    if (joystick2 != NULL) {
      SDL_JoystickClose(joystick2);
      joystick2 = NULL;
    }
  }

  if (joystick_count > 0) {
    if (joystick1 == NULL)
      joystick1 = SDL_JoystickOpen(0);
    if (joystick2 == NULL)
      joystick2 = SDL_JoystickOpen(1);
  }

  SDL_JoystickUpdate();
}

} // namespace enigma
