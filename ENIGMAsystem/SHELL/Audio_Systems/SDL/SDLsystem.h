#ifndef ENIGMA_SDL_SYSTEM_H
#define ENIGMA_SDL_SYSTEM_H

#include <cstddef>
#include <list>
#include <string>
#include <SDL_mixer.h>
#include "SoundResource.h"

namespace enigma {
int sound_add_from_buffer(int, void *, unsigned long long);
int audiosystem_initialize();
void audiosystem_update(void);
void audiosystem_cleanup();

}

#endif
