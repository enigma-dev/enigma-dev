#include "Gamepad.h"
#include "Widget_Systems/widgets_mandatory.h"

#include <string>

namespace enigma {

static std::vector<Gamepad> gamepads;

using namespace enigma_user;

int digitalButtons[gp_axisrv] = {
  gp_face1, gp_face2, gp_face3, gp_face4, gp_select, 0, gp_start, gp_stickl, gp_stickr, gp_shoulderl, gp_shoulderr,
  gp_padu, gp_padd, gp_padl, gp_padr, 0, 0, 0, 0
};

void Gamepad::clear() {
  for (unsigned i = 0; i < enigma_user::gp_padr; ++i) {
    state.lastButtonStatus[i] = false;
    state.buttonStatus[i] = false;
  }
}

void Gamepad::push() {
  for (unsigned i = 0; i < enigma_user::gp_padr; ++i) {
    state.lastButtonStatus[i] = state.buttonStatus[i];
  }
}

void initGamepads() {
  SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
  for (int j = 0; j < SDL_NumJoysticks(); ++j) addGamepad(j);
}

void cleanupGamepads() {
  for (size_t i = 0; i < gamepads.size(); ++i) removeGamepad(i);
}

void pushGamepads() {
  for (Gamepad& gp : gamepads) gp.push();
}

void setGamepadButton(int gamepad, int btn, bool pressed) {
#ifdef DEBUG_MODE
  if (gamepad < 0 || gamepad >= static_cast<int>(gamepads.size())) return;
  if (btn < 0 || btn >= gp_axisrv) return;
#endif
  gamepads[gamepad].state.buttonStatus[digitalButtons[btn]] = pressed;
}

void addGamepad(unsigned i) {
  if (!SDL_IsGameController(i)) {
    DEBUG_MESSAGE("Could not open gamepad " + std::to_string(i) + ": " + SDL_GetError(), MESSAGE_TYPE::M_ERROR);
    return;
  }

  Gamepad g;
  g.clear();
  g.controller = SDL_GameControllerOpen(i);

  if (g.controller == nullptr) {
    DEBUG_MESSAGE("Could not open gamepad " + std::to_string(i) + ": " + SDL_GetError(), MESSAGE_TYPE::M_ERROR);
    return;
  }

  DEBUG_MESSAGE("Connected gamepad " + std::to_string(i) + " " + SDL_GameControllerName(g.controller), MESSAGE_TYPE::M_INFO);

  SDL_Joystick* j = SDL_GameControllerGetJoystick(g.controller);

  if (SDL_JoystickIsHaptic(j)) {
    g.haptic = SDL_HapticOpenFromJoystick(j);

    if (SDL_HapticRumbleSupported(g.haptic) && SDL_HapticRumbleInit(g.haptic))
      DEBUG_MESSAGE("Rumble Supported: yes", MESSAGE_TYPE::M_INFO);
    else
      DEBUG_MESSAGE("Rumble Supported: no", MESSAGE_TYPE::M_INFO);
  }

  if (i < gamepads.size())
    gamepads[i] = g;
  else
    gamepads.push_back(g);
}

void removeGamepad(unsigned i) {
  DEBUG_MESSAGE("Disconnected gamepad " + std::to_string(i) + ": " + SDL_GameControllerName(gamepads[i].controller), MESSAGE_TYPE::M_INFO);
  if (gamepads[i].haptic != nullptr) SDL_HapticClose(gamepads[i].haptic);
  SDL_GameControllerClose(gamepads[i].controller);
}

}  // namespace enigma

using enigma::gamepads;

namespace enigma_user {
bool gamepad_is_supported() {
  //SDL supports gamepads
  return true;
}

int gamepad_get_device_count() { return gamepads.size(); }

bool gamepad_is_connected(int id) { return id >= 0 && id < SDL_NumJoysticks() && SDL_IsGameController(id); }

std::string gamepad_get_description(int id) { return SDL_GameControllerNameForIndex(id); }

int gamepad_axis_count(int id) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return 0;
#endif

