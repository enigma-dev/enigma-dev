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
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "DSsystem.h"

#include "Universal_System/estring.h"

namespace enigma_user {

bool audio_exists(int sound) { return sound_exists(sound); }

bool audio_is_playing(int index) {
  return sound_isplaying(index);
}

bool audio_is_paused(int index) {
  return sound_ispaused(index);
}

int audio_play_sound(int sound, double priority, bool loop) {
  if (loop) return sound_loop(sound);
  return sound_play(sound);
}

void audio_stop_sound(int index) {
  sound_stop(index);
}

void audio_pause_sound(int index) {
  sound_pause(index);
}

void audio_resume_sound(int index) {
  sound_resume(index);
}

void audio_stop_all() {
  sound_stop_all();
}

void audio_pause_all() {
  sound_pause_all();
}

void audio_resume_all() {
  sound_resume_all();
}

void audio_sound_seek(int index, double offset) { sound_seek(index, (float)offset); }

double audio_sound_offset(int index) {
  return sound_get_position(index);
}

int audio_sound_length(int index) {
  return (int)sound_get_length(index);
}

void audio_sound_gain(int index, float volume, double time) {
  sound_volume(index, volume);
}

void audio_master_gain(float volume) { sound_global_volume(volume); }

int audio_add(string fname) {
  return sound_add(fname, 0, true);
}

void audio_delete(int sound) {
  sound_delete(sound);
}

}  // namespace enigma_user
