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

#include <stdio.h>

#include "ALsystem.h"
#include "SoundInstance.h"
#include "SoundEmitter.h"

#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

int falloff_model = 0;
bool load_al_dll();
size_t channel_num = 128;

ALfloat listenerPos[] = {0.0f,0.0f,0.0f};
ALfloat listenerVel[] = {0.0f,0.0f,0.0f};
ALfloat listenerOri[] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};

// first one is reserved for music
vector<sound_instance*> sound_sources(1);
vector<soundEmitter*> sound_emitters(0);

namespace enigma {

    int get_free_channel(double priority)
    {
    // test for channels not playing anything
    for(size_t i = 1; i < sound_sources.size(); i++)
    {
      ALint state;
      alGetSourcei(sound_sources[i]->source, AL_SOURCE_STATE, &state);
      if (state != AL_PLAYING)
      {
        return i;
      }
    }
    // finally if you still couldnt find an empty channel, and we have a few more we can generate
    // go ahead and generate a new one
    if (sound_sources.size() < channel_num)
    {
      int i = sound_sources.size();
      ALuint src;
      alGenSources(1, &src);
      //sound_sources.resize(i + 1);
      //sound_sources[i]->source = src;
      sound_instance* sndinst = new sound_instance(src,i);
      sound_sources.push_back(sndinst);
      return i;
    }
    // test for channels playing a lower priority sound, and take their spot if they are
    for(size_t i = 1; i < sound_sources.size(); i++)
    {
      if (sound_sources[i]->priority < priority)
      {
        return i;
      }
    }
  
    return -1;
  }

  sound **sounds;

  void eos_callback(void *soundID, ALuint src)
  {
    get_sound(snd, (ptrdiff_t)soundID, );
    snd->playing = false;
    snd->idle = true;
  }

  int audiosystem_initialize()
  {
    starttime = clock();
    elapsedtime = starttime;
    lasttime = elapsedtime;
    printf("Initializing audio system...\n");
    if (sound_idmax == 0)
      sounds = NULL;
    else
      sounds = new sound*[sound_idmax];

    #ifdef _WIN32
    if (!load_al_dll())
      return 1;
	printf("Starting ALURE (Windows thing).\n");
	init_alure();
    #endif

	printf("Opening ALURE devices.\n");
    if(!alureInitDevice(NULL, NULL)) {
      fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
      return 1;
    }

    for (size_t i = 0; i < sound_idmax; i++)
      sounds[i] = NULL;

    return 0;
  }

  sound* sound_new_with_source() {
    sound *res = new sound();
    alGetError();
    int a = alGetError();
    if(a != AL_NO_ERROR) {
      fprintf(stderr, "Failed to create OpenAL source! Error %d: %s\n",a,alGetString(a));
      printf("%d %d %d %d %d\n",AL_INVALID_NAME,AL_INVALID_ENUM,AL_INVALID_VALUE,AL_INVALID_OPERATION,AL_OUT_OF_MEMORY);
      delete res;
      return NULL;
    }
    res->loaded = LOADSTATE_SOURCED;
    return res;
  }

  int sound_add_from_buffer(int id, void* buffer, size_t bufsize)
  {
    sound *snd = sounds[id];
    if (!snd)
      snd = sounds[id] = sound_new_with_source();
    if (snd->loaded != LOADSTATE_SOURCED) {
      fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 1;
    }

    ALuint& buf = snd->buf[0];
    buf = alureCreateBufferFromMemory((ALubyte*)buffer, bufsize);

    if(!buf) {
      fprintf(stderr, "Could not load sound %d: %s\n", id, alureGetErrorString());
      return 2;
    }

    snd->loaded = LOADSTATE_COMPLETE;
    return 0;
  }


  int sound_add_from_stream(int id, size_t (*callback)(void *userdata, void *buffer, size_t size), void (*seek)(void *userdata, float position), void (*cleanup)(void *userdata), void *userdata)
  {
    sound *snd = sounds[id];
    if (!snd)
      snd = sounds[id] = sound_new_with_source();
    if (snd->loaded != LOADSTATE_SOURCED)
      return 1;

    snd->stream = alureCreateStreamFromCallback((ALuint (*)(void*, ALubyte*, ALuint))callback, userdata, AL_FORMAT_STEREO16, 44100, 4096, 0, NULL);
    if (!snd->stream) {
      fprintf(stderr, "Could not create stream %d: %s\n", id, alureGetErrorString());
      return 1;
    }
    snd->cleanup = cleanup;
    snd->userdata = userdata;
    snd->seek = seek;

    snd->loaded = LOADSTATE_COMPLETE;
    return 0;
  }

  int sound_allocate()
  {
    // Make room for sound
    sound **nsounds = new sound*[sound_idmax+1];
    for (size_t i = 0; i < sound_idmax; i++)
      nsounds[i] = sounds[i];
    nsounds[sound_idmax] = sound_new_with_source();
    delete[] sounds;
    sounds = nsounds;
    return sound_idmax++;
  }

  void audiosystem_update(void)
  {
    alureUpdate();

    elapsedtime = clock() - lasttime;
    lasttime = elapsedtime;
    // update all the sounds so they can calculate fall off and gain
    for(size_t i = 1; i < sound_sources.size(); i++) {
      sound_sources[i]->sound_update();
    }
  }

  void audiosystem_cleanup()
  {
    for (size_t i = 0; i < sound_idmax; i++)
    if (sounds[i])
    {
      switch (sounds[i]->loaded)
      {
        case LOADSTATE_COMPLETE:
          alDeleteBuffers(sounds[i]->stream ? 3 : 1, sounds[i]->buf);
          if (sounds[i]->stream) {
            alureStopSource(sound_sources[i]->source, true);
            alureDestroyStream(sounds[i]->stream, 0, 0);
            if (sounds[i]->cleanup) sounds[i]->cleanup(sounds[i]->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          for(size_t j = 1; j < sound_sources.size(); j++) {
            alDeleteSources(1, &sound_sources[j]->source);
          }
          break;
        
        case LOADSTATE_INDICATED:
        case LOADSTATE_NONE:
        default: ;
      }
    }

    alureShutdownDevice();
  }

}