  SDL_GameController* c = gamepads[id].controller;

#ifdef DEBUG_MODE
  if (c == nullptr) return 0;
#endif

  SDL_Joystick* j = SDL_GameControllerGetJoystick(c);
  return SDL_JoystickNumAxes(j);
}

float gamepad_axis_value(int id, int axis) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return 0;
#endif

  SDL_GameController* c = gamepads[id].controller;

#ifdef DEBUG_MODE
  if (c == nullptr) return 0;
#endif

  SDL_GameControllerAxis SDLaxis;

  switch (axis) {
    case gp_axislh:
      SDLaxis = SDL_CONTROLLER_AXIS_LEFTX;
      break;
    case gp_axislv:
      SDLaxis = SDL_CONTROLLER_AXIS_LEFTY;
      break;
    case gp_axisrh:
      SDLaxis = SDL_CONTROLLER_AXIS_RIGHTX;
      break;
    case gp_axisrv:
      SDLaxis = SDL_CONTROLLER_AXIS_RIGHTY;
      break;
    case gp_shoulderlb:
      SDLaxis = SDL_CONTROLLER_AXIS_TRIGGERLEFT;
      break;
    case gp_shoulderrb:
      SDLaxis = SDL_CONTROLLER_AXIS_TRIGGERRIGHT;
      break;

    default:
      return 0;
  }

  //The state is a value ranging from -32768 to 32767.
  return SDL_GameControllerGetAxis(c, SDLaxis) / 32768;
}

bool gamepad_button_check(int id, int btn) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return false;
#endif

  return gamepads[id].state.buttonStatus[btn];
}

bool gamepad_button_check_pressed(int id, int btn) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size()) || btn > gp_padr) return false;
#endif

  auto g = gamepads[id].state;

  if (btn == gp_any || btn == gp_none) {
    for (int i = 0; i < gp_axisrv; i++)
      if (g.buttonStatus[i] && !g.lastButtonStatus[i]) return (btn == gp_any);

    return !(btn == gp_any);
  }

  return g.buttonStatus[btn] && !g.lastButtonStatus[btn];
}

bool gamepad_button_check_released(int id, int btn) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size()) || btn > gp_padr) return false;
#endif

  auto g = gamepads[id].state;

  if (btn == gp_any || btn == gp_none) {
    for (int i = 0; i < gp_axisrv; i++)
      if (!g.buttonStatus[i] && g.lastButtonStatus[i]) return (btn == gp_any);

    return !(btn == gp_any);
  }

  return !g.buttonStatus[btn] && g.lastButtonStatus[btn];
}

int gamepad_button_count(int id) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return 0;
#endif

  SDL_GameController* c = gamepads[id].controller;

#ifdef DEBUG_MODE
  if (c == nullptr) return 0;
#endif

  SDL_Joystick* j = SDL_GameControllerGetJoystick(c);
  return SDL_JoystickNumButtons(j);
}

float gamepad_button_value(int id, int btn) {
  //analog buttons are axes
  return gamepad_axis_value(id, btn);
}

void gamepad_set_vibration(int id, double left, double right) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return;
#endif

  if (gamepads[id].haptic != nullptr) {
    if (left == 0)
      SDL_HapticRumbleStop(gamepads[id].haptic);
    else
      SDL_HapticRumblePlay(gamepads[id].haptic, left, SDL_HAPTIC_INFINITY);
  }
}

float gamepad_get_button_threshold(int id) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return 0;
#endif

  return gamepads[id].thresh;
}

void gamepad_set_button_threshold(int id, double threshold) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return;
#endif

  gamepads[id].thresh = threshold;
}

float gamepad_get_axis_deadzone(int id, double deadzone) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return 0;
#endif

  return gamepads[id].deadzone;
}

void gamepad_set_axis_deadzone(int id, double deadzone) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= static_cast<int>(gamepads.size())) return;
#endif

  gamepads[id].deadzone = deadzone;
}

//gamepad_set_colour is playstation 4 only so why bother?

}  // namespace enigma_user
