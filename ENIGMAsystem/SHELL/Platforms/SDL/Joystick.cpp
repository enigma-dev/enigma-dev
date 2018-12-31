#include "SDL2/SDL.h"
#include "Joystick.h"
#include "Platforms/General/PFjoystick.h"
#include <string>

namespace {

SDL_Joystick *joystick1;
SDL_Joystick *joystick2;
bool update = true;

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
  int axes = 0;
  if (SDL_JoystickNumAxes(joystick_get_handle(id)) >= 0)
    axes = SDL_JoystickNumAxes(joystick_get_handle(id));
  return axes;
}

int joystick_buttons(int id) {
  int buttons = 0;
  if (SDL_JoystickNumButtons(joystick_get_handle(id)) >= 0)
    buttons = SDL_JoystickNumButtons(joystick_get_handle(id));
  return buttons;
}

bool joystick_button(int id, int numb) {
  return SDL_JoystickGetButton(joystick_get_handle(id), numb - 1);
}

double joystick_axis(int id, int numb) {
  double axis;
  double pos = 0;
  axis = SDL_JoystickGetAxis(joystick_get_handle(id), axis - 1);
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
  joystick1 = SDL_JoystickOpen(0);
  joystick2 = SDL_JoystickOpen(1);
  return init;
}

void joystick_uninit() {
  SDL_JoystickClose(joystick1);
  SDL_JoystickClose(joystick2);
  SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
}
  
void joystick_update() {     
  if (enigma_user::joystick_exists(1) || enigma_user::joystick_exists(2))
    update = true;
  else if (update) {
    joystick_uninit();
    if (joystick_init())
      update = false;
  }
  if (update)
    SDL_JoystickUpdate();
}
  
} // namespace enigma
