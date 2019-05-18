/** Copyright (C) 2013, 2018 Robert B. Colton
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
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "DSsystem.h"

#ifdef DEBUG_MODE
#include "Widget_Systems/widgets_mandatory.h"  // show_error
#include "libEGMstd.h"
#endif

#include "Universal_System/estring.h"

namespace enigma_user {

bool sound_exists(int sound) { return unsigned(sound) < sound_resources.size() && bool(sound_resources[sound]); }

bool sound_play(int sound)  // Returns whether sound is playing
{
  get_sound(snd, sound, false);
  snd->soundBuffer->SetCurrentPosition(0);
  snd->soundBuffer->Play(0, 0, 0);
  return true;
}

bool sound_loop(int sound)  // Returns whether sound is playing
{
  get_sound(snd, sound, false);
  snd->soundBuffer->SetCurrentPosition(0);
  snd->soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
  return true;
}

bool sound_pause(int sound)  // Returns whether the sound was successfully paused
{
  get_sound(snd, sound, false);
  snd->soundBuffer->Stop();
  return true;
}

void sound_pause_all() {
  for (size_t i = 0; i < sound_resources.size(); i++) {
    get_soundv(snd, i);
    snd->soundBuffer->Stop();
  }
}

void sound_stop(int sound) {
  get_soundv(snd, sound);
  snd->soundBuffer->Stop();
}

void sound_stop_all() {
  for (size_t i = 0; i < sound_resources.size(); i++) {
    get_soundv(snd, i);
    snd->soundBuffer->Stop();
  }
}

void sound_delete(int sound) {
  sound_stop(sound);
  sound_resources.erase(sound_resources.begin() + sound);
}

void sound_volume(int sound, float volume) {
  get_soundv(snd, sound);
  snd->soundBuffer->SetVolume((1 - volume) * DSBVOLUME_MIN);
}

void sound_global_volume(float volume) { primaryBuffer->SetVolume(volume); }

bool sound_resume(int sound)  // Returns whether the sound is playing
{
  get_sound(snd, sound, false);
  snd->soundBuffer->Play(0, 0, 0);
  return true;
}

void sound_resume_all() {
  for (size_t i = 0; i < sound_resources.size(); i++) {
    get_soundv(snd, i);
    snd->soundBuffer->Play(0, 0, 0);
  }
}

bool sound_isplaying(int sound) {
  get_sound(snd, sound, false);
  DWORD ret = 0;
  snd->soundBuffer->GetStatus(&ret);
  return (ret & DSBSTATUS_PLAYING);
}

bool sound_ispaused(int sound) {
  get_sound(snd, sound, false);
  return !snd->idle and !snd->playing;
}

void sound_pan(int sound, float value) {
  get_soundv(snd, sound);
  snd->soundBuffer->SetPan(value * 10000);
}

float sound_get_pan(int sound) {
  get_sound(snd, sound, -1);
  LONG ret = 0;
  snd->soundBuffer->GetPan(&ret);
  return ret;
}

float sound_get_volume(int sound) {
  get_sound(snd, sound, -1);
  LONG ret = 0;
  snd->soundBuffer->GetVolume(&ret);
  return ret;
}

float sound_get_length(int sound) {  // Not for Streams
  get_sound(snd, sound, -1);

  return snd->length;
}

float sound_get_position(int sound) {  // Not for Streams
  get_sound(snd, sound, -1);
  DWORD ret = 0;
  snd->soundBuffer->GetCurrentPosition(&ret, NULL);
  return ret;
}

void sound_seek(int sound, float position) {
  get_soundv(snd, sound);
  snd->soundBuffer->SetCurrentPosition(position);
}

void sound_seek_all(float position) {
  for (size_t i = 0; i < sound_resources.size(); i++) {
    get_soundv(snd, i);
    snd->soundBuffer->SetCurrentPosition(position);
  }
}

void action_sound(int snd, bool loop) { (loop ? sound_loop : sound_play)(snd); }

const char* sound_get_audio_error() { return ""; }

}  // namespace enigma_user

#include <string>
#include "../General/ASutil.h"
using namespace std;
extern void show_message(string);

namespace enigma_user {

int sound_add(string fname, int kind, bool preload)  //At the moment, the latter two arguments do nothing! =D
{
  // Open sound
  size_t flen = 0;
  char* fdata = enigma::read_all_bytes(fname, flen);
  if (!fdata) return -1;

  // Decode sound
  int rid = enigma::sound_allocate();
  bool fail = enigma::sound_add_from_buffer(rid, fdata, flen);
  delete[] fdata;

  if (fail) return -1;
  return rid;
}

bool sound_replace(int sound, string fname, int kind, bool preload) {
  if (sound >= 0 && size_t(sound) < sound_resources.size() && sound_resources[sound]) {
    get_sound(snd, sound, false);
    delete snd;
  }

  // Open sound
  size_t flen = 0;
  char* fdata = enigma::read_all_bytes(fname, flen);
  if (!fdata) return -1;

  // Decode sound
  bool fail = enigma::sound_add_from_buffer(sound, fdata, flen);
  delete[] fdata;
  return fail;
}

void sound_3d_set_sound_cone(int sound, float x, float y, float z, double anglein, double angleout, long voloutside) {
  get_soundv(snd, sound);

  // query for the 3d buffer interface
  IDirectSound3DBuffer8* sound3DBuffer8 = 0;
  snd->soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetConeOrientation(x, y, z, DS3D_IMMEDIATE);
  sound3DBuffer8->SetConeAngles(anglein, angleout, DS3D_IMMEDIATE);
  sound3DBuffer8->SetConeOutsideVolume(voloutside, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_distance(int sound, float mindist, float maxdist) {
  get_soundv(snd, sound);

  // query for the 3d buffer interface
  IDirectSound3DBuffer8* sound3DBuffer8 = 0;
  snd->soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetMinDistance(mindist, DS3D_IMMEDIATE);
  sound3DBuffer8->SetMaxDistance(maxdist, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_position(int sound, float x, float y, float z) {
  get_soundv(snd, sound);

  // query for the 3d buffer interface
  IDirectSound3DBuffer8* sound3DBuffer8 = 0;
  snd->soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetPosition(x, y, z, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_velocity(int sound, float x, float y, float z) {
  get_soundv(snd, sound);

  // query for the 3d buffer interface
  IDirectSound3DBuffer8* sound3DBuffer8 = 0;
  snd->soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetVelocity(x, y, z, DS3D_IMMEDIATE);
}

void sound_effect_chorus(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay,
                         long phase) {
  /*
	DSFXChorus effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fDepth = depth;
  effectParams.fFeedback = Feedback;
  effectParams.fFrequency = frequency;
  effectParams.lWaveform = wave;
  effectParams.fDelay = delay;
  effectParams.lPhase = phase;
  */
}

