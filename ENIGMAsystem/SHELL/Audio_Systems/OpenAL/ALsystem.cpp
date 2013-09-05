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
#include "SoundChannel.h"
#include "SoundResource.h"
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
vector<SoundChannel*> sound_channels(1);
vector<SoundResource*> sound_resources(0);
vector<SoundEmitter*> sound_emitters(0);

#include "Widget_Systems/widgets_mandatory.h" // show_error

namespace enigma {

  int get_free_channel(double priority)
  {
    // test for channels not playing anything
    for(size_t i = 1; i < sound_channels.size(); i++)
    {
      ALint state;
      alGetSourcei(sound_channels[i]->source, AL_SOURCE_STATE, &state);
      if (state != AL_PLAYING)
      {
        return i;
      }
    }
    // finally if you still couldnt find an empty channel, and we have a few more we can generate
    // go ahead and generate a new one
    if (sound_channels.size() < channel_num)
    {
      int i = sound_channels.size();
      ALuint src;
      alGenSources(1, &src);
      SoundChannel* sndsrc = new SoundChannel(src,i);
      sound_channels.push_back(sndsrc);
      return i;
    }
    // test for channels playing a lower priority sound, and take their spot if they are
    for(size_t i = 1; i < sound_channels.size(); i++)
    {
      if (sound_channels[i]->priority < priority)
      {
        return i;
      }
    }

    return -1;
  }

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

    return 0;
  }

  SoundResource* sound_new_with_source() {
    SoundResource *res = new SoundResource();
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
    SoundResource *snd = sound_new_with_source();
	if (unsigned(id) > sound_resources.size()) {
		sound_resources.resize(id + 1);
	}
    sound_resources.insert(sound_resources.begin() + id, snd);

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
    SoundResource *snd = sound_new_with_source();
	if (unsigned(id) > sound_resources.size()) {
		sound_resources.resize(id + 1);
	}
	sound_resources.insert(sound_resources.begin() + id, snd);

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
	int id = -1;
	for (unsigned i = 0; i < sound_resources.size(); i++) {
		if (sound_resources[id] == NULL) {
			id = i;
		}
	}
	if (id < 0) {
	  id = sound_resources.size();
	}
	else if (unsigned(id) > sound_resources.size()) {
		sound_resources.resize(id + 1);
	}
	sound_resources.insert(sound_resources.begin() + id, new SoundResource());
	return id;
  }

  void audiosystem_update(void)
  {
    alureUpdate();

    elapsedtime = clock() - lasttime;
    lasttime = elapsedtime;
    // update all the sound channels so they can calculate fall off and gain
    for(size_t i = 1; i < sound_channels.size(); i++) {
      sound_channels[i]->sound_update();
    }
  }

  void audiosystem_cleanup()
  {
    for (size_t i = 0; i < sound_resources.size(); i++)
    if (sound_resources[i])
    {
      switch (sound_resources[i]->loaded)
      {
        case LOADSTATE_COMPLETE:
          alDeleteBuffers(sound_resources[i]->stream ? 3 : 1, sound_resources[i]->buf);
          if (sound_resources[i]->stream) {
            alureDestroyStream(sound_resources[i]->stream, 0, 0);
            if (sound_resources[i]->cleanup) sound_resources[i]->cleanup(sound_resources[i]->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          for(size_t j = 1; j < sound_channels.size(); j++) {
			alureStopSource(sound_channels[i]->source, true);
            alDeleteSources(1, &sound_channels[j]->source);
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
