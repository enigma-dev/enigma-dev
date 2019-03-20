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

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string>
using std::string;
#include "../General/ASadvanced.h"
#include "ALsystem.h"
#include "Audio_Systems/audio_mandatory.h"
#include "SoundChannel.h"
#include "SoundEmitter.h"
#include "SoundResource.h"

#ifdef __APPLE__
#include "../../../additional/alure/include/AL/alure.h"
#else
#include <AL/alure.h>
#endif

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"  // show_error
#include "libEGMstd.h"
#endif

#include "Universal_System/estring.h"

#include <vector>
using std::vector;
#include <time.h>

ALenum falloff_models[7] = {AL_NONE,
                            AL_INVERSE_DISTANCE,
                            AL_INVERSE_DISTANCE_CLAMPED,
                            AL_LINEAR_DISTANCE,
                            AL_LINEAR_DISTANCE_CLAMPED,
                            AL_EXPONENT_DISTANCE,
                            AL_EXPONENT_DISTANCE_CLAMPED};

namespace enigma_user {

bool audio_exists(int sound) { return sound_resources.find(sound) != sound_resources.end() && sound_resources[sound]; }

bool audio_is_playing(int index) {
  if (index >= 200000) {
    if (sound_channels[index - 200000]->soundIndex == index) {
      ALint state;
      alGetSourcei(sound_channels[index - 200000]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PLAYING) {
        return true;
      }
    }
  }
  // test for channels playing the sound
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == index) {
      ALint state;
      alGetSourcei(sound_channels[i]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PLAYING) {
        return true;
      }
    }
  }
  return false;
}

bool audio_is_paused(int index) {
  if (index >= 200000) {
    if (sound_channels[index - 200000]->soundIndex == index) {
      ALint state;
      alGetSourcei(sound_channels[index - 200000]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PAUSED) {
        return true;
      }
    }
  }
  // test for channels with the sound paused
  for (size_t i = 0; i < sound_channels.size(); i++) {
    if (sound_channels[i]->soundIndex == index) {
      ALint state;
      alGetSourcei(sound_channels[i]->source, AL_SOURCE_STATE, &state);
      if (state == AL_PAUSED) {
        return true;
      }
    }
  }
  return false;
}

