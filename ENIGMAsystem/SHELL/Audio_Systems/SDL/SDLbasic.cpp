#include "SDLsystem.h"
#include "SoundResource.h"
#include "../General/ASbasic.h"
#include "../General/ASadvanced.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "../General/ASutil.h"
#include <SDL_mixer.h>
#include <string>
#include <stdio.h>

namespace enigma_user {

bool sound_exists(int sound) { return sounds.exists(sound); }

int sound_add(string fname, int kind, bool preload) {
  return audio_add(fname);
}

void sound_delete(int sound) {
  sound_stop(sound);
  sounds.destroy(sound);
}

bool sound_play(int sound) {
  const Sound& snd = sounds.get(sound);
  if (Mix_PlayChannel(-1,snd.mc, 0) == -1) { return false; }
  return true;
}

bool sound_loop(int sound) {
  const Sound& snd = sounds.get(sound);
  if (Mix_PlayChannel(-1,snd.mc, -1) == -1) { return false; }
  return true;
}

void sound_stop(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc) {
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
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc && Mix_Playing(i) == 1) {
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
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc && Mix_Paused(i) == 1) {
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
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc && Mix_Playing(i) == 1) { return true; }
  }
  return false;
}

bool sound_ispaused(int sound) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc && Mix_Paused(i) == 1) { return true; }
  }
  return false;
}

void sound_pan(int sound, float value) {
  const Sound& snd = sounds.get(sound);
  int channel_count = Mix_AllocateChannels(-1);
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == snd.mc) {
      Mix_SetPanning(i, (Uint8)(value*254), (Uint8)(254 - (value * 254)));
    }
  }
}
float sound_get_volume(int sound) {
  const Sound& snd = sounds.get(sound);
  return (float)snd.mc->volume / MIX_MAX_VOLUME;
}

float sound_get_length(int sound) {
  const Sound& snd = sounds.get(sound);
  Uint32 points = 0;
  Uint32 frames = 0;
  int freq = 0;
  Uint16 fmt = 0;
  int chans = 0;
  if (!Mix_QuerySpec(&freq, &fmt, &chans)) { return 0; }
  points = (snd.mc->alen / ((fmt & 0xFF) / 8));
  frames = (points / chans);
  return ((frames * 1000) / freq);
}

void sound_volume(int sound, float value) {
  const Sound& snd = sounds.get(sound);
  Mix_VolumeChunk(snd.mc,(int)(value * MIX_MAX_VOLUME));
}

void sound_global_volume(float mastervolume) {
  Mix_Volume(-1, (int)(mastervolume * MIX_MAX_VOLUME));
}

void sound_3d_set_sound_position(int snd, float x, float y, float z) {
  Sound& sound = sounds.get(snd);
  sound.X = x;
  sound.Y = y;
  sound.Z = z;
  int channel_count = Mix_AllocateChannels(-1);
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == sound.mc) {
      sound.update(i);
    }
  }
}

void sound_3d_set_sound_distance(int snd, float mindist, float maxdist) {
  Sound& sound = sounds.get(snd);
  sound.minD = mindist;
  sound.maxD = maxdist;
  int channel_count = Mix_AllocateChannels(-1);
  for(int i = 0;i <= channel_count ; i++) {
    if (Mix_GetChunk(i) == sound.mc) {
      sound.update(i);
    }
  }
}

}
