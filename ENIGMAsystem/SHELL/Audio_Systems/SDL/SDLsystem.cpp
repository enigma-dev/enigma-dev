#include <iostream>
#include <stdio.h>
#include <map>
#include <vector>
#include <SDL.h>
#include "Widget_Systems/widgets_mandatory.h"
#include "Bridges/Win32/WINDOWShandle.h"
#include "SDLsystem.h"
#include "SoundResource.h"

AssetArray<Sound> sounds;
namespace enigma {
int audiosystem_initialize() {
  SDL_Init(SDL_INIT_AUDIO);
  int flags=MIX_INIT_OGG|MIX_INIT_MOD;
  int initted=Mix_Init(flags);
  if(initted&flags != flags) {
    DEBUG_MESSAGE(std::string("Mix_Init: %s\n")+ std::string(Mix_GetError()),MESSAGE_TYPE::M_ERROR);
  }
  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) { return 0; }
  return 1;  
}

void audiosystem_update(void) {}

void audiosystem_cleanup() {
  Mix_CloseAudio();
  Mix_Quit();
}

int sound_add_from_buffer(int id, void* buffer, size_t bufsize) {
  Sound snd;
  snd.mc=Mix_QuickLoad_WAV(reinterpret_cast<Uint8*>((char*)buffer));
  DEBUG_MESSAGE( Mix_GetError(), MESSAGE_TYPE::M_ERROR);
  sounds.assign(id, std::move(snd));
  return 0;
}

}
