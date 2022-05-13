#include "SDLsystem.h"
#include "SoundResource.h"
#include "Widget_Systems/widgets_mandatory.h"
#include <SDL.h>
#include <map>
#include <vector>
#include <stdio.h>

AssetArray<Sound> sounds;
namespace enigma {
int audiosystem_initialize() {
  SDL_Init(SDL_INIT_AUDIO);
  int flags = MIX_INIT_OGG|MIX_INIT_MOD|MIX_INIT_FLAC|MIX_INIT_MP3;
  int initted = Mix_Init(flags);
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) { return 0; }
  return 1;
}

void audiosystem_update(void) {}

void audiosystem_cleanup() {
  Mix_CloseAudio();
  Mix_Quit();
}

int sound_add_from_buffer(int id, void* buffer, size_t bufsize) {
  Sound snd;
  SDL_RWops* rw = SDL_RWFromMem(buffer, bufsize);
  snd.mc = Mix_LoadWAV_RW(rw, true);
  if (!snd.mc) {
    DEBUG_MESSAGE(std::string("Failed to load the sample for id -") + std::to_string(id), MESSAGE_TYPE::M_ERROR);
  }
  sounds.assign(id, std::move(snd));
  return 0;
}

}
