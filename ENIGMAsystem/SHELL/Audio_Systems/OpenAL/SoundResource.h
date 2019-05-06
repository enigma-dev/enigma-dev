/** Copyright (C) 2008-2013 Josh Ventura, Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
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

#ifndef ENIGMA_SOUND_RESOURCE_H
#define ENIGMA_SOUND_RESOURCE_H

#include "ALsystem.h"

#include <map>

enum load_state { LOADSTATE_NONE, LOADSTATE_INDICATED, LOADSTATE_COMPLETE };

struct SoundResource {
  ALuint buf[3];                                 // The buffer-id of the sound data
  alureStream *stream;                           // optional stream
  void (*cleanup)(void *userdata);               // optional cleanup callback for streams
  void *userdata;                                // optional userdata for streams
  void (*seek)(void *userdata, float position);  // optional seeking
  int kind;                                      //
  float volume;
  float pan;
  float pitch;

  load_state loaded;  // Degree to which this sound has been loaded successfully
  bool idle;          // True if this sound is not being used, false if playing or paused.
  bool playing;       // True if this sound is playing; not paused or idle.

  SoundResource() : stream(0), cleanup(0), userdata(0), seek(0), kind(0), loaded(LOADSTATE_NONE), idle(1), playing(0) {
    buf[0] = 0;
    buf[1] = 0;
    buf[2] = 0;
    volume = 1.0f;
    pan = 0.0f;
    pitch = 1.0f;
  }

  ~SoundResource() { garbageBuffers.push_back(buf[0]); }
};

extern std::map<int, SoundResource *> sound_resources;

#endif
