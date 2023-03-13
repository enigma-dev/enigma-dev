#ifndef ENIGMA_SOUND_RESOURCE_H
#define ENIGMA_SOUND_RESOURCE_H

#include "Universal_System/Resources/AssetArray.h"
#include "Universal_System/mathnc.h"
#include <SDL_mixer.h>
using enigma::AssetArray;


enum load_state { LOADSTATE_NONE, LOADSTATE_INDICATED, LOADSTATE_COMPLETE };

struct Sound {
  Mix_Chunk *mc;
  Mix_Music *mm;
  float X = 0, Y = 0, Z = 0 , minD = 1, maxD = 1000000000;
  
  static const char* getAssetTypeName() { return "sound"; }

  const void update(int channel) {
    float dist3d = enigma_user::point_distance_3d(0,0,0,X,Y,Z);
    if (dist3d >= maxD) { dist3d = maxD; }
    else if (dist3d <= minD) { dist3d = minD; }
    dist3d = 0 + (dist3d - minD) * (255 / (maxD - minD));
    Mix_SetPosition(channel ,(Sint16)(enigma_user::point_direction(0,0,X,Y)),(Uint8)dist3d);
  }

  bool isDestroyed() const { return !mc; }
  
  void destroy() {
    if (!isDestroyed()) {
      Mix_FreeChunk(mc);
    }
  }
  
};

extern AssetArray<Sound> sounds;

#endif
