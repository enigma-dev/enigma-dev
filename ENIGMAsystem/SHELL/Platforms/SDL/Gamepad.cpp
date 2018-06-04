#include "Gamepad.h"

#include <SDL2/SDL.h>

#include <string>

namespace enigma {
  std::vector<GamepadState> gamepads;
}

namespace enigma_user {

bool gamepad_is_supported() {
  //SDL supports gamepads
  return true;
}

int gamepad_get_device_count() {
  int count = 0;

  for (int i = 0; i < SDL_NumJoysticks(); ++i)
    if (SDL_IsGameController(i)) count++;

  return count;
}

bool gamepad_is_connected(int id) { return id >= 0 && id < SDL_NumJoysticks() - 1 && SDL_IsGameController(id); }

std::string gamepad_get_description(int id) { return SDL_GameControllerNameForIndex(id); }

int gamepad_axis_count(int id) {
  SDL_GameController* c = SDL_GameControllerFromInstanceID(id);
  SDL_Joystick* j = SDL_GameControllerGetJoystick(c);
  return SDL_JoystickNumAxes(j);
}

double gamepad_axis_value(int id, int axis) {
  SDL_GameController* c = SDL_GameControllerFromInstanceID(id);

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

bool gamepad_button_check(int id, int btn) { return enigma::gamepads[id].buttonStatus[btn]; }

bool gamepad_button_check_pressed(int id, int btn) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= enigma::gamepads.size() || btn > gp_padr) return false;
#endif

  auto g = enigma::gamepads[id];

  if (btn == gp_any || btn == gp_none) {
    for (int i = 0; i < gp_axisrv; i++)
      if (g.buttonStatus[i] && !g.lastButtonStatus[i]) return (btn == gp_any);

    return !(btn == gp_any);
  }

  return g.buttonStatus[btn] && !g.lastButtonStatus[btn];
}

bool gamepad_button_check_released(int id, int btn) {
#ifdef DEBUG_MODE
  if (id < 0 || id >= enigma::gamepads.size() || btn > gp_padr) return false;
#endif

  auto g = enigma::gamepads[id];

  if (btn == gp_any || btn == gp_none) {
    for (int i = 0; i < gp_axisrv; i++)
      if (!g.buttonStatus[i] && g.lastButtonStatus[i]) return (btn == gp_any);

    return !(btn == gp_any);
  }

  return !g.buttonStatus[btn] && g.lastButtonStatus[btn];
}

int gamepad_button_count(int id) {
  SDL_GameController* c = SDL_GameControllerFromInstanceID(id);
  SDL_Joystick* j = SDL_GameControllerGetJoystick(c);
  return SDL_JoystickNumButtons(j);
}

double gamepad_button_value(int id, int btn) {
  //analog buttons are axes
  return gamepad_axis_value(id, btn);
}

//TODO: Implement these
/*void gamepad_set_vibration(int id, double left, double right)
{
  // https://wiki.libsdl.org/CategoryForceFeedback
  #ifdef DEBUG_MODE
  printf("Error: use of unimplemented function: gamepad_set_vibration\n");
  #endif
}

double gamepad_get_button_threshold(int id)
{
  #ifdef DEBUG_MODE
  printf("Error: use of unimplemented function: gamepad_get_button_threshold\n");
  #endif
  //GM Default
  return 0.5;
}

void gamepad_set_button_threshold(int id, double threshold)
{
  #ifdef DEBUG_MODE
  printf("Error: use of unimplemented function: gamepad_set_button_threshold\n");
  #endif
}

void gamepad_set_axis_deadzone(int id, double deadzone)
{
  #ifdef DEBUG_MODE
  printf("Error: use of unimplemented function:  gamepad_set_axis_deadzoned\n");
  #endif
}*/

//gamepad_set_colour is playstation 4 only so why bother?

}  // namespace enigma_user
