/** Copyright (C) 2008-2011 Josh Ventura, (C) 2013 Robert B. Colton
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

#include <vector>
using std::vector;
#include <time.h>
clock_t starttime;
clock_t elapsedtime;
clock_t lasttime;

ALfloat listenerPos[]={0.0f,0.0f,0.0f};
ALfloat listenerVel[]={0.0f,0.0f,0.0f};
ALfloat listenerOri[]={0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
int falloff_model = 0;

int channel_num = 128;
struct sound_instance {
  ALuint source;
  int soundIndex;
  double priority;
  bool playing;
  int type;
  sound_instance(ALint alsource, int sound_id): source(alsource), soundIndex(sound_id) {}
  sound_instance() {}
  void sound_update();
};
void sound_instance::sound_update()
{
  // NOTE: Use starttime, elapsedtime, and lasttime
  // calculate fade

  // calculate falloff
  switch (falloff_model)
  {
    case audio_falloff_exponent_distance:
      // gain = (listener_distance / reference_distance) ^ (-falloff_factor)
      break;
    case audio_falloff_exponent_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = (distance / reference_distance) ^ (-falloff_factor)
      break;
    case audio_falloff_inverse_distance:
      // gain = reference_distance / (reference_distance + falloff_factor * (listener_distance – reference_distance))
      break;
    case audio_falloff_inverse_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = reference_distance / (reference_distance + falloff_factor * (distance – reference_distance))
      break;
    case audio_falloff_linear_distance:
      // distance = min(distance, maximum_distance)
      // gain = (1 – falloff_factor * (distance – reference_distance) / (maximum_distance – reference_distance))
      break;
    case audio_falloff_linear_distance_clamped:
      // distance = clamp(listener_distance, reference_distance, maximum_distance)
      // gain = (1 – falloff_factor * (distance – reference_distance) / (maximum_distance – reference_distance))
      break;
    case audio_falloff_none:
      // gain = 1
      break;
    default:
      break;
  }
}

// first one is reserved for background music
vector<sound_instance> sound_sources(1);

struct soundEmitter
{
  ALfloat emitPos[3];
  ALfloat emitVel[3];
  ALfloat falloff[3];
  ALfloat pitch;
  ALfloat volume;
  soundEmitter()
  {
    ALfloat emitPos[3]={0.0f,0.0f,0.0f};
    ALfloat emitVel[3]={0.0f,0.0f,0.0f};
    ALfloat falloff[3]={0.0f,0.0f,1.0f};
    volume = 1.0f;
  }
};
vector<soundEmitter*> sound_emitters(0);

int get_free_channel(double priority)
{
  // test for channels not playing anything
  for(size_t i = 1; i < sound_sources.size(); i++)
  {
    ALint state;
    alGetSourcei(sound_sources[i].source, AL_SOURCE_STATE, &state);
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
    //sound_sources[i].source = src;
    sound_sources.push_back(sound_instance(src,-1));
    return i;
  }
  // test for channels playing a lower priority sound, and take their spot if they are
  for(size_t i = 1; i < sound_sources.size(); i++)
  {
    if (sound_sources[i].priority < priority)
    {
      return i;
    }
  }
}

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
    ALuint buf[3]; // The buffer-id of the sound data
    alureStream *stream; // optional stream
    void (*cleanup)(void *userdata); // optional cleanup callback for streams
    void *userdata; // optional userdata for streams
    void (*seek)(void *userdata, float position); // optional seeking
    int type; //0 for sound, 1 for music, -1 for error
    int kind;

    load_state loaded;   // Degree to which this sound has been loaded successfully
    bool idle;    // True if this sound is not being used, false if playing or paused.
    bool playing; // True if this sound is playing; not paused or idle.

    sound(): type(0), kind(0), stream(0), cleanup(0), userdata(0), seek(0), loaded(LOADSTATE_NONE), idle(1), playing(0) {
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

  void audiosystem_update(void)
  {
    elapsedtime = clock() - lasttime;
    lasttime = elapsedtime;

    alureUpdate();
    // update all the sounds so they can calculate fall off and gain
    for(size_t i = 1; i < sound_sources.size(); i++) {
      sound_sources[i].sound_update();
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
            alureStopSource(sound_sources[i].source, true);
            alureDestroyStream(sounds[i]->stream, 0, 0);
            if (sounds[i]->cleanup) sounds[i]->cleanup(sounds[i]->userdata);
          }
          // fallthrough
        case LOADSTATE_SOURCED:
          for(size_t i = 1; i < sound_sources.size(); i++) {
            alDeleteSources(1, &sound_sources[i].source);
          }
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
  int src = get_free_channel(1);
  if (src != -1)
  {
    get_sound(snd,sound,0); //snd.looping = false;
    alSourcei(sound_sources[src].source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[src].source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[src].source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[src].source, AL_LOOPING, AL_FALSE); //Just playing
    sound_sources[src].soundIndex = sound;
    return !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src].source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src].source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
  }
  else
  {
    return -1;
  }
}
bool sound_loop(int sound) // Returns whether sound is playing
{
  int src = get_free_channel(1);
  if (src != -1)
  {
    get_sound(snd,sound,0); //snd.looping = false;
    alSourcei(sound_sources[src].source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[src].source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[src].source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[src].source, AL_LOOPING, AL_TRUE); //Just playing
    sound_sources[src].soundIndex = sound;
    return !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src].source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src].source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
  }
  else
  {
    return -1;
  }
}
bool sound_pause(int sound) // Returns whether the sound is still playing
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      return sound_sources[i].playing = !alurePauseSource(sound_sources[i].source);
    }
  }
}
void sound_pause_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
    sound_sources[i].playing = !alurePauseSource(sound_sources[i].source);
  }
}
void sound_stop(int sound) {
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      sound_sources[i].playing = !alureStopSource(sound_sources[i].source, AL_FALSE);
    }
  }
}
void sound_stop_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
    sound_sources[i].playing = !alureStopSource(sound_sources[i].source, AL_FALSE);
  }
}
void sound_delete(int sound) {
  sound_stop(sound);
  get_sound(snd,sound,);
  alureDestroyStream(snd->stream, 0, 0);
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      alDeleteSources(1, &sound_sources[i].source);
      sound_sources[i].soundIndex=-1;
    }
  }
  delete enigma::sounds[sound];
  enigma::sounds[sound] = NULL;
}
void sound_volume(int sound, float volume) {

  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      alSourcef(sound_sources[i].source, AL_GAIN, volume);
    }
  }
}
void sound_global_volume(float mastervolume) {
    alListenerf(AL_GAIN, mastervolume);
}
bool sound_resume(int sound) // Returns whether the sound is playing
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      return sound_sources[i].playing = alureResumeSource(sound_sources[i].source);
    }
  }
}
void sound_resume_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
    sound_sources[i].playing = alureResumeSource(sound_sources[i].source);
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
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      const float pan = value*2-1;
      alSource3f(sound_sources[i].source,AL_POSITION,pan,sqrt(1-pan*pan),0);
    }
  }
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

  //alGetSourcef(channel_sources[0], AL_SEC_OFFSET, &offset);
  return offset;
}
void sound_seek(int sound, float position) {
  get_sound(snd,sound,);
  if (snd->seek) snd->seek(snd->userdata, position); // Streams
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      alSourcef(sound_sources[i].source, AL_SEC_OFFSET, position); // Non Streams
    }
  }
}
void sound_seek_all(float position) {
  for(size_t i = 0;i < enigma::sound_idmax;i++) {
    if(enigma::sounds[i]) {
      if (enigma::sounds[i]->seek) enigma::sounds[i]->seek(enigma::sounds[i]->userdata, position); // Streams
    }
  }

  for(size_t i = 1; i < sound_sources.size(); i++) {
    alSourcef(sound_sources[i].source, AL_SEC_OFFSET, position); // Non Streams
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
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound)
    {
      alDeleteSources(1, &sound_sources[i].source);
    }
  }
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
    // channel 0 is reserved for the background music, only ever one at a time can play
    if (!sound_sources[0].source)
    {
      alGenSources(1, &sound_sources[0].source);
    }
    get_sound(snd,sound,0);
    alSourcei(sound_sources[0].source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[0].source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[0].source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[0].source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    sound_sources[0].soundIndex = sound;
    sound_sources[0].type = 1;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[0].source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[0].source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}

int audio_play_sound(int sound, double priority, bool loop)
{
  int src = get_free_channel(priority);
  if (src != -1)
  {
    get_sound(snd,sound,0);
    alSourcei(sound_sources[src].source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[src].source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[src].source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[src].source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    sound_sources[src].priority = priority;
    sound_sources[src].soundIndex = sound;
    sound_sources[src].type = 0;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src].source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src].source, snd->stream, 3, -1, enigma::eos_callback,
        (void*)(ptrdiff_t)sound) != AL_FALSE));
    return src;
  }
  else
  {
    return -1;
  }
}

int audio_play_sound_at(int sound, double x, double y, double z, int falloff_ref, double falloff_max, double falloff_factor, bool loop, double priority)
{
  int src = get_free_channel(priority);
  if (src != -1)
  {
    get_sound(snd,sound,0);
    alSourcei(sound_sources[src].source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    ALfloat soundPos[]={0.0f,0.0f,0.0f};
    alSourcefv(sound_sources[src].source, AL_POSITION, soundPos);
    sound_sources[src].priority = priority;
    sound_sources[src].soundIndex = sound;
    sound_sources[src].type = 0;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src].source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src].source, snd->stream, 3, -1, enigma::eos_callback,
        (void*)(ptrdiff_t)sound) != AL_FALSE));
    return src;
  }
  else
  {
    return -1;
  }
}

void audio_pause_sound(int index)
{
  alurePauseSource(sound_sources[index].source);
}

void audio_resume_sound(int index)
{
  alureResumeSource(sound_sources[index].source);
}

void audio_stop_sound(int index)
{
  alureStopSource(sound_sources[index].source, AL_TRUE);
  sound_sources[index].playing = false;
}

void audio_stop_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      alureStopSource(sound_sources[i].source, AL_FALSE);
      sound_sources[i].playing = false;
  }
}

void audio_stop_music()
{
  alureStopSource(sound_sources[0].source, AL_FALSE);
  sound_sources[0].playing = false;
}

void audio_pause_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      sound_sources[i].playing = !alurePauseSource(sound_sources[i].source);
  }
}

void audio_pause_music()
{
  sound_sources[0].playing = !alurePauseSource(sound_sources[0].source);
}

void audio_resume_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      sound_sources[i].playing = alureResumeSource(sound_sources[i].source);
  }
}

void audio_resume_music()
{
  sound_sources[0].playing = alureResumeSource(sound_sources[0].source);
}

void audio_music_seek(double offset)
{
  alSourcef(sound_sources[0].source, AL_SEC_OFFSET, offset);
}

void audio_sound_seek(int index, double offset)
{
  alSourcef(sound_sources[index].source, AL_SEC_OFFSET, offset);
}

double audio_music_offset()
{
  float offset;
  alGetSourcef(sound_sources[0].source, AL_SEC_OFFSET, &offset);
  return offset;
}

double audio_sound_offset(int index)
{
  float offset;
  alGetSourcef(sound_sources[index].source, AL_SEC_OFFSET, &offset);
  return offset;
}

int audio_get_type(int index)
{
  // -1 for out of bounds or error, 0 for normal sound, 1 for bg music
  if (!audio_exists(index)) {return -1;}
  const sound_instance &sndsrc = sound_sources[index];
  return enigma::sounds[sndsrc.soundIndex]->type;
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

void audio_music_gain(float volume, double time)
{
  alSourcef(sound_sources[0].source, AL_GAIN, volume);
}

void audio_sound_gain(int index, float volume, double time)
{
  alSourcef(sound_sources[index].source, AL_GAIN, volume);
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
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i].soundIndex == sound) {
      alDeleteSources(1, &sound_sources[i].source);
      sound_sources[i].soundIndex=-1;
      audio_stop_sound(i);
    }
  }
  delete enigma::sounds[sound];
  enigma::sounds[sound] = NULL;
}

void audio_falloff_set_model(int model)
{
  falloff_model = model;
}

int audio_emitter_create()
{
  soundEmitter *res = new soundEmitter();
  int i = sound_emitters.size();
  sound_emitters.resize(i + 1);
  sound_emitters[i] = res;
  return i;
}

bool audio_emitter_exists(int index)
{
  return (index < sound_emitters.size()) && bool(sound_emitters[index]);
}

void audio_emitter_falloff(int emitter, double falloff_ref, double falloff_max, double falloff_factor)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->falloff[0] = falloff_ref;
  emit->falloff[1] = falloff_max;
  emit->falloff[2] = falloff_factor;
}

void audio_emitter_free(int emitter)
{
  delete sound_emitters[emitter];
  sound_emitters[emitter] = NULL;
}

void audio_emitter_gain(int emitter, double volume)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->volume = volume;
}

void audio_emitter_pitch(int emitter, double pitch)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->pitch = pitch;
}

void audio_emitter_position(int emitter, double x, double y, double z)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->emitPos[0] = x;
  emit->emitPos[1] = y;
  emit->emitPos[2] = z;
}

void audio_emitter_velocity(int emitter, double vx, double vy, double vz)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->emitVel[0] = vx;
  emit->emitVel[1] = vy;
  emit->emitVel[2] = vz;
}

void audio_play_sound_on(int emitter, int sound, bool loop, double priority)
{
  soundEmitter *emit = sound_emitters[emitter];
  int src = audio_play_sound_at(sound, emit->emitPos[0], emit->emitPos[1], emit->emitPos[2],
    emit->falloff[0], emit->falloff[1], emit->falloff[2], loop, priority);
  alSourcefv(sound_sources[src].source, AL_VELOCITY, emit->emitVel);
  alSourcei(sound_sources[src].source, AL_PITCH, emit->pitch);
}
