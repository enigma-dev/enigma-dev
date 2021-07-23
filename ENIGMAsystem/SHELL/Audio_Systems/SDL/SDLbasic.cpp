#include <stdio.h>
#include <string>
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "../General/ASutil.h"
#include "Universal_System/estring.h"
#include "SoundResource.h"
#include <SDL_mixer.h>

namespace enigma_user {

bool sound_exists(int sound) { return sounds.exists(sound); }

int sound_add(string fname, int kind, bool preload) {

  // Open sound
  size_t flen = 0;
  char* fdata = enigma::read_all_bytes(fname, flen);
  if (!fdata) return -1;

  // Decode sound
  int rid = sounds.size();
  bool fail = enigma::sound_add_from_buffer(rid, fdata, flen);
  delete[] fdata;

  if (fail) return -1;
  return rid;	

}

bool sound_play(int sound) {
  const Sound& snd = sounds.get(sound);
  if(Mix_PlayChannel(-1,snd.mc, 0)==-1) { return false; }
  return true;
}

bool sound_loop(int sound) {
  const Sound& snd = sounds.get(sound);
  if(Mix_PlayChannel(-1,snd.mc, -1) == -1) { return false; }
  return true;
}

void sound_stop(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i=0;i<=channel_count;i++) {
    if(Mix_GetChunk(i) == snd.mc) {
      Mix_HaltChannel(i);
      }
  }
}

void sound_stop_all() {
  Mix_HaltChannel(-1);
}

bool sound_pause(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i=0;i<=channel_count;i++) {
    if(Mix_GetChunk(i) == snd.mc && Mix_Playing(i) == 1) {
      Mix_Pause(i);
	  return true;
      }
  }
  return false;
}

void sound_pause_all() {
  Mix_Pause(-1);
}

bool sound_resume(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i=0;i<=channel_count;i++) {
    if(Mix_GetChunk(i) == snd.mc && Mix_Paused(i) == 1) {
      Mix_Resume(i);
	  return true;
      }
  }
  return false;
}

void sound_resume_all() {
  Mix_Resume(-1);
}

bool sound_isplaying(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i=0;i<=channel_count;i++) {
    if(Mix_GetChunk(i) == snd.mc && Mix_Playing(i) == 1) { return true; }
  }
  return false;
}

bool sound_ispaused(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i=0;i<=channel_count;i++) {
    if(Mix_GetChunk(i) == snd.mc && Mix_Paused(i) == 1) { return true; }
  }
  return false;
}

float sound_get_volume(int sound) {
  const Sound& snd = sounds.get(sound);
  return (float)snd.mc->volume;
}

}
