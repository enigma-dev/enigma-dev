/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <string>

#include "joysticks.h"
#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFjoystick.h"

namespace {

SDL_Joystick *joystick1 = nullptr;
SDL_Joystick *joystick2 = nullptr;

static inline SDL_Joystick *joystick_get_handle(int id) {
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
  if (joystick1 == nullptr)
    joystick1 = SDL_JoystickOpen(0);

  if (joystick2 == nullptr)
    joystick2 = SDL_JoystickOpen(1);

  return init;
}

void joystick_uninit() {
  if (joystick1 != nullptr)
    SDL_JoystickClose(joystick1);

  if (joystick2 != nullptr)
    SDL_JoystickClose(joystick2);

  SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}

void joystick_update() {
  int joystick_count = SDL_NumJoysticks();
  if (joystick_count <= 0) {
    if (joystick1 != nullptr) {
      SDL_JoystickClose(joystick1);
      joystick1 = nullptr;
    }

    if (joystick2 != nullptr) {
      SDL_JoystickClose(joystick2);
      joystick2 = nullptr;
    }
  }

  if (joystick_count > 0) {
    if (joystick1 == nullptr)
      joystick1 = SDL_JoystickOpen(0);
    if (joystick2 == nullptr)
      joystick2 = SDL_JoystickOpen(1);
  }

  SDL_JoystickUpdate();
}

} // namespace enigma
