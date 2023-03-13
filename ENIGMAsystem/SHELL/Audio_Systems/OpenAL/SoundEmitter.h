/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
***
*** This file is a part of the ENIGMA Development Environment.
***
*** ENIGMA is free software: you can redistribute it and/or modify it under the
*** terms of the GNU General Public License as published by the Free Software
*** Foundation, version 3 of the license or any later version.
***
*** This application and its source code is distributed AS-IS, WITHOUT ANY
*** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
*** FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
*** details.
***
*** You should have received a copy of the GNU General Public License along
*** with this code. If not, see <http://www.gnu.org/licenses/>
**/

#ifndef ENIGMA_SOUND_EMITTER_H
#define ENIGMA_SOUND_EMITTER_H

#include "ALsystem.h"

#include <vector>
using std::vector;

struct SoundEmitter {
  ALfloat emitPos[3];
  ALfloat emitVel[3];
  ALfloat falloff[3];
  ALfloat pitch;
  ALfloat volume;
  vector<int> sound_tracks;
  SoundEmitter() {
    emitPos[0] = emitPos[1] = emitPos[2] = 0.0f, emitVel[0] = emitVel[1] = emitVel[2] = 0.0f, falloff[0] = 100.0f,
    falloff[1] = 300.0f, falloff[2] = 1.0f, volume = 1.0f;
  }
};

extern vector<SoundEmitter*> sound_emitters;
#endif
