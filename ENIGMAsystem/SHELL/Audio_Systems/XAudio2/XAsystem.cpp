/** Copyright (C) 2013 Robert B. Colton
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

#include <stdio.h>

#include "XAsystem.h"
#include "SoundEmitter.h"
#include "SoundChannel.h"

#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

int falloff_model = 0;
size_t channel_num = 128;

float listenerPos[] = {0.0f,0.0f,0.0f};
float listenerVel[] = {0.0f,0.0f,0.0f};
float listenerOri[] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};

// first one is reserved for music
vector<SoundChannel*> sound_channels(1);
vector<SoundResource*> sound_resources(0);
vector<SoundEmitter*> sound_emitters(0);

namespace enigma {

  int get_free_channel(double priority)
  {

  }

  void eos_callback(void *soundID, unsigned src)
  {

  }

  int audiosystem_initialize()
  {

  }

  SoundResource* sound_new_with_source() {

  }

  int sound_add_from_buffer(int id, void* buffer, size_t bufsize)
  {

  }


  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata)
  {

  }

  int sound_allocate()
  {

  }

  void audiosystem_update(void)
  {

  }

  void audiosystem_cleanup()
  {

  }

}
