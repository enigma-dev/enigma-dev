#include "SDL2/SDL.h"
#include "Platforms/General/PFjoystick.h"
#include <string>

SDL_Joystick *joystick1;
SDL_Joystick *joystick2;

bool update = true;

namespace enigma_user {

  bool joystick_exists(int id) {

    bool exists;

    if (id - 1 < 0.5)
      exists = (SDL_JoystickGetAttached(joystick1) == SDL_TRUE);
    else
      exists = (SDL_JoystickGetAttached(joystick2) == SDL_TRUE);

    return exists;

  }

  std::string joystick_name(int id) {
    
    enigma::joystick_update();

    const char *name;

    if (id - 1 < 0.5)
      name = SDL_JoystickName(joystick1);
    else
      name = SDL_JoystickName(joystick2);

    return name;

  }

  int joystick_axes(int id) {
    
    enigma::joystick_update();

    int axes = 0;

    if (id - 1 < 0.5) {

      if (SDL_JoystickNumAxes(joystick1) >= 0)
        axes = SDL_JoystickNumAxes(joystick1);

    } else {

      if (SDL_JoystickNumAxes(joystick2) >= 0)
        axes = SDL_JoystickNumAxes(joystick2);

    }

    return axes;

  }

  int joystick_buttons(int id) {
    
    enigma::joystick_update();

    int buttons = 0;

    if (id - 1 < 0.5) {

      if (SDL_JoystickNumButtons(joystick1) >= 0)
        buttons = SDL_JoystickNumButtons(joystick1);

    } else {

      if (SDL_JoystickNumButtons(joystick2) >= 0)
        buttons = SDL_JoystickNumButtons(joystick2);

    }

    return buttons;

  }

  bool joystick_button(int id, int numb) {
    
    enigma::joystick_update();

    bool button;

    if (id - 1 < 0.5)
      button = SDL_JoystickGetButton(joystick1, numb - 1);
    else
      button = SDL_JoystickGetButton(joystick2, numb - 1);

    return button;

  }

  double joystick_axes(int id, int numb) {
    
    enigma::joystick_update();

    double axis;
    double pos = 0;

    if (id - 1 < 0.5)
      axis = SDL_JoystickGetAxis(joystick1, axis - 1);
    else
      axis = SDL_JoystickGetAxis(joystick2, axis - 1);

    if (axis != -256 && axis != 0) // don't ask why this shit is necessary; it just is...
      pos = (axis / 32768);
    else if (joystick_exists(id))
      pos = -0.01;

    return pos;

  }

}

namespace enigma {

  bool joystick_init() {

    int init;
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
  
}
