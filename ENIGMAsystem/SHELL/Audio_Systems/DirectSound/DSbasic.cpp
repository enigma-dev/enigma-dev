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
#include "../General/ASbasic.h"
#include "Audio_Systems/audio_mandatory.h"
#include "SoundInstance.h"
#include "SoundEmitter.h"
#include "DSsystem.h"

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
    return unsigned(sound) < enigma::sound_idmax && bool(enigma::sounds[sound]);
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
  return !enigma::sounds[sound]->idle and !enigma::sounds[sound]->playing;
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
