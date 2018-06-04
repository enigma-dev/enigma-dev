#ifndef E_SDL_CLIPBOARD_H
#define E_SDL_CLIPBOARD_H

#include <string>

std::string clipboard_get_text();
bool clipboard_has_text();
void clipboard_set_text(std::string text);

#endif