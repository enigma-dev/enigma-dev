#include "Platforms/General/PFwindow.h"

#include <SDL2/SDL.h>

namespace enigma_user {
  std::string clipboard_get_text()
  {
    return SDL_GetClipboardText();
  }

  bool clipboard_has_text()
  {
    return SDL_HasClipboardText();
  }

  void clipboard_set_text(std::string text)
  {
    SDL_SetClipboardText(text.c_str());
  }
}
