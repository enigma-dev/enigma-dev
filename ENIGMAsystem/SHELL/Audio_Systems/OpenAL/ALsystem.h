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

#ifndef _AL_SYSTEM__H
#define _AL_SYSTEM__H
#include <stddef.h>

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

extern int falloff_model;

extern size_t channel_num;

extern ALfloat listenerPos[];
extern ALfloat listenerVel[];
extern ALfloat listenerOri[];

namespace enigma { 

  enum load_state {
    LOADSTATE_NONE,
    LOADSTATE_SOURCED,
    LOADSTATE_INDICATED,
    LOADSTATE_COMPLETE
  };
  struct sound
  {
    ALuint buf[3]; // The buffer-id of the sound data
    alureStream *stream; // optional stream
    void (*cleanup)(void *userdata); // optional cleanup callback for streams
    void *userdata; // optional userdata for streams
    void (*seek)(void *userdata, float position); // optional seeking
    int type; //0 for sound, 1 for music, -1 for error
    int kind; //

    load_state loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.

    sound(): stream(0), cleanup(0), userdata(0), seek(0), type(0), kind(0), loaded(LOADSTATE_NONE), idle(1), playing(0) {
      buf[0] = 0; buf[1] = 0; buf[2] = 0;
    }
  };

  int get_free_channel(double priority);

  extern sound **sounds;
  extern size_t sound_idmax;

  #ifdef DEBUG_MODE
    #define get_sound(snd,id,failure)\
      if (id < 0 or size_t(id) >= enigma::sound_idmax or !enigma::sounds[id]) {\
        show_error("Sound " + toString(id) + " does not exist", false);\
        return failure;\
      } enigma::sound *const snd = enigma::sounds[id];
  #else
    #define get_sound(snd,id,failure)\
      enigma::sound *const snd = enigma::sounds[id];
  #endif

  void eos_callback(void *soundID, ALuint src);
  int audiosystem_initialize();
  sound* sound_new_with_source();
  int sound_add_from_buffer(int id, void* buffer, size_t bufsize);
  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata);
  int sound_allocate();
  void audiosystem_update(void);
  void audiosystem_cleanup();
}

#endif
