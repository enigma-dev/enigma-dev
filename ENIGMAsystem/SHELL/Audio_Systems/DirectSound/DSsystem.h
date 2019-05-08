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

#ifndef ENIGMA_DS_SYSTEM_H
#define ENIGMA_DS_SYSTEM_H

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"  // show_error
#include "libEGMstd.h"
#endif

#include <dsound.h>
#include <mmsystem.h>
#include <stddef.h>
#include <windows.h>

extern IDirectSoundBuffer *primaryBuffer;

#include "SoundResource.h"

namespace enigma {

int get_free_channel(double priority);

#ifdef DEBUG_MODE
#define get_sound(snd, id, failure)                                                           \
  if (id < 0 or size_t(id) >= sound_resources.size() or !sound_resources[id]) {               \
    enigma_user::show_error("Sound " + enigma_user::toString(id) + " does not exist", false); \
    return failure;                                                                           \
  }                                                                                           \
  SoundResource *const snd = sound_resources[id];
#define get_soundv(snd, id)                                                                   \
  if (id < 0 or size_t(id) >= sound_resources.size() or !sound_resources[id]) {               \
    enigma_user::show_error("Sound " + enigma_user::toString(id) + " does not exist", false); \
    return;                                                                                   \
  }                                                                                           \
  SoundResource *const snd = sound_resources[id];
#else
#define get_sound(snd, id, failure) SoundResource *const snd = sound_resources[id];
#define get_soundv(snd, id) SoundResource *const snd = sound_resources[id];
#endif

void eos_callback(void *soundID, unsigned src);
int audiosystem_initialize();
SoundResource *sound_new_with_source();
int sound_add_from_buffer(int id, void *buffer, size_t bufsize);
int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size),
                          void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata),
                          void *userdata);
int sound_allocate();
void audiosystem_update(void);
void audiosystem_cleanup();

}  // namespace enigma

#endif
