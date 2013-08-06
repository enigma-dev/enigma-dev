/** Copyright (C) 2008-2013 Robert B. Colton
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
#include "DSbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "DSsystem.h"

#ifdef DEBUG_MODE
#include "libEGMstd.h"
#include "Widget_Systems/widgets_mandatory.h" // show_error
#endif

#include "Universal_System/estring.h"

namespace enigma_user
{

bool sound_exists(int sound)
{
    return unsigned(sound) < enigma::sound_idmax && bool(enigma::sounds[sound]);
}

bool sound_play(int sound) // Returns whether sound is playing
{
	get_sound(snd, sound, 0);
	snd->soundBuffer->SetCurrentPosition(0);
	snd->soundBuffer->Play(0, 0, 0);
}

bool sound_loop(int sound) // Returns whether sound is playing
{
	get_sound(snd, sound, 0);
	snd->soundBuffer->SetCurrentPosition(0);
	snd->soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
}

bool sound_pause(int sound) // Returns whether the sound was successfully paused
{
	get_sound(snd, sound, 0);
	snd->soundBuffer->Stop();
}

void sound_pause_all()
{
  for (size_t i = 0; i < enigma::sound_idmax; i++) {
    get_sound(snd, i, 0);
	snd->soundBuffer->Stop();
  }
}

void sound_stop(int sound) {
	get_sound(snd, sound, 0);
	snd->soundBuffer->Stop();
}

void sound_stop_all()
{
  for (size_t i = 0; i < enigma::sound_idmax; i++) {
    get_sound(snd, i, 0);
	snd->soundBuffer->Stop();
  }
}

void sound_delete(int sound) {
  sound_stop(sound);
  delete enigma::sounds[sound];
  enigma::sounds[sound] = NULL;
}

void sound_volume(int sound, float volume) {
	get_sound(snd,sound,0);
	snd->soundBuffer->SetVolume(volume);
}

void sound_global_volume(float volume) {
	primaryBuffer->SetVolume(volume);
}

bool sound_resume(int sound) // Returns whether the sound is playing
{
	get_sound(snd, sound, 0);
	snd->soundBuffer->Play(0, 0, 0);
}

void sound_resume_all()
{
  for (size_t i = 0; i < enigma::sound_idmax; i++) {
    get_sound(snd, i, 0);
	snd->soundBuffer->Play(0, 0, 0);
  }
}

bool sound_isplaying(int sound) {
	get_sound(snd, sound, 0);
	LPDWORD ret;
	snd->soundBuffer->GetStatus(ret);
	if (*ret == DSBSTATUS_LOOPING || *ret == DSBSTATUS_LOOPING) {
		return true;
	} else {
		return false;
	}
}

bool sound_ispaused(int sound) {
  return !enigma::sounds[sound]->idle and !enigma::sounds[sound]->playing;
}

void sound_pan(int sound, float value)
{
	get_sound(snd, sound, 0);
	snd->soundBuffer->SetPan(value);
}

float sound_get_pan(int sound) {
	get_sound(snd, sound, 0);
	LPLONG ret;
	snd->soundBuffer->GetPan(ret);
	return *ret;
}

float sound_get_volume(int sound) {
	get_sound(snd, sound, 0);
	LPLONG ret;
	snd->soundBuffer->GetVolume(ret);
	return *ret;
}

float sound_get_length(int sound) { // Not for Streams
	get_sound(snd, sound, 0);
	//snd->soundBuffer->GetLength();
}

float sound_get_position(int sound) { // Not for Streams
	get_sound(snd, sound, 0);
	LPDWORD ret;
	snd->soundBuffer->GetCurrentPosition(ret, NULL);
	return *ret;
}

void sound_seek(int sound, float position) {
	get_sound(snd, sound, 0);
	snd->soundBuffer->SetCurrentPosition(position);
}

void sound_seek_all(float position) {
  for (size_t i = 0; i < enigma::sound_idmax; i++) {
    get_sound(snd, i, 0);
	snd->soundBuffer->SetCurrentPosition(position);
  }
}

void action_sound(int snd, bool loop)
{
    (loop ? sound_loop:sound_play)(snd);
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

bool sound_replace(int sound, string fname, int kind, bool preload)
{

}

}