int audio_play_sound(int sound, double priority, bool loop) {
  int src = enigma::get_free_channel(priority);
  if (src != -1) {
    get_sound(snd, sound, 0);
    alSourcei(sound_channels[src]->source, AL_BUFFER, snd->buf[0]);
    alSourcei(sound_channels[src]->source, AL_SOURCE_RELATIVE, AL_TRUE);
    alSourcei(sound_channels[src]->source, AL_REFERENCE_DISTANCE, 1);
    alSourcei(sound_channels[src]->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    alSourcef(sound_channels[src]->source, AL_PITCH, snd->pitch);
    alSourcef(sound_channels[src]->source, AL_GAIN, snd->volume);
    sound_channels[src]->priority = priority;
    sound_channels[src]->soundIndex = sound;
    snd->idle = !(snd->playing =
                      !snd->stream ? alurePlaySource(sound_channels[src]->source, enigma::eos_callback,
                                                     (void *)(ptrdiff_t)sound) != AL_FALSE
                                   : alurePlaySourceStream(sound_channels[src]->source, snd->stream, 3, -1,
                                                           enigma::eos_callback, (void *)(ptrdiff_t)sound) != AL_FALSE);
    return src + 200000;
  } else {
    return -1;
  }
}

int audio_play_sound_at(int sound, as_scalar x, as_scalar y, as_scalar z, as_scalar falloff_ref, as_scalar falloff_max,
                        as_scalar falloff_factor, bool loop, double priority) {
  int src = enigma::get_free_channel(priority);
  if (src != -1) {
    get_sound(snd, sound, 0);
    alSourcei(sound_channels[src]->source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    ALfloat soundPos[3] = {x, y, z};
    alSourcefv(sound_channels[src]->source, AL_POSITION, soundPos);
    alSourcef(sound_channels[src]->source, AL_REFERENCE_DISTANCE, falloff_ref);
    alSourcef(sound_channels[src]->source, AL_MAX_DISTANCE, falloff_max);
    alSourcef(sound_channels[src]->source, AL_ROLLOFF_FACTOR, falloff_factor);
    alSourcef(sound_channels[src]->source, AL_PITCH, snd->pitch);
    alSourcef(sound_channels[src]->source, AL_GAIN, snd->volume);
    sound_channels[src]->priority = priority;
    sound_channels[src]->soundIndex = sound;
    snd->idle = !(snd->playing =
                      !snd->stream ? alurePlaySource(sound_channels[src]->source, enigma::eos_callback,
                                                     (void *)(ptrdiff_t)sound) != AL_FALSE
                                   : alurePlaySourceStream(sound_channels[src]->source, snd->stream, 3, -1,
                                                           enigma::eos_callback, (void *)(ptrdiff_t)sound) != AL_FALSE);
    return src + 200000;
  } else {
    return -1;
  }
}

int audio_play_sound_on(int emitter, int sound, bool loop, double priority) {
  SoundEmitter *emit = sound_emitters[emitter];
  int src = audio_play_sound_at(sound, emit->emitPos[0], emit->emitPos[1], emit->emitPos[2], emit->falloff[0],
                                emit->falloff[1], emit->falloff[2], loop, priority) -
            200000;
  alSourcefv(sound_channels[src]->source, AL_VELOCITY, emit->emitVel);
  alSourcei(sound_channels[src]->source, AL_PITCH, emit->pitch);
  return src + 200000;
}

void audio_stop_sound(int index) {
  if (index >= 200000) {
    alureStopSource(sound_channels[index - 200000]->source, AL_TRUE);
  } else {
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == index) {
        alureStopSource(sound_channels[i]->source, AL_TRUE);
      }
    }
  }
}

void audio_pause_sound(int index) {
  if (index >= 200000) {
    alurePauseSource(sound_channels[index - 200000]->source);
  } else {
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == index) {
        alurePauseSource(sound_channels[i]->source);
      }
    }
  }
}

void audio_resume_sound(int index) {
  if (index >= 200000) {
    alureResumeSource(sound_channels[index - 200000]->source);
  } else {
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == index) {
        alureResumeSource(sound_channels[i]->source);
      }
    }
  }
}

void audio_stop_all() {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alureStopSource(sound_channels[i]->source, AL_TRUE);
  }
}

void audio_pause_all() {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alurePauseSource(sound_channels[i]->source);
  }
}

void audio_resume_all() {
  for (size_t i = 0; i < sound_channels.size(); i++) {
    alureResumeSource(sound_channels[i]->source);
  }
}

void audio_sound_seek(int index, double offset) { alSourcef(sound_channels[index]->source, AL_SEC_OFFSET, offset); }

double audio_sound_offset(int index) {
  float offset;
  alGetSourcef(sound_channels[index]->source, AL_SEC_OFFSET, &offset);
  return offset;
}

int audio_sound_length(int index) {
  ALint buffer;
  if (index >= 200000) {
    alGetSourcei(sound_channels[index - 200000]->source, AL_BUFFER, &buffer);
  } else {
    get_sound(snd, index, 0);
    buffer = snd->buf[0];
  }
  ALint size, bits, channels, freq;

  alGetBufferi(buffer, AL_SIZE, &size);
  alGetBufferi(buffer, AL_BITS, &bits);
  alGetBufferi(buffer, AL_CHANNELS, &channels);
  alGetBufferi(buffer, AL_FREQUENCY, &freq);

  return size / channels / (bits / 8) / (float)freq;
}

void audio_sound_gain(int index, float volume, double time) {
  if (index >= 200000) {
    alSourcef(sound_channels[index - 200000]->source, AL_GAIN, volume);
  } else {
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == index) {
        alSourcef(sound_channels[i]->source, AL_GAIN, volume);
      }
    }
  }
}

