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

#include "DSsystem.h"
#include "Audio_Systems/General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "Universal_System/estring.h"

#include <string>
#include <vector>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

using std::string;

namespace enigma {

template <typename EffectInterface, typename EffectParams>
void set_sound_effect_parameters(int sound, REFGUID rguidObject, DWORD dwIndex, REFGUID rguidInterface, EffectParams* effectParams) {
  const Sound& snd = sounds.get(sound);

  ComPtr<IDirectSoundBuffer8> soundBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundBuffer8);

  ComPtr<EffectInterface> sndFX8 = 0;
  soundBuffer8->GetObjectInPath(rguidObject, dwIndex, rguidInterface, (void**)&sndFX8);
  if (sndFX8) sndFX8->SetAllParameters(effectParams);
}

} // namespace enigma

namespace enigma_user {

bool sound_exists(int sound) { return sounds.exists(sound); }

bool sound_play(int sound)  // Returns whether sound is playing
{
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->SetCurrentPosition(0);
  snd.soundBuffer->Play(0, 0, 0);
  return true;
}

bool sound_loop(int sound)  // Returns whether sound is playing
{
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->SetCurrentPosition(0);
  snd.soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
  return true;
}

bool sound_pause(int sound)  // Returns whether the sound was successfully paused
{
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->Stop();
  return true;
}

void sound_pause_all() {
  for (std::pair<int, const Sound&> sndi : sounds)
    sndi.second.soundBuffer->Stop();
}

void sound_stop(int sound) {
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->Stop();
}

void sound_stop_all() {
  for (std::pair<int, const Sound&> sndi : sounds)
    sndi.second.soundBuffer->Stop();
}

void sound_delete(int sound) {
  sound_stop(sound);
  sounds.destroy(sound);
}

void sound_volume(int sound, float volume) {
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->SetVolume((1 - volume) * DSBVOLUME_MIN);
}

void sound_global_volume(float volume) { primaryBuffer->SetVolume(volume); }

bool sound_resume(int sound)  // Returns whether the sound is playing
{
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->Play(0, 0, 0);
  return true;
}

void sound_resume_all() {
  for (std::pair<int, const Sound&> sndi : sounds)
    sndi.second.soundBuffer->Play(0, 0, 0);
}

bool sound_isplaying(int sound) {
  const Sound& snd = sounds.get(sound);
  DWORD ret = 0;
  snd.soundBuffer->GetStatus(&ret);
  return (ret & DSBSTATUS_PLAYING);
}

bool sound_ispaused(int sound) {
  const Sound& snd = sounds.get(sound);
  return !snd.idle and !snd.playing;
}

void sound_pan(int sound, float value) {
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->SetPan(value * 10000);
}

float sound_get_pan(int sound) {
  const Sound& snd = sounds.get(sound);
  LONG ret = 0;
  snd.soundBuffer->GetPan(&ret);
  return ret;
}

float sound_get_volume(int sound) {
  const Sound& snd = sounds.get(sound);
  LONG ret = 0;
  snd.soundBuffer->GetVolume(&ret);
  return ret;
}

float sound_get_length(int sound) {  // Not for Streams
  const Sound& snd = sounds.get(sound);
  return snd.length;
}

float sound_get_position(int sound) {  // Not for Streams
  const Sound& snd = sounds.get(sound);
  DWORD ret = 0;
  snd.soundBuffer->GetCurrentPosition(&ret, NULL);
  return ret;
}

void sound_seek(int sound, float position) {
  const Sound& snd = sounds.get(sound);
  snd.soundBuffer->SetCurrentPosition(position);
}

void sound_seek_all(float position) {
  for (std::pair<int, const Sound&> sndi : sounds)
    sndi.second.soundBuffer->SetCurrentPosition(position);
}

const char* sound_get_audio_error() { return ""; }

}  // namespace enigma_user

#include <string>
#include "../General/ASutil.h"
using namespace std;

