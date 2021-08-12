#ifndef ENIGMA_SDL_SYSTEM_H
#define ENIGMA_SDL_SYSTEM_H

#include "SoundResource.h"
#include <SDL_mixer.h>

namespace enigma {
int sound_add_from_buffer(int, void *, unsigned long long);
int audiosystem_initialize();
void audiosystem_update(void);
void audiosystem_cleanup();

}

#endif