void audio_sound_pitch(int index, float pitch) {
  if (index >= 200000) {
    alSourcef(sound_channels[index - 200000]->source, AL_PITCH, pitch);
  } else {
    for (size_t i = 0; i < sound_channels.size(); i++) {
      if (sound_channels[i]->soundIndex == index) {
        alSourcef(sound_channels[i]->source, AL_PITCH, pitch);
      }
    }
  }
}

void audio_listener_orientation(as_scalar lookat_x, as_scalar lookat_y, as_scalar lookat_z, as_scalar up_x,
                                as_scalar up_y, as_scalar up_z) {
  listenerOri[0] = up_x;
  listenerOri[1] = up_y;
  listenerOri[2] = up_z;
  listenerOri[3] = lookat_x;
  listenerOri[4] = lookat_y;
  listenerOri[5] = lookat_z;
  alListenerfv(AL_ORIENTATION, listenerOri);
}

void audio_listener_position(as_scalar x, as_scalar y, as_scalar z) {
  listenerPos[0] = x;
  listenerPos[1] = y;
  listenerPos[2] = z;
  alListenerfv(AL_POSITION, listenerPos);
}

void audio_listener_velocity(as_scalar vx, as_scalar vy, as_scalar vz) {
  listenerVel[0] = vx;
  listenerVel[1] = vy;
  listenerVel[2] = vz;
  alListenerfv(AL_VELOCITY, listenerVel);
}

void audio_master_gain(float volume) { alListenerf(AL_GAIN, volume); }

void audio_channel_num(int num) { channel_num = num; }

int audio_system() { return audio_new_system; }

int audio_add(string fname) {
  // Decode sound
  int rid = enigma::sound_allocate();
  bool fail = enigma::sound_add_from_file(rid, fname);

  return (fail ? -1 : rid);
}

void audio_delete(int sound) {
  if (sound_resources.find(sound) != sound_resources.end()) {
    if (sound_resources[sound]) {
      get_soundv(snd, sound);
      alureDestroyStream(snd->stream, 0, 0);
      delete sound_resources[sound];
      sound_resources[sound] = 0;
    }
  }
}

void audio_falloff_set_model(int model) { alDistanceModel(falloff_models[model]); }

int audio_emitter_create() {
  int i = sound_emitters.size();
  sound_emitters.push_back(new SoundEmitter());
  return i;
}

bool audio_emitter_exists(int index) { return (((unsigned)index < sound_emitters.size()) && (sound_emitters[index])); }

void audio_emitter_falloff(int emitter, as_scalar falloff_ref, as_scalar falloff_max, as_scalar falloff_factor) {
  SoundEmitter *emit = sound_emitters[emitter];
  emit->falloff[0] = falloff_ref;
  emit->falloff[1] = falloff_max;
  emit->falloff[2] = falloff_factor;
}

void audio_emitter_free(int emitter) {
  delete sound_emitters[emitter];
  sound_emitters[emitter] = NULL;
}

void audio_emitter_gain(int emitter, double volume) {
  SoundEmitter *emit = sound_emitters[emitter];
  emit->volume = volume;
}

void audio_emitter_pitch(int emitter, double pitch) {
  SoundEmitter *emit = sound_emitters[emitter];
  emit->pitch = pitch;
}

void audio_emitter_position(int emitter, as_scalar x, as_scalar y, as_scalar z) {
  SoundEmitter *emit = sound_emitters[emitter];
  emit->emitPos[0] = x;
  emit->emitPos[1] = y;
  emit->emitPos[2] = z;
}

void audio_emitter_velocity(int emitter, as_scalar vx, as_scalar vy, as_scalar vz) {
  SoundEmitter *emit = sound_emitters[emitter];
  emit->emitVel[0] = vx;
  emit->emitVel[1] = vy;
  emit->emitVel[2] = vz;
}

}  // namespace enigma_user