namespace enigma_user {

int sound_add(string fname, int kind, bool preload)  //At the moment, the latter two arguments do nothing! =D
{
  // Open sound
  size_t flen = 0;
  char* fdata = enigma::read_all_bytes(fname, flen);
  if (!fdata) return -1;

  // Decode sound
  int rid = sounds.size();
  bool fail = enigma::sound_add_from_buffer(rid, fdata, flen);
  delete[] fdata;

  if (fail) return -1;
  return rid;
}

bool sound_replace(int sound, string fname, int kind, bool preload) {
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
  const Sound& snd = sounds.get(sound);

  // query for the 3d buffer interface
  ComPtr<IDirectSound3DBuffer8> sound3DBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetConeOrientation(x, y, z, DS3D_IMMEDIATE);
  sound3DBuffer8->SetConeAngles(anglein, angleout, DS3D_IMMEDIATE);
  sound3DBuffer8->SetConeOutsideVolume(voloutside, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_distance(int sound, float mindist, float maxdist) {
  const Sound& snd = sounds.get(sound);

  // query for the 3d buffer interface
  ComPtr<IDirectSound3DBuffer8> sound3DBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetMinDistance(mindist, DS3D_IMMEDIATE);
  sound3DBuffer8->SetMaxDistance(maxdist, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_position(int sound, float x, float y, float z) {
  const Sound& snd = sounds.get(sound);

  // query for the 3d buffer interface
  ComPtr<IDirectSound3DBuffer8> sound3DBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetPosition(x, y, z, DS3D_IMMEDIATE);
}

void sound_3d_set_sound_velocity(int sound, float x, float y, float z) {
  const Sound& snd = sounds.get(sound);

  // query for the 3d buffer interface
  ComPtr<IDirectSound3DBuffer8> sound3DBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSound3DBuffer, (void**)&sound3DBuffer8);

  sound3DBuffer8->SetVelocity(x, y, z, DS3D_IMMEDIATE);
}

void sound_effect_chorus(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay,
                         long phase) {
	DSFXChorus effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fDepth = depth;
  effectParams.fFeedback = feedback;
  effectParams.fFrequency = frequency;
  effectParams.lWaveform = wave;
  effectParams.fDelay = delay;
  effectParams.lPhase = phase;

  enigma::set_sound_effect_parameters<IDirectSoundFXChorus8, DSFXChorus>(
    sound, GUID_DSFX_STANDARD_CHORUS, 0, IID_IDirectSoundFXChorus8, &effectParams);
}

void sound_effect_echo(int sound, float wetdry, float feedback, float leftdelay, float rightdelay, long pandelay) {
  DSFXEcho effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fFeedback = feedback;
  effectParams.fLeftDelay = leftdelay;
  effectParams.fRightDelay = rightdelay;
  effectParams.lPanDelay = pandelay;

  enigma::set_sound_effect_parameters<IDirectSoundFXEcho8, DSFXEcho>(
    sound, GUID_DSFX_STANDARD_ECHO, 0, IID_IDirectSoundFXEcho8, &effectParams);
}

void sound_effect_flanger(int sound, float wetdry, float depth, float feedback, float frequency, long wave, float delay,
                          long phase) {
  DSFXFlanger effectParams = { };
  effectParams.fWetDryMix = wetdry;
  effectParams.fDepth = depth;
  effectParams.fFeedback = feedback;
  effectParams.fFrequency = frequency;
  effectParams.lWaveform = wave;
  effectParams.fDelay = delay;
  effectParams.lPhase = phase;

  enigma::set_sound_effect_parameters<IDirectSoundFXFlanger8, DSFXFlanger>(
    sound, GUID_DSFX_STANDARD_FLANGER, 0, IID_IDirectSoundFXFlanger8, &effectParams);
}

void sound_effect_gargle(int sound, unsigned rate, unsigned wave) {
  DSFXGargle effectParams = { };
  effectParams.dwRateHz = rate;
  effectParams.dwWaveShape = wave;

  enigma::set_sound_effect_parameters<IDirectSoundFXGargle8, DSFXGargle>(
    sound, GUID_DSFX_STANDARD_GARGLE, 0, IID_IDirectSoundFXGargle8, &effectParams);
}

void sound_effect_reverb(int sound, float gain, float mix, float time, float ratio) {
  DSFXWavesReverb effectParams = { };
  effectParams.fInGain = gain;
  effectParams.fReverbMix = mix;
  effectParams.fReverbTime = time;
  effectParams.fHighFreqRTRatio = ratio;

  enigma::set_sound_effect_parameters<IDirectSoundFXWavesReverb8, DSFXWavesReverb>(
    sound, GUID_DSFX_WAVES_REVERB, 0, IID_IDirectSoundFXWavesReverb8, &effectParams);
}

void sound_effect_compressor(int sound, float gain, float attack, float release, float threshold, float ratio,
                             float delay) {
  DSFXCompressor effectParams = { };
  effectParams.fGain = gain;
  effectParams.fAttack = attack;
  effectParams.fRelease = release;
  effectParams.fThreshold = threshold;
  effectParams.fRatio = ratio;
  effectParams.fPredelay = delay;

  enigma::set_sound_effect_parameters<IDirectSoundFXCompressor8, DSFXCompressor>(
    sound, GUID_DSFX_STANDARD_COMPRESSOR, 0, IID_IDirectSoundFXCompressor8, &effectParams);
}

void sound_effect_equalizer(int sound, float center, float bandwidth, float gain) {
  DSFXParamEq effectParams = { };
  effectParams.fCenter = center;
  effectParams.fBandwidth = bandwidth;
  effectParams.fGain = gain;

  enigma::set_sound_effect_parameters<IDirectSoundFXParamEq8, DSFXParamEq>(
    sound, GUID_DSFX_STANDARD_PARAMEQ, 0, IID_IDirectSoundFXParamEq8, &effectParams);
}

static const GUID sound_effect_guids[7] = {
    GUID_DSFX_STANDARD_CHORUS, GUID_DSFX_STANDARD_ECHO,       GUID_DSFX_STANDARD_FLANGER, GUID_DSFX_STANDARD_GARGLE,
    GUID_DSFX_WAVES_REVERB,    GUID_DSFX_STANDARD_COMPRESSOR, GUID_DSFX_STANDARD_PARAMEQ};

void sound_effect_set(int sound, int effect) {
  size_t numOfEffects = 0;

  std::vector<DWORD> dwResults;
  std::vector<DSEFFECTDESC> dsEffects;

  if (effect != 0) {
    // count the number of effect flags that were set
    for (size_t i = 0; i < 7; ++i) {
      if ((effect >> i) & 1U) {
        ++numOfEffects;
      }
    }

    // allocate an array of effect descriptions and results
    dwResults.resize(numOfEffects);
    dsEffects.resize(numOfEffects);

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

  const Sound& snd = sounds.get(sound);

  // GM8 allows changing the effect flags in the middle of a sound playing
  // but this doesn't work unless we follow the Microsoft documentation
  // and temporarily pause the sound
  // "Additionally, the buffer must not be playing or locked."
  // https://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.idirectsoundbuffer8.idirectsoundbuffer8.setfx%28v=vs.85%29.aspx
  DWORD status = 0;
  snd.soundBuffer->GetStatus(&status);
  bool wasPlaying = (status & DSBSTATUS_PLAYING);
  bool wasLooping = (status & DSBSTATUS_LOOPING);
  if (wasPlaying) snd.soundBuffer->Stop();  // pause

  // query for the effect interface and set the effects on the sound buffer
  ComPtr<IDirectSoundBuffer8> soundBuffer8 = 0;
  snd.soundBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&soundBuffer8);
  if (soundBuffer8) soundBuffer8->SetFX(numOfEffects, dsEffects.data(), dwResults.data());

  if (wasPlaying) snd.soundBuffer->Play(0, 0, wasLooping ? DSBPLAY_LOOPING : 0);  // resume
}

}  // namespace enigma_user
