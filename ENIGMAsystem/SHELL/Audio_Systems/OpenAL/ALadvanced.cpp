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

#include <string>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
using std::string;
#include "../General/ASadvanced.h"
#include "Audio_Systems/audio_mandatory.h"
#include "SoundInstance.h"
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
#include <time.h>

namespace enigma_user
{

bool audio_exists(int sound)
{
  return unsigned(sound) < enigma::sound_idmax && bool(enigma::sounds[sound]);
}
bool audio_is_playing(int sound) {
  // test for channels playing the sound
  for(size_t i = 0; i < sound_sources.size(); i++)
  {
    if (sound_sources[i]->soundIndex == sound) {
      ALint state;
      alGetSourcei(sound_sources[i]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PLAYING)
      {
        return true;
      }
    }
  }
  return false;
}

void audio_play_music(int sound, bool loop)
{
    // channel 0 is reserved for the background music, only ever one at a time can play
    if (!sound_sources[0]->source)
    {
      alGenSources(1, &sound_sources[0]->source);
    }
    get_sound(snd,sound,);
    alSourcei(sound_sources[0]->source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[0]->source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[0]->source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[0]->source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    sound_sources[0]->soundIndex = sound;
    sound_sources[0]->type = 1;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[0]->source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[0]->source, snd->stream, 3, -1, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE));
}

int audio_play_sound(int sound, double priority, bool loop)
{
  int src = enigma::get_free_channel(priority);
  if (src != -1)
  {
    get_sound(snd,sound,0);
    alSourcei(sound_sources[src]->source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_sources[src]->source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_sources[src]->source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_sources[src]->source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    sound_sources[src]->priority = priority;
    sound_sources[src]->soundIndex = sound;
    sound_sources[src]->type = 0;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src]->source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src]->source, snd->stream, 3, -1, enigma::eos_callback,
        (void*)(ptrdiff_t)sound) != AL_FALSE));
    return sound_sources[src]->source;
  }
  else
  {
    return -1;
  }
}

int audio_play_sound_at(int sound, as_scalar x, as_scalar y, as_scalar z, int falloff_ref, as_scalar falloff_max, as_scalar falloff_factor, bool loop, double priority)
{
  int src = enigma::get_free_channel(priority);
  if (src != -1)
  {
    get_sound(snd,sound,0);
    alSourcei(sound_sources[src]->source, AL_LOOPING, loop?AL_TRUE:AL_FALSE);
    ALfloat soundPos[]={0.0f,0.0f,0.0f};
    alSourcefv(sound_sources[src]->source, AL_POSITION, soundPos);
    sound_sources[src]->priority = priority;
    sound_sources[src]->soundIndex = sound;
    sound_sources[src]->type = 0;
    !(snd->idle = !(snd->playing = !snd->stream ?
      alurePlaySource(sound_sources[src]->source, enigma::eos_callback, (void*)(ptrdiff_t)sound) != AL_FALSE :
      alurePlaySourceStream(sound_sources[src]->source, snd->stream, 3, -1, enigma::eos_callback,
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
  alurePauseSource(sound_sources[index]->source);
}

void audio_resume_sound(int index)
{
  alureResumeSource(sound_sources[index]->source);
}

void audio_stop_sound(int index)
{
  alureStopSource(sound_sources[index]->source, AL_TRUE);
}

void audio_pause_channel(int index)
{
  alurePauseSource(index);
}

void audio_resume_channel(int index)
{
  alureResumeSource(index);
}

void audio_stop_channel(int index)
{
  alureStopSource(index, AL_TRUE);
}


void audio_stop_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      alureStopSource(sound_sources[i]->source, AL_FALSE);
  }
}

void audio_stop_music()
{
  alureStopSource(sound_sources[0]->source, AL_FALSE);
}

void audio_pause_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      alurePauseSource(sound_sources[i]->source);
  }
}

void audio_pause_music()
{
  alurePauseSource(sound_sources[0]->source);
}

void audio_resume_all()
{
  for(size_t i = 1; i < sound_sources.size(); i++) {
      alureResumeSource(sound_sources[i]->source);
  }
}

void audio_resume_music()
{
  alureResumeSource(sound_sources[0]->source);
}

void audio_music_seek(double offset)
{
  alSourcef(sound_sources[0]->source, AL_SEC_OFFSET, offset);
}

void audio_sound_seek(int index, double offset)
{
  alSourcef(sound_sources[index]->source, AL_SEC_OFFSET, offset);
}

