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

#include <string>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
using std::string;
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "SoundChannel.h"
#include "SoundResource.h"
#include "SoundEmitter.h"
#include "ALsystem.h"

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

#ifdef DEBUG_MODE
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h" // show_error
#endif

#include "Universal_System/estring.h"

#include <vector>
using std::vector;

namespace enigma_user
{

bool sound_exists(int sound)
{
  return sound_resources.find(sound)!=sound_resources.end() && sound_resources[sound];
}

bool sound_play(int sound) { // Returns whether sound is playing
  int src = enigma::get_free_channel(1);
  if (src == -1) { return false; }
  get_sound(snd,sound,false);
  alSourcei(sound_channels[src]->source, AL_BUFFER, snd->buf[0]);
  alSourcei(sound_channels[src]->source, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcei(sound_channels[src]->source, AL_REFERENCE_DISTANCE, 1);
  alSourcei(sound_channels[src]->source, AL_LOOPING, AL_FALSE);
  alSourcef(sound_channels[src]->source, AL_GAIN, snd->volume);
  alSourcef(sound_channels[src]->source, AL_PITCH, snd->pitch);
  float sourcePosAL[] = { snd->pan, 0.0f, 0.0f};
  alSourcefv(sound_channels[src]->source, AL_POSITION, sourcePosAL);
  sound_channels[src]->soundIndex = sound;
  return !(snd->idle = !(snd->playing = !snd->stream ?
	alurePlaySource(sound_channels[src]->source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
	alurePlaySourceStream(sound_channels[src]->source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}

bool sound_loop(int sound) { // Returns whether sound is playing
  int src = enigma::get_free_channel(1);
  if (src == -1) { return false; }
  get_sound(snd,sound,false);
  alSourcei(sound_channels[src]->source, AL_BUFFER, snd->buf[0]);
  alSourcei(sound_channels[src]->source, AL_SOURCE_RELATIVE, AL_TRUE);
  alSourcei(sound_channels[src]->source, AL_REFERENCE_DISTANCE, 1);
  alSourcei(sound_channels[src]->source, AL_LOOPING, AL_TRUE); //Just playing
  alSourcef(sound_channels[src]->source, AL_GAIN, snd->volume);
  alSourcef(sound_channels[src]->source, AL_PITCH, snd->pitch);
  float sourcePosAL[] = { snd->pan, 0.0f, 0.0f};
  alSourcefv(sound_channels[src]->source, AL_POSITION, sourcePosAL);
  sound_channels[src]->soundIndex = sound;
  return !(snd->idle = !(snd->playing = !snd->stream ?
	alurePlaySource(sound_channels[src]->source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
	alurePlaySourceStream(sound_channels[src]->source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}

bool sound_pause(int sound) { // Returns whether the sound was successfully paused
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      return alurePauseSource(sound_channels[i]->source);
    }
  }
  return false;
}

void sound_pause_all() {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alurePauseSource(sound_channels[i]->source);
  }
}

void sound_stop(int sound) {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      alureStopSource(sound_channels[i]->source, AL_FALSE);
    }
  }
}

void sound_stop_all() {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alureStopSource(sound_channels[i]->source, AL_FALSE);
  }
}

void sound_delete(int sound) {
  if (sound_resources.find(sound)!=sound_resources.end()) {
    if (sound_resources[sound]) {
      sound_stop(sound);
      get_sound(snd,sound,);
      alureDestroyStream(snd->stream, 0, 0);
      for (size_t i = 0; i < sound_channels.size(); i++) {
        if (sound_channels[i]->soundIndex == sound) {
          alDeleteSources(1, &sound_channels[i]->source);
          sound_channels[i]->soundIndex=-1;
        }
      }
      delete sound_resources[sound];
    }
    sound_resources.erase(sound);
  }
}

void sound_pan(int sound, float value) {
  get_sound(snd,sound,);
  snd->pan = value;
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      float sourcePosAL[] = { value, 0.0f, 0.0f};
      alSourcefv(sound_channels[i]->source, AL_POSITION, sourcePosAL);
    }
  }
}

void sound_volume(int sound, float value) {
  get_sound(snd,sound,);
  snd->volume = value;
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      alSourcef(sound_channels[i]->source, AL_GAIN, value);
    }
  }
}

void sound_pitch(int sound, float value) {
  get_sound(snd,sound,);
  snd->pitch = value;
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      alSourcef(sound_channels[i]->source, AL_PITCH, value);
    }
  }
}

void sound_global_volume(float mastervolume) {
    alListenerf(AL_GAIN, mastervolume);
}

bool sound_resume(int sound) // Returns whether the sound is playing
{
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      return alureResumeSource(sound_channels[i]->source);
    }
  }
  return false;
}