void sound_effect_echo(int sound, float wetdry, float feedback, float leftdelay, float rightdelay, long pandelay) {
  /*
  DSFXEcho effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fFeedback = feedback;
  effectParams.fLeftDelay = leftdelay;
  effectParams.fRightDelay = rightdelay;
  effectParams.lPanDelay = pandelay;
  */
}

void sound_effect_flanger(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay,
                          long phase) {
  /*
  DSFXFlanger effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fDepth = depth;
  effectParams.fFeedback = feedback;
  effectParams.fFrequency = frequency;
  effectParams.lWaveform = wave;
  effectParams.fDelay = delay;
  effectParams.lPhase = phase;
  */
}

void sound_effect_gargle(int sound, unsigned rate, unsigned wave) {
  /*
  DSFXGargle effectParams = { };
  effectParams.dwRateHz = rate;
  effectParams.dwWaveShape = wave;
  */
}

void sound_effect_reverb(int sound, float gain, float mix, float time, float ratio) {
  /*
  DSFXWavesReverb effectParams = { };
  effectParams.fInGain = gain;
  effectParams.fReverbMix = mix;
  effectParams.fReverbTime = time;
  effectParams.fHighFreqRTRatio = ratio;
  */
}

void sound_effect_compressor(int sound, float gain, float attack, float release, float threshold, float ratio,
                             float delay) {
  /*
  DSFXCompressor effectParams = { };
  effectParams.fGain = gain;
  effectParams.fAttack = attack;
  effectParams.fRelease = release;
  effectParams.fThreshold = threshold;
  effectParams.fRatio = ratio;
  effectParams.fPredelay = delay;
  */
}

void sound_effect_equalizer(int sound, float center, float bandwidth, float gain) {
  /*
  DSFXParamEq effectParams = { };
  effectParams.fCenter = center;
  effectParams.fBandwidth = bandwidth;
  effectParams.fGain = gain;
  */
}

static const GUID sound_effect_guids[7] = {
    GUID_DSFX_STANDARD_CHORUS, GUID_DSFX_STANDARD_ECHO,       GUID_DSFX_STANDARD_FLANGER, GUID_DSFX_STANDARD_GARGLE,
    GUID_DSFX_WAVES_REVERB,    GUID_DSFX_STANDARD_COMPRESSOR, GUID_DSFX_STANDARD_PARAMEQ};

void sound_effect_set(int sound, int effect) {
  size_t numOfEffects = 0;

  DWORD* dwResults = 0;
  DSEFFECTDESC* dsEffects = 0;

  if (effect != 0) {
    // count the number of effect flags that were set
    for (size_t i = 0; i < 7; ++i) {
      if ((effect >> i) & 1U) {
        ++numOfEffects;
      }
    }

    // allocate an array of effect descriptions and results
    dwResults = new DWORD[numOfEffects];
    dsEffects = new DSEFFECTDESC[numOfEffects];

    size_t eff = 0;
    // loop all the bits of the effect parameter to see which of the flags were set
    // and fill in the effect descriptions based on that
    for (size_t i = 0; i < 7; ++i) {
      if ((effect >> i) & 1U) {
        memset(&dsEffects[eff], 0, sizeof(DSEFFECTDESC));
        dsEffects[eff].dwSize = sizeof(DSEFFECTDESC);
        dsEffects[eff].dwFlags = 0;
        dsEffects[eff].guidDSFXClass = sound_effect_guids[i];
        ++eff;
      }
    }
  }

  get_soundv(snd, sound);

  // GM8 allows changing the effect flags in the middle of a sound playing
  // but this doesn't work unless we follow the Microsoft documentation
  // and temporarily pause the sound
  // "Additionally, the buffer must not be playing or locked."
  // https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsoundbuffer8.idirectsoundbuffer8.setfx%28v=vs.85%29.aspx
  DWORD status = 0;
  snd->soundBuffer->GetStatus(&status);
  bool wasPlaying = (status & DSBSTATUS_PLAYING);
  bool wasLooping = (status & DSBSTATUS_LOOPING);
  if (wasPlaying) snd->soundBuffer->Stop();  // pause

  // query for the effect interface and set the effects on the sound buffer
  IDirectSoundBuffer8* soundBuffer8 = 0;
  snd->soundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundBuffer8);
  soundBuffer8->SetFX(numOfEffects, dsEffects, dwResults);

  if (wasPlaying) snd->soundBuffer->Play(0, 0, wasLooping ? DSBPLAY_LOOPING : 0);  // resume
}

}  // namespace enigma_user
