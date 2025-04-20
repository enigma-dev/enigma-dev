#ifndef ENIGMA_SOUND_CHANNEL_H
#define ENIGMA_SOUND_CHANNEL_H

#include <vector>
#include <SDL_mixer.h>
using std::vector;

struct SoundChannel {
  Mix_Chunk *mchunk;
  int soundIndex;
  double priority;
};

extern vector<SoundChannel*> sound_channels;

#endif
