#ifndef SDL_Window_h
#define SDL_Window_h

#include "Platforms/General/PFwindow.h"

#include <SDL2/SDL.h>

#include <unordered_map>

namespace enigma
{
  extern SDL_Window* windowHandle;
  namespace keyboard {
    extern std::unordered_map<SDL_Keycode,int> keymap;
    extern std::unordered_map<SDL_Keycode, int> inverse_keymap;
  }
  void window_id_init();
} //namespace enigma

#endif
