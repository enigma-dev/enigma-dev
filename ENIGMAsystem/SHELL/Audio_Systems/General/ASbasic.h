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

// ***** Old Sound System *****
#ifndef ENIGMA_AS_BASIC_H
#define ENIGMA_AS_BASIC_H

#include <string>
using std::string;

namespace enigma_user
{

bool sound_exists(int sound);
bool sound_play(int sound);
bool sound_loop(int sound);
void sound_stop(int sound);
void sound_stop_all();
void sound_delete(int sound);
bool sound_pause(int sound);
void sound_pause_all();
bool sound_resume(int sound);
void sound_resume_all();

float sound_get_pan(int sound);
float sound_get_volume(int sound);
float sound_get_length(int sound);
float sound_get_position(int sound);
void sound_seek(int sound, float position);
void sound_seek_all(float position);

bool sound_isplaying(int sound);
bool sound_ispaused(int sound);

inline void action_sound(int snd, bool loop) { (loop ? sound_loop : sound_play)(snd); }

int sound_add(string fname, int kind, bool preload);
bool sound_replace(int sound, string fname, int kind, bool preload);
inline bool action_replace_sound(int sound, string fname)
{
  return sound_replace(sound, fname, 0, true);
}

const char* sound_get_audio_error();

void sound_pan(int sound, float value);
void sound_pitch(int sound, float value);
void sound_volume(int sound, float value);
void sound_global_volume(float mastervolume);

void sound_3d_set_sound_cone(int snd, float x, float y, float z, double anglein, double angleout, long voloutside);
void sound_3d_set_sound_distance(int snd, float mindist, float maxdist);
void sound_3d_set_sound_position(int snd, float x, float y, float z);
void sound_3d_set_sound_velocity(int snd, float x, float y, float z);

enum {
  se_none = 0,
  se_chorus = 1,
  se_echo = 2,
  se_flanger = 4,
  se_gargle = 8,
  se_reverb = 16,
  se_compressor = 32,
  se_equalizer = 64
};

void sound_effect_chorus(int snd, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase);
void sound_effect_echo(int snd, float wetdry, float feedback, float leftdelay, float rightdelay, long pandelay);
void sound_effect_flanger(int snd, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase);
void sound_effect_gargle(int snd, unsigned rate, unsigned wave);
void sound_effect_reverb(int snd, float gain, float mix, float time, float ratio);
void sound_effect_compressor(int snd, float gain, float attack, float release, float threshold, float ratio, float delay);
void sound_effect_equalizer(int snd, float center, float bandwidth, float gain);
void sound_effect_set(int snd, int effect);

}

#endif
