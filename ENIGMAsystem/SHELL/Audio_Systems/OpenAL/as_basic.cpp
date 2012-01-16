/** Copyright (C) 2008-2011 Josh Ventura
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

#include <string>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
using std::string;
#include "as_basic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "alure/include/AL/alure.h"

#ifdef DEBUG_MODE
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#endif

namespace enigma
{
  enum load_state {
    LOADSTATE_NONE,
    LOADSTATE_SOURCED,
    LOADSTATE_INDICATED,
    LOADSTATE_COMPLETE
  };
  struct sound
  {
    ALuint src, buf[3]; // The source-id and buffer-id of the sound data
    alureStream *stream; // optional stream
    void (*cleanup)(void *userdata); // optional cleanup callback for streams
    void *userdata; // optional userdata for streams
    void (*seek)(void *userdata, float position); // optional seeking shits

    load_state loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.

    sound(): src(0), stream(0), cleanup(0), userdata(0), seek(0), loaded(LOADSTATE_NONE), idle(1), playing(0) {
      buf[0] = 0; buf[1] = 0; buf[2] = 0;
    }
  };
  
  sound **sounds;
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

  static void eos_callback(void *soundID, ALuint src)
  {
    get_sound(snd, (ptrdiff_t)soundID, );
    snd->playing = false;
    snd->idle = true;
  }

  int audiosystem_initialize()
  {
    if (sound_idmax == 0)
      sounds = NULL;
    else
      sounds = new sound*[sound_idmax];
    
    #ifdef _WIN32
    if (!load_al_dll())
      return 1;
    #endif
    
    if(!alureInitDevice(NULL, NULL)) {
      fprintf(stderr, "Failed to open OpenAL device: %s\n", alureGetErrorString());
      return 1;
    }
    
    for (size_t i = 0; i < sound_idmax; i++)
      sounds[i] = NULL;
    
    return 0;
  }
  
  static sound* sound_new_with_source() {
    sound *res = new sound();
    alGetError();
    alGenSources(1, &res->src);
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
    alSourcei(snd->src, AL_BUFFER, buf);
    alSourcei(snd->src, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(snd->src, AL_REFERENCE_DISTANCE, 1);
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
  
  void audiosystem_update(void) { alureUpdate(); }
  
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
            alureStopSource(sounds[i]->src, true);
            alureDestroyStream(sounds[i]->stream, 0, 0);
            if (sounds[i]->cleanup) sounds[i]->cleanup(sounds[i]->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          alDeleteSources(1, &sounds[i]->src);
          break;
        default: ;
      }
    }
    
    alureShutdownDevice();
  }
};

bool sound_play(int sound) // Returns whether sound is playing
{
  get_sound(snd,sound,0); //snd.looping = false;
  alSourcei(snd->src, AL_LOOPING, AL_FALSE); //Just playing
  return !(snd->idle = !(snd->playing = !snd->stream ?
    alurePlaySource(snd->src, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
    alurePlaySourceStream(snd->src, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}
bool sound_loop(int sound) // Returns whether sound is playing
{
  get_sound(snd,sound,0);
  alSourcei(snd->src, AL_LOOPING, AL_TRUE); // Toggle on looping
  return snd->idle = !(snd->playing = (alurePlaySource(snd->src, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}
bool sound_pause(int sound) // Returns whether the sound is still playing
{
  get_sound(snd,sound,0);
  return snd->playing = !alurePauseSource(snd->src);
}
void sound_pause_all() 
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && enigma::sounds[i]->src && enigma::sounds[i]->playing)
      enigma::sounds[i]->playing = !alurePauseSource(enigma::sounds[i]->src);
  }
}
void sound_stop(int sound) {
  get_sound(snd,sound,);
  alureStopSource(snd->src,AL_FALSE);
  if (snd->seek) snd->seek(snd->userdata, 0);
}
void sound_stop_all() 
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    alureStopSource(enigma::sounds[i]->src,AL_FALSE);
    if (enigma::sounds[i]->seek) enigma::sounds[i]->seek(enigma::sounds[i]->userdata,0);
  }
}
void sound_delete(int sound) {
  get_sound(snd,sound,);
  alureDestroyStream(snd->stream, 0, 0);
  alDeleteSources(1,&snd->src);
}
void sound_volume(int sound, float volume) {
    get_sound(snd,sound,);
    alSourcef(snd->src, AL_GAIN, volume);
}
void sound_global_volume(float mastervolume) {
    alListenerf(AL_GAIN, mastervolume);
}
bool sound_resume(int sound) // Returns whether the sound is playing
{
  get_sound(snd,sound,false);
  return snd->playing = alureResumeSource(snd->src);
}
void sound_resume_all() 
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && enigma::sounds[i]->src && !enigma::sounds[i]->playing)
      enigma::sounds[i]->playing = alureResumeSource(enigma::sounds[i]->src);
  }
}
bool sound_isplaying(int sound) {
  get_sound(snd,sound,false);
  return snd->playing;
}
bool sound_ispaused(int sound) {
  return !enigma::sounds[sound]->idle and !enigma::sounds[sound]->playing;
}

void sound_pan(int sound, float value)
{
  get_sound(snd,sound,);
  const float pan = value*2-1;
  alSource3f(snd->src,AL_POSITION,pan,sqrt(1-pan*pan),0);
}
float sound_get_length(int sound) { // Not for Streams
  get_sound(snd,sound,0);
  ALint size, bits, channels, freq;

  alGetBufferi(snd->buf[0], AL_SIZE, &size);
  alGetBufferi(snd->buf[0], AL_BITS, &bits);
  alGetBufferi(snd->buf[0], AL_CHANNELS, &channels);
  alGetBufferi(snd->buf[0], AL_FREQUENCY, &freq);

  return size / channels / (bits/8) / (float)freq;
}
float sound_get_position(int sound) { // Not for Streams
  get_sound(snd,sound,-1);
  float offset;
  
  alGetSourcef(snd->src, AL_SEC_OFFSET, &offset);
  return offset;
}
void sound_seek(int sound, float position) {
  get_sound(snd,sound,);
  alSourcef(snd->src, AL_SEC_OFFSET, position); // Non Streams 
  if (snd->seek) snd->seek(snd->userdata, position); // Streams
}
void sound_seek_all(float position) {
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && enigma::sounds[i]->src) {
      alSourcef(enigma::sounds[i]->src, AL_SEC_OFFSET, position); // Non Streams
      if (enigma::sounds[i]->seek) enigma::sounds[i]->seek(enigma::sounds[i]->userdata, position); // Streams
    }
  }
}

const char* sound_get_audio_error() {
  return alureGetErrorString();
}

#include <string>
using namespace std;
extern void show_message(string);
int sound_add(string fname, int kind, bool preload) //At the moment, the latter two arguments do nothing! =D
{
  // Open sound
  FILE *afile = fopen(fname.c_str(),"rb");
  if (!afile)
    return -1;
  
  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  char fdata[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");
  
  // Decode sound
  int rid = enigma::sound_allocate();
  if (enigma::sound_add_from_buffer(rid,fdata,flen))
    return (--enigma::sound_idmax, -1);
  
  return rid;
}
