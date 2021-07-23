#ifndef ENIGMA_SOUND_RESOURCE_H
#define ENIGMA_SOUND_RESOURCE_H

#include "Universal_System/Resources/AssetArray.h"
#include <SDL_mixer.h>
using enigma::AssetArray;


enum load_state { LOADSTATE_NONE, LOADSTATE_INDICATED, LOADSTATE_COMPLETE };

struct Sound {
    Mix_Chunk *mc;
    Mix_Music *mm;
  
  static const char* getAssetTypeName() { return "sound"; }
   bool isDestroyed() const { return !mc; }
   void destroy() {}
};

extern AssetArray<Sound> sounds;

#endif
