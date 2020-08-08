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

#include <string>
#include <vector>

#include "SDL2/SDL.h"
#include "Joystick.h"
#include "Platforms/General/PFjoystick.h"
#include "Widget_Systems/widgets_mandatory.h"

#ifdef DEBUG_MODE
#include "Universal_System/Resources/resource_data.h"
#include "Universal_System/Object_Tiers/object.h"
#include "Universal_System/debugscope.h"
#endif

namespace enigma {

struct JoystickState {
  bool lastButtonStatus[enigma_user::gp_padr];
  bool buttonStatus[enigma_user::gp_padr];
};

struct Joystick {
  SDL_Joystick *joystick;
  JoystickState state;
  float thresh = 0.05f; //FIXME: these do nothing atm because I have no idea how gm behaves
  float deadzone = 0.0f;
  void clear();
  void push();
};

static std::vector<Joystick> joysticks;

void Joystick::clear() {
  for (unsigned i = 0; i < enigma_user::gp_padr; ++i) {
    state.lastButtonStatus[i] = false;
    state.buttonStatus[i] = false;
  }
}

void Joystick::push() {
  for (unsigned i = 0; i < enigma_user::gp_padr; ++i) {
    state.lastButtonStatus[i] = state.buttonStatus[i];
  }
}

} // namespace enigma

namespace {

SDL_Joystick *joystick_get_handle(int id) {
  // joystick id starts at 1 in GameMaker
  #ifdef DEBUG_MODE
  if (id < 1 || id > joysticks.size())) { 
    DEBUG_MESSAGE("invalid joystick id: " + std::to_string(id - 1), MESSAGE_TYPE::M_INFO); 
    return nullptr
  }
  #endif
  return joysticks.joystick[id - 1];
}

} // anonymous namespace

namespace enigma_user {

bool joystick_exists(int id) {
  return (SDL_JoystickGetAttached(joystick_get_handle(id)) == SDL_TRUE);
}

std::string joystick_name(int id) {
  const char *name = SDL_JoystickName(joystick_get_handle(id));
  #ifdef DEBUG_MODE
  if (name == nullptr) {
    DEBUG_MESSAGE(std::string("SDL_JoystickName failed: ") + SDL_GetError(), MESSAGE_TYPE::M_INFO);
  }
  #endif
  return ((name == nullptr) ? name : "");
}

int joystick_axes(int id) {
  SDL_Joystick *joystick = joystick_get_handle(id);
  int axes = SDL_JoystickNumAxes(joystick);
  #ifdef DEBUG_MODE
  if (axes < 0) {
    DEBUG_MESSAGE(std::string("SDL_JoystickNumAxes failed: ") + SDL_GetError(), MESSAGE_TYPE::M_INFO);
  }
  #endif
  return (axes > 0 ? axes : 0);
}

int joystick_buttons(int id) {
  SDL_Joystick *joystick = joystick_get_handle(id);
  int buttons = SDL_JoystickNumButtons(joystick);
  #ifdef DEBUG_MODE
  if (buttons < 0) {
    DEBUG_MESSAGE(std::string("SDL_JoystickNumButtons failed: ") + SDL_GetError(), MESSAGE_TYPE::M_INFO);
  }
  #endif
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
  for (size_t i = 0; i < joysticks.size(); i++) {
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

void joysticks_open(int id) {
  #ifdef DEBUG_MODE
  DEBUG_MESSAGE(std::string("adding joystick ") + std::to_string(i - 1) + std::string("..."), MESSAGE_TYPE::M_INFO);
  #endif
  Joystick j 
  j.clear();
  j.joystick = SDL_JoystickOpen(i);
  int joystick_count = SDL_NumJoysticks();
  if (joysticks.size() < joystick_count) {
    joysticks.resize(joystick_count, nullptr);
  }
  if (i < joysticks.size()) {
    if (j == nullptr) {
      if (i < gamepads.size()) {
        joysticks[i] = j;
      } else {
        joysticks.push_back(j);
      }
    }
  }
  #ifdef DEBUG_MODE
  if (joysticks[i] == nullptr) 
    DEBUG_MESSAGE(std::string("joystick ") + std::to_string(i - 1) + std::string("(\"")
      enigma_user::joystick_name(i) + std::string("\") not added!"), MESSAGE_TYPE::M_INFO);
  } else {
    DEBUG_MESSAGE(std::string("joystick ") + std::to_string(i - 1) + std::string("(\"")
      enigma_user::joystick_name(i) + std::string("\") added!"), MESSAGE_TYPE::M_INFO);
  }
  #endif
}

void joysticks_close(int id) {
  if (joysticks[i] != nullptr) {
    #ifdef DEBUG_MODE
    DEBUG_MESSAGE(std::string("removing joystick ") + std::to_string(i - 1) + std::string(" (\"")
      enigma_user::joystick_name(i) + std::string("\")..."), MESSAGE_TYPE::M_INFO);
    #endif
    SDL_JoystickClose(joysticks[i].joystick);
  }
  #ifdef DEBUG_MODE
  if (joysticks[i] == nullptr) {
    DEBUG_MESSAGE(std::string("joystick ") + std::to_string(i - 1) + std::string(" doesn't exit; cannot close!"), MESSAGE_TYPE::M_INFO);
  } else {
    DEBUG_MESSAGE(std::string("joystick ") + std::to_string(i - 1) + std::string(" removed!"), MESSAGE_TYPE::M_INFO);
  }
  #endif
  joysticks[i] = nullptr;
}

void joystick_update() {
  SDL_JoystickUpdate();
}

} // namespace enigma
