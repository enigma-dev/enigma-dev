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
#ifndef _AS_BASIC__H
#define _AS_BASIC__H

#include <string>
using std::string;

namespace enigma_user
{

/** des
@param sound The index of the sound asset.
@return
**/
bool sound_exists(int sound);
/** des
@param sound The index of the sound asset.
@return
**/
bool sound_play(int sound);
/** des
@param sound The index of the sound asset.
@return
**/
bool sound_loop(int sound);
/** des
@param sound The index of the sound asset.
**/
void sound_stop(int sound);
/** des
**/
void sound_stop_all();
/** des
@param sound The index of the sound asset.
@param volume
**/
void sound_volume(int sound, float volume);
/** des
@param mastervolume
**/
void sound_global_volume(float mastervolume);
/** des
@param sound The index of the sound asset.
**/
void sound_delete(int sound);
/** des
@param sound The index of the sound asset.
@return
**/
bool sound_pause(int sound);
/** des
**/
void sound_pause_all();
/** des
@param sound The index of the sound asset.
@return
**/
bool sound_resume(int sound);
/** des
**/
void sound_resume_all();

/** des
@param sound The index of the sound asset.
@return
**/
float sound_get_length(int sound);
/** des
@param sound The index of the sound asset.
@return
**/
float sound_get_position(int sound);
/** des
@param sound The index of the sound asset.
@param position
**/
void sound_seek(int sound, float position);
/** des
@param position
**/
void sound_seek_all(float position);

/** des
@param sound The index of the sound asset.
@return
**/
bool sound_isplaying(int sound);
/** des
@param sound The index of the sound asset. 
@return
**/
bool sound_ispaused(int sound);

/** des
@param snd The index of the sound asset.
@param loop
**/
void action_sound(int snd, bool loop);

/** des
@param sound The index of the sound asset.
@param value
**/
void sound_pan(int sound, float value);

/** des
@param fname
@param kind
@param preload
@return
**/
int sound_add(string fname, int kind, bool preload);
/** des
@param sound The index of the sound asset.
@param fname
@param kind
@param preload
@return
**/
bool sound_replace(int sound, string fname, int kind, bool preload);
/** des
@param sound The index of the sound asset.
@param fname
@return
**/
inline bool action_replace_sound(int sound, string fname)
{
    return sound_replace(sound, fname, 0, true);
}

/** des
@return
**/
const char* sound_get_audio_error();

void sound_pitch(int sound, float value);
}

#endif
