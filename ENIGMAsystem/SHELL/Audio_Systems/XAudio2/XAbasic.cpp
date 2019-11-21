/** Copyright (C) 2013 Robert B. Colton
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
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "SoundResource.h"
#include "SoundEmitter.h"
#include "XAsystem.h"

#include "Universal_System/estring.h"

#include <vector>
using std::vector;

namespace enigma_user
{

bool sound_exists(int sound)
{

}

bool sound_play(int sound) // Returns whether sound is playing
{

}
bool sound_loop(int sound) // Returns whether sound is playing
{

}

bool sound_pause(int sound) // Returns whether the sound was successfully paused
{

}

void sound_pause_all()
{

}

void sound_stop(int sound) {

}

void sound_stop_all()
{

}

void sound_delete(int sound) {

}

void sound_volume(int sound, float volume) {

}

void sound_global_volume(float mastervolume) {

}

bool sound_resume(int sound) // Returns whether the sound is playing
{

}
void sound_resume_all()
{

}

bool sound_isplaying(int sound) {

}

bool sound_ispaused(int sound) {

}

void sound_pan(int sound, float value)
{

}

float sound_get_length(int sound) { // Not for Streams

}

float sound_get_position(int sound) { // Not for Streams

}

void sound_seek(int sound, float position) {

}

void sound_seek_all(float position) {

}

void action_sound(int snd, bool loop)
{

}

const char* sound_get_audio_error() {

}

}

#include <string>
using namespace std;
extern void show_message(string);

namespace enigma_user
{

int sound_add(string fname, int kind, bool preload) //At the moment, the latter two arguments do nothing! =D
{

}

bool sound_replace(int sound, string fname, int kind, bool preload)
{

}

}