void sound_resume_all()
{
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alureResumeSource(sound_channels[i]->source);
  }
}

bool sound_isplaying(int sound) {
  // test for channels playing the sound
  for (size_t i = 0; i < sound_channels.size(); i++)
  {
    if (sound_channels[i]->soundIndex == sound) {
      ALint state;
      alGetSourcei(sound_channels[i]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PLAYING)
      {
        return true;
      }
    }
  }
  return false;
}

bool sound_ispaused(int sound) {
  return !sound_resources[sound]->idle and !sound_resources[sound]->playing;
}

float sound_get_length(int sound) { // Not for Streams
  get_sound(snd,sound,0.0);
  ALint size, bits, channels, freq;

  alGetBufferi(snd->buf[0], AL_SIZE, &size);
  alGetBufferi(snd->buf[0], AL_BITS, &bits);
  alGetBufferi(snd->buf[0], AL_CHANNELS, &channels);
  alGetBufferi(snd->buf[0], AL_FREQUENCY, &freq);

  return size / channels / (bits/8) / (float)freq;
}

float sound_get_position(int sound) { // Not for Streams
  float offset = -1;
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound)
      alGetSourcef(sound_channels[i]->source, AL_SEC_OFFSET, &offset);
  }
  return offset;
}

void sound_seek(int sound, float position) {
  get_sound(snd,sound,);
  if (snd->seek) snd->seek(snd->userdata, position); // Streams
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == sound) {
      alSourcef(sound_channels[i]->source, AL_SEC_OFFSET, position); // Non Streams
    }
  }
}

void sound_seek_all(float position) {
  for (std::map<int, SoundResource*>::iterator it=sound_resources.begin(); it!=sound_resources.end(); it++) {
    SoundResource* sr = it->second;
    if(sr) {
      if (sr->seek) sr->seek(sr->userdata, position); // Streams
    }
  }

  for(size_t i = 0; i < sound_channels.size(); i++) {
    alSourcef(sound_channels[i]->source, AL_SEC_OFFSET, position); // Non Streams
  }
}

void action_sound(int snd, bool loop)
{
    (loop ? sound_loop:sound_play)(snd);
}

const char* sound_get_audio_error() {
  return alureGetErrorString();
}

}

#include <string>
using namespace std;
extern void show_message(string);

namespace enigma_user
{

int sound_add(string fname, int kind, bool preload) //At the moment, the latter two arguments do nothing! =D
{
  // Open sound
  FILE *afile = fopen(fname.c_str(),"rb");
  if (!afile)
    return -1;

  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  char *fdata = new char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");

  // Decode sound
  int rid = enigma::sound_allocate();
  bool fail = enigma::sound_add_from_buffer(rid,fdata,flen);
  delete [] fdata;

  if (fail)
    return -1;
  return rid;
}

bool sound_replace(int sound, string fname, int kind, bool preload)
{
  if (sound_resources.find(sound)!=sound_resources.end() && sound_resources[sound]) {
    get_sound(snd,sound,false);
    alureDestroyStream(snd->stream, 0, 0);
    for(size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == sound)
      {
        alDeleteSources(1, &sound_channels[i]->source);
      }
    }
  }
  sound_resources[sound] = enigma::sound_new_with_source();

  // Open sound
  FILE *afile = fopen(fname.c_str(),"rb");
  if (!afile)
    return -1;

  // Buffer sound
  fseek(afile,0,SEEK_END);
  const size_t flen = ftell(afile);
  char *fdata = new char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");

  // Decode sound
  bool fail = enigma::sound_add_from_buffer(sound,fdata,flen);
  delete [] fdata;
  return fail;
}

void sound_3d_set_sound_cone(int sound, float x, float y, float z, double anglein, double angleout, long voloutside) {
}

void sound_3d_set_sound_distance(int sound, float mindist, float maxdist) {
}

void sound_3d_set_sound_position(int sound, float x, float y, float z) {
}

void sound_3d_set_sound_velocity(int sound, float x, float y, float z) {
}

void sound_effect_chorus(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase) {

}

void sound_effect_compressor(int sound, float gain, float attack, float release, float threshold, float ratio, float delay) {
}

void sound_effect_echo(int sound, float wetdry, float feedback, float leftdelay, float rightdelay, long pandelay) {
}

void sound_effect_equalizer(int sound, float center, float bandwidth, float gain) {
}

void sound_effect_flanger(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase) {
}

void sound_effect_gargle(int sound, unsigned rate, unsigned wave) {
}

void sound_effect_reverb(int sound, float gain, float mix, float time, float ratio) {
}

void sound_effect_set(int sound, int effect) {

}

}
