#ifndef SDL_Window_h
#define SDL_Window_h

#include "Platforms/General/PFwindow.h"

#include <SDL2/SDL.h>

#include <map>

namespace enigma
{
  extern SDL_Window* windowHandle;
  namespace keyboard {
    extern std::map<SDL_Keycode,int> keymap;
  }
} //namespace enigma

#endif
