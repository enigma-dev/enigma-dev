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

// We don't want to load ALURE from a DLL. Would be kind of a waste.
#define ALURE_STATIC_LIBRARY 1

#ifndef _AL_SYSTEM__H
#define _AL_SYSTEM__H
#include <stddef.h>

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

extern int falloff_model;
extern bool load_al_dll();
extern size_t channel_num;

extern ALfloat listenerPos[];
extern ALfloat listenerVel[];
extern ALfloat listenerOri[];

#include "SoundResource.h"

namespace enigma { 
  extern size_t sound_idmax;

  int get_free_channel(double priority);

  #ifdef DEBUG_MODE
    #define get_sound(snd,id,failure)\
      if (id < 0 or size_t(id) >= sound_resources.size() or !sound_resources[id]) {\
        show_error("Sound " + toString(id) + " does not exist", false);\
        return failure;\
      } SoundResource *const snd = sound_resources[id];
  #else
    #define get_sound(snd,id,failure)\
      SoundResource *const snd = sound_resources[id];
  #endif

  void eos_callback(void *soundID, ALuint src);
  int audiosystem_initialize();
  SoundResource* sound_new_with_source();
  int sound_add_from_buffer(int id, void* buffer, size_t bufsize);
  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata);
  int sound_allocate();
  void audiosystem_update(void);
  void audiosystem_cleanup();
}

#endif
