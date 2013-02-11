/** Copyright (C) 2008-2011 Josh Ventura, Robert B. Colton
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

#include <string>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
using std::string;
#include "as_basic.h"
#include "Audio_Systems/audio_mandatory.h"

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

bool load_al_dll();

#ifdef DEBUG_MODE
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h"
#endif

#include "Universal_System/estring.h"

#include <AL/al.h>
ALfloat listenerPos[]={0.0f,0.0f,0.0f};
ALfloat listenerVel[]={0.0f,0.0f,0.0f};
ALfloat listenerOri[]={0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
int channel_num = 128;
ALuint channel_sources[128];
int falloff_model = 0;
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
    ALuint buf[3]; // The buffer-idof the sound data
    alureStream *stream; // optional stream
    void (*cleanup)(void *userdata); // optional cleanup callback for streams
    void *userdata; // optional userdata for streams
    void (*seek)(void *userdata, float position); // optional seeking
    int type = 0; //0 for sound, 1 for music, -1 for error
    int kind = 0; //

    load_state loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.

    sound(): stream(0), cleanup(0), userdata(0), seek(0), loaded(LOADSTATE_NONE), idle(1), playing(0) {
      buf[0] = 0; buf[1] = 0; buf[2] = 0;
    }
  };

  struct soundEmitter
  {
    ALfloat emitPos[3];
    ALfloat emitVel[3];
    ALfloat pitch;
    soundEmitter()
    {
      ALfloat emitPos[3]={0.0f,0.0f,0.0f};
      ALfloat emitVel[3]={0.0f,0.0f,0.0f};
    }
  };

  sound **sounds;
  soundEmitter **soundEmitters;
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

  #ifdef DEBUG_MODE
    #define get_emitter(emit,id,failure)\
      if (id < 0 or !enigma::soundEmitters[id]) {\
        show_error("Emitter " + toString(id) + " does not exist", false);\
        return failure;\
      } enigma::soundEmitter *const emit = enigma::soundEmitters[id];
  #else
    #define get_emitter(emit,id,failure)\
      enigma::soundEmitter *const emit = enigma::soundEmitters[id];
  #endif

  static void eos_callback(void *soundID, ALuint src)
  {
    get_sound(snd, (ptrdiff_t)soundID, );
    snd->playing = false;
    snd->idle = true;
  }

  int audiosystem_initialize()
  {
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

    alGenSources(channel_num, channel_sources);
    fprintf(stderr, "Generating default %d audio listening channels.\n",channel_num);

    for (size_t i = 0; i < sound_idmax; i++)
      sounds[i] = NULL;

    return 0;
  }

  static sound* sound_new_with_source() {
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
            alureStopSource(channel_sources[0], true);
            alureDestroyStream(sounds[i]->stream, 0, 0);
            if (sounds[i]->cleanup) sounds[i]->cleanup(sounds[i]->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          alDeleteSources(1, channel_sources);
          break;
        default: ;
      }
    }

    alureShutdownDevice();
  }
};

// ***** OLD SOUND SYSTEM *****

bool sound_exists(int sound)
{
    return unsigned(sound) < enigma::sound_idmax && bool(enigma::sounds[sound]);
}

bool sound_play(int sound) // Returns whether sound is playing
{
  int src = -1;
  for(size_t i = 0; i < channel_num; i++) 
  {
    ALint state;
    alGetSourcei(channel_sources[i], AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    { 
      src = i;
    }
  }
  if (src == -1) 
  {
    // test for channels playing a lower priority
  }
  if (src != -1) 
  {
    get_sound(snd,sound,0); //snd.looping = false;
    alSourcei(channel_sources[src], AL_BUFFER, snd->buf[0]);
    alSourcei(channel_sources[src], AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(channel_sources[src], AL_REFERENCE_DISTANCE, 1);
    alSourcei(channel_sources[src], AL_LOOPING, AL_FALSE); //Just playing
    return !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(channel_sources[src], enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(channel_sources[src], snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
  } 
  else 
  {
    return -1;
  }
}
bool sound_loop(int sound) // Returns whether sound is playing
{
  int src = -1;
  for(size_t i = 0; i < channel_num; i++) 
  {
    ALint state;
    alGetSourcei(channel_sources[i], AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    { 
      src = i;
    }
  }
  if (src == -1) 
  {
    // test for channels playing a lower priority
  }
  if (src != -1) 
  {
    get_sound(snd,sound,0);
    alSourcei(channel_sources[src], AL_LOOPING, AL_TRUE); // Toggle on looping
    return snd->idle = !(snd->playing = (alurePlaySource(channel_sources[src], enigma::eos_callback, 
      (void*)(ptrdiff_t)sound) != AL_FALSE));
  }
  else 
  {
    return -1;
  }
}
bool sound_pause(int sound) // Returns whether the sound is still playing
{
  get_sound(snd,sound,0);
  return snd->playing = !alurePauseSource(channel_sources[0]);
}
void sound_pause_all()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && channel_sources[0] && enigma::sounds[i]->playing)
      enigma::sounds[i]->playing = !alurePauseSource(channel_sources[0]);
  }
}
void sound_stop(int sound) {
  get_sound(snd,sound,);
  alureStopSource(channel_sources[0], AL_FALSE);
  snd->playing = false;
  if (snd->seek)
    snd->seek(snd->userdata, 0);
}
void sound_stop_all()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    alureStopSource(channel_sources[0],AL_FALSE);
    enigma::sounds[i]->playing = false;
    if (enigma::sounds[i]->seek)
        enigma::sounds[i]->seek(enigma::sounds[i]->userdata,0);
  }
}
void sound_delete(int sound) {
  get_sound(snd,sound,);
  alureDestroyStream(snd->stream, 0, 0);
  alDeleteSources(channel_num, channel_sources);
  delete enigma::sounds[sound];
  enigma::sounds[sound] = NULL;
}
void sound_volume(int sound, float volume) {
    get_sound(snd,sound,);
    alSourcef(channel_sources[0], AL_GAIN, volume);
}
void sound_global_volume(float mastervolume) {
    alListenerf(AL_GAIN, mastervolume);
}
bool sound_resume(int sound) // Returns whether the sound is playing
{
  get_sound(snd,sound,false);
  return snd->playing = alureResumeSource(channel_sources[0]);
}
void sound_resume_all()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && channel_sources[0] && !enigma::sounds[i]->playing)
      enigma::sounds[i]->playing = alureResumeSource(channel_sources[0]);
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
  alSource3f(channel_sources[0],AL_POSITION,pan,sqrt(1-pan*pan),0);
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

  alGetSourcef(channel_sources[0], AL_SEC_OFFSET, &offset);
  return offset;
}
void sound_seek(int sound, float position) {
  get_sound(snd,sound,);
  alSourcef(channel_sources[0], AL_SEC_OFFSET, position); // Non Streams
  if (snd->seek) snd->seek(snd->userdata, position); // Streams
}
void sound_seek_all(float position) {
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i] && channel_sources[0]) {
      alSourcef(channel_sources[0], AL_SEC_OFFSET, position); // Non Streams
      if (enigma::sounds[i]->seek) enigma::sounds[i]->seek(enigma::sounds[i]->userdata, position); // Streams
    }
  }
}

void action_sound(int snd, bool loop)
{
    (loop ? sound_loop:sound_play)(snd);
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

bool sound_replace(int sound, string fname, int kind, bool preload)
{
  get_sound(snd,sound,false);
  alureDestroyStream(snd->stream, 0, 0);
  alDeleteSources(channel_num,channel_sources);
  enigma::sounds[sound] = enigma::sound_new_with_source();
  return true;
}

// ***** NEW SOUND SYSTEM *****
 
bool audio_exists(int sound)
{
  return unsigned(sound) < enigma::sound_idmax && bool(enigma::sounds[sound]);
}
bool audio_is_playing(int sound) {
  get_sound(snd,sound,false);
  return snd->playing;
}

void audio_play_music(int sound, bool loop) 
{
    // channel 0 is reserved for music, but sounds can play there if they want
    // but if music starts they get overridin, and you can only play 1 background
    // music at a time
    get_sound(snd,sound,0);
    alSourcei(channel_sources[0], AL_BUFFER, snd->buf[0]);
    alSourcei(channel_sources[0], AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(channel_sources[0], AL_REFERENCE_DISTANCE, 1);
    alSourcei(channel_sources[0], AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(channel_sources[0], enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(channel_sources[0], snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}

int audio_play_sound(int sound, int priority, bool loop) 
{
  int src = -1;
  for(size_t i = 0; i < channel_num; i++) 
  {
    ALint state;
    alGetSourcei(channel_sources[i], AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    { 
      src = i;
    }
  }
  if (src == -1) 
  {
    // test for channels playing a lower priority sound, and take their spot if they are
  }
  if (src != -1) 
  {
    get_sound(snd,sound,0);
    alSourcei(channel_sources[src], AL_BUFFER, snd->buf[0]);
    alSourcei(channel_sources[src], AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(channel_sources[src], AL_REFERENCE_DISTANCE, 1);
    alSourcei(channel_sources[src], AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(channel_sources[src], enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(channel_sources[src], snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
    return src;
  } 
  else 
  {
    return -1;
  }
}

int audio_play_sound_at(int sound, double x, double y, double z, int falloff_ref, double falloff_max, double falloff_factor, bool loop, double priority) 
{
  int src = -1;
  for(size_t i = 0; i < channel_num; i++) 
  {
    ALint state;
    alGetSourcei(channel_sources[i], AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    { 
      src = i;
    }
  }
  if (src == -1) 
  {
    // test for channels playing a lower priority
  }
  if (src != -1) 
  {
    get_sound(snd,sound,0);
    alSourcei(channel_sources[0], AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    ALfloat soundPos[]={0.0f,0.0f,0.0f};
    alSourcefv(channel_sources[0], AL_POSITION, soundPos);
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(channel_sources[0], enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(channel_sources[0], snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
    return src;
  } 
  else 
  {
    return -1;
  }
}

void audio_pause_sound(int index)
{
  alurePauseSource(channel_sources[index]);
}

void audio_resume_sound(int index)
{
  alureResumeSource(channel_sources[index]);
}

void audio_stop_sound(int index)
{
  alureStopSource(channel_sources[index], AL_TRUE);
}

void audio_stop_all()
{
  for(size_t i = 0; i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 0) {
      alureStopSource(channel_sources[0],AL_FALSE);
      enigma::sounds[i]->playing = false;
      if (enigma::sounds[i]->seek)
          enigma::sounds[i]->seek(enigma::sounds[i]->userdata,0);
    }
  }
}

void audio_stop_music()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 1) {
      alureStopSource(channel_sources[0],AL_FALSE);
      enigma::sounds[i]->playing = false;
      if (enigma::sounds[i]->seek)
          enigma::sounds[i]->seek(enigma::sounds[i]->userdata,0);
    }
  }
}

void audio_pause_all()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 0) {
      if(enigma::sounds[i] && channel_sources[0] && enigma::sounds[i]->playing)
        enigma::sounds[i]->playing = !alurePauseSource(channel_sources[0]);
    }
  }
}

void audio_pause_music()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 1) {
      if(enigma::sounds[i] && channel_sources[0] && enigma::sounds[i]->playing)
        enigma::sounds[i]->playing = !alurePauseSource(channel_sources[0]);
    }
  }
}

void audio_resume_all()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 0) {
      if(enigma::sounds[i] && channel_sources[0] && !enigma::sounds[i]->playing)
        enigma::sounds[i]->playing = alureResumeSource(channel_sources[0]);
    }
  }
}

void audio_resume_music()
{
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if (enigma::sounds[i]->type == 1) {
      if(enigma::sounds[i] && channel_sources[0] && !enigma::sounds[i]->playing)
        enigma::sounds[i]->playing = alureResumeSource(channel_sources[0]);
    }
  }
}

int audio_get_type(int index) 
{
  if (!audio_exists(index)) {return -1;}
  get_sound(snd, index, 0);
  return snd->type;
}

void audio_listener_orientation(double lookat_x, double lookat_y, double lookat_z, double up_x, double up_y, double up_z)
{
  listenerOri[0] = up_x;
  listenerOri[1] = up_y;
  listenerOri[2] = up_z;
  listenerOri[3] = lookat_x;
  listenerOri[4] = lookat_y;
  listenerOri[5] = lookat_z;
  alListenerfv(AL_ORIENTATION, listenerOri);
}

void audio_listener_position(double x, double y, double z)
{
  listenerPos[0] = x;
  listenerPos[1] = y;
  listenerPos[2] = z;
  alListenerfv(AL_POSITION, listenerPos);
}

void audio_listener_velocity(double vx, double vy, double vz)
{
  listenerVel[0] = vx;
  listenerVel[1] = vy;
  listenerVel[2] = vz;
  alListenerfv(AL_VELOCITY, listenerVel);
}

double audio_sound_length(int index) 
{
  get_sound(snd,index,0);
  ALint size, bits, channels, freq;

  alGetBufferi(snd->buf[0], AL_SIZE, &size);
  alGetBufferi(snd->buf[0], AL_BITS, &bits);
  alGetBufferi(snd->buf[0], AL_CHANNELS, &channels);
  alGetBufferi(snd->buf[0], AL_FREQUENCY, &freq);

  return size / channels / (bits/8) / (float)freq;
}

void audio_master_gain(float volume, double time) 
{
  alListenerf(AL_GAIN, volume);
}

void audio_music_gain(int index, float volume, double time) 
{
  alSourcef(channel_sources[index], AL_GAIN, volume);
}

void audio_sound_gain(int index, float volume, double time)
{	
  alSourcef(channel_sources[index], AL_GAIN, volume);
}

void audio_channel_num(int num) {
  channel_num = num;
}

int audio_system()
{
  // return audio_old_system or audio_new_system
}

int audio_add(string fname, int type)
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

void audio_delete(int sound)
{
  get_sound(snd,sound,);
  alureDestroyStream(snd->stream, 0, 0);
  alDeleteSources(channel_num, channel_sources);
  delete enigma::sounds[sound];
  enigma::sounds[sound] = NULL;
}

void audio_falloff_set_model(int model)
{

}

int audio_emitter_create()
{

}

bool audio_emitter_exists(int index)
{

}

void audio_emitter_falloff(int emitter, double falloff_ref, double falloff_max, double falloff_factor)
{

}

void audio_emitter_free(int emitter)
{

}

void audio_emitter_gain(int emitter, double gain)
{

}

void audio_emitter_pitch(int emitter, double pitch)
{
  get_emitter(emit,emitter,);
  emit->pitch = pitch;
}

void audio_emitter_position(int emitter, double x, double y, double z)
{
  get_emitter(emit,emitter,);
  emit->emitPos[0] = x;
  emit->emitPos[1] = y;
  emit->emitPos[2] = z;
}

void audio_emitter_velocity(int emitter, double vx, double vy, double vz)
{
  get_emitter(emit,emitter,);
  emit->emitVel[0] = vx;
  emit->emitVel[1] = vy;
  emit->emitVel[2] = vz;
}

void audio_play_sound_on(int emitter, int sound, bool loop, double priority)
{

}