double audio_music_offset()
{
  float offset;
  alGetSourcef(sound_sources[0]->source, AL_SEC_OFFSET, &offset);
  return offset;
}

double audio_sound_offset(int index)
{
  float offset;
  alGetSourcef(sound_sources[index]->source, AL_SEC_OFFSET, &offset);
  return offset;
}

int audio_get_type(int index)
{
  // -1 for out of bounds or error, 0 for normal sound, 1 for bg music
  if (!audio_exists(index)) {return -1;}
  const sound_instance* sndsrc = sound_sources[index];
  return enigma::sounds[sndsrc->soundIndex]->type;
}

void audio_listener_orientation(as_scalar lookat_x, as_scalar lookat_y, as_scalar lookat_z, as_scalar up_x, as_scalar up_y, as_scalar up_z)
{
  listenerOri[0] = up_x;
  listenerOri[1] = up_y;
  listenerOri[2] = up_z;
  listenerOri[3] = lookat_x;
  listenerOri[4] = lookat_y;
  listenerOri[5] = lookat_z;
  alListenerfv(AL_ORIENTATION, listenerOri);
}

void audio_listener_position(as_scalar x, as_scalar y, as_scalar z)
{
  listenerPos[0] = x;
  listenerPos[1] = y;
  listenerPos[2] = z;
  alListenerfv(AL_POSITION, listenerPos);
}

void audio_listener_velocity(as_scalar vx, as_scalar vy, as_scalar vz)
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
  alSourcef(sound_sources[0]->source, AL_GAIN, volume);
}

void audio_sound_gain(int index, float volume, double time)
{
  alSourcef(sound_sources[index]->source, AL_GAIN, volume);
}

void audio_channel_num(int num) {
  channel_num = num;
}

int audio_system()
{
  return audio_new_system;
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
  char *fdata = new char[flen];
  fseek(afile,0,SEEK_SET);
  if (fread(fdata,1,flen,afile) != flen)
    puts("WARNING: Resource stream cut short while loading sound data");

  // Decode sound
  int rid = enigma::sound_allocate();
  bool fail = enigma::sound_add_from_buffer(rid,fdata,flen);
  delete fdata;
  
  if (fail)
    return (--enigma::sound_idmax, -1);
  return rid;
}

void audio_delete(int sound)
{
  get_sound(snd,sound,);
  alureDestroyStream(snd->stream, 0, 0);
  for(size_t i = 1; i < sound_sources.size(); i++) {
    if (sound_sources[i]->soundIndex == sound) {
      alDeleteSources(1, &sound_sources[i]->source);
      sound_sources[i]->soundIndex=-1;
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
  return (((unsigned)index < sound_emitters.size()) && (sound_emitters[index]));
}

void audio_emitter_play(int index, bool loop)
{

}

void audio_emitter_stop(int index)
{

}

void audio_emitter_pause(int index)
{

}

void audio_emitter_resume(int index)
{

}

void audio_emitter_seek(int index)
{

}

void audio_emitter_skip(int index, int track)
{

}

void audio_emitter_skip_start(int index)
{
  audio_emitter_skip(index, 0);
}

void audio_emitter_skip_end(int index)
{

}

void audio_emitter_skip_previous(int index)
{

}

void audio_emitter_skip_back(int index)
{

}

void audio_emitter_skip_next(int index)
{

}

int audio_emitter_sound_count(int index)
{

}

void audio_emitter_sound_add(int index, int sound)
{

}

void audio_emitter_sound_remove(int index, int track)
{

}

void audio_emitter_sound_set(int index, int track, int sound)
{

}

int audio_emitter_sound_get(int index, int track)
{

}

void audio_emitter_loop(int index, bool loop)
{

}

void audio_emitter_falloff(int emitter, as_scalar falloff_ref, as_scalar falloff_max, as_scalar falloff_factor)
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

void audio_emitter_position(int emitter, as_scalar x, as_scalar y, as_scalar z)
{
  soundEmitter *emit = sound_emitters[emitter];
  emit->emitPos[0] = x;
  emit->emitPos[1] = y;
  emit->emitPos[2] = z;
}

void audio_emitter_velocity(int emitter, as_scalar vx, as_scalar vy, as_scalar vz)
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
  alSourcefv(sound_sources[src]->source, AL_VELOCITY, emit->emitVel);
  alSourcei(sound_sources[src]->source, AL_PITCH, emit->pitch);
}

}

