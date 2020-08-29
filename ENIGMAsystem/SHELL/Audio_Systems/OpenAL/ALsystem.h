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

// We don't want to load ALURE from a DLL. Would be kind of a waste.
#define ALURE_STATIC_LIBRARY 1

#ifndef ENIGMA_AL_SYSTEM_H
#define ENIGMA_AL_SYSTEM_H

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"  // show_error
#endif

#include <string>
#include <stddef.h>

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

extern bool load_al_dll();
extern size_t channel_num;

extern ALfloat listenerPos[];
extern ALfloat listenerVel[];
extern ALfloat listenerOri[];

#include <list>
using std::list;
extern list<ALuint> garbageBuffers;

#include "SoundResource.h"

namespace enigma {
extern size_t sound_idmax;

int get_free_channel(double priority);

#ifdef DEBUG_MODE
#define get_sound(snd, id, failure)                                                          \
  if (id < 0 or sound_resources.find(id) == sound_resources.end() or !sound_resources[id]) { \
    DEBUG_MESSAGE("Sound " + enigma_user::toString(id) + " does not exist", MESSAGE_TYPE::M_USER_ERROR);\
    return failure;                                                                          \
  }                                                                                          \
  SoundResource *const snd = sound_resources[id];
#define get_soundv(snd, id)                                                                  \
  if (id < 0 or sound_resources.find(id) == sound_resources.end() or !sound_resources[id]) { \
    DEBUG_MESSAGE("Sound " + enigma_user::toString(id) + " does not exist", MESSAGE_TYPE::M_USER_ERROR);\
    return;                                                                                  \
  }                                                                                          \
  SoundResource *const snd = sound_resources[id];
#else
#define get_sound(snd, id, failure) \
  if (id < 0) return failure;       \
  SoundResource *const snd = sound_resources[id];
#define get_soundv(snd, id) \
  if (id < 0) return;       \
  SoundResource *const snd = sound_resources[id];
#endif

void eos_callback(void *soundID, ALuint src);
int audiosystem_initialize();
SoundResource *sound_new_with_source();
int sound_add_from_buffer(int id, void *buffer, size_t bufsize);
int sound_add_from_file(int id, std::string fname);
int sound_replace_from_file(int id, std::string fname);
int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size),
                          void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata),
                          void *userdata);
int sound_allocate();
void audiosystem_update(void);
void audiosystem_cleanup();
}  // namespace enigma

#endif
