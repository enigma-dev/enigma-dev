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

// ***** New Sound System *****

namespace enigma_user
{

enum { 
  audio_falloff_exponent_distance,
  audio_falloff_exponent_distance_clamped,
  audio_falloff_inverse_distance,
  audio_falloff_inverse_distance_clamped,
  audio_falloff_linear_distance,
  audio_falloff_linear_distance_clamped,
  audio_falloff_none,
  audio_old_system,
  audio_new_system
};

/** des
@param index The index of the audio asset.
@return 
**/
bool audio_exists(int index);
/** des
@param index The index of the audio asset.
@return 
**/
bool audio_is_playing(int index);

/** des
@param index The index of the audio asset.
@param loop Whether or not to loop the sound at the end.
**/
void audio_play_music(int index, bool loop);
/** des
**/
void audio_pause_music();
/** des
**/
void audio_resume_music();
/** des
**/
void audio_stop_music(); 

/** des
@param index The index of the audio asset.
@param priority The priority of the sound in queue.
@param loop Whether or not to loop the sound at the end.
@return
**/
int audio_play_sound(int index, double priority, bool loop);
/** des
@param sound The index of the audio asset.
@param x
@param y
@param z
@param falloff_ref
@param falloff_max
@param falloff_factor
@param loop Whether or not to loop the sound at the end.
@param priority The priority of the sound in queue.
@return
**/
int audio_play_sound_at(int sound, double x, double y, double z, int falloff_ref, double falloff_max, double falloff_factor, bool loop, double priority);
/** des
@param index The index of the audio asset.
**/
void audio_pause_sound(int index);
/** des
@param index The index of the audio asset.
**/
void audio_resume_sound(int index);
/** des
@param index The index of the audio asset.
**/
void audio_stop_sound(int index);
/** des
**/
void audio_pause_all();
/** des
**/
void audio_resume_all();
/** des
**/
void audio_stop_all();

/** des
@param offset
**/
void audio_music_seek(double offset);
/** des
@param index The index of the audio asset.
@param offset 
**/
void audio_sound_seek(int index, double offset);
/** des
@return
**/
double audio_music_offset();
/** des
@param index The index of the audio asset.
@return
**/
double audio_sound_offset(int index);

/** des
@param lookat_x
@param lookat_y
@param lookat_z
@param up_x
@param up_y
@param up_z
**/
void audio_listener_orientation(double lookat_x, double lookat_y, double lookat_z, double up_x, double up_y, double up_z);
/** des
@param x
@param y
@param z
**/
void audio_listener_position(double x, double y, double z);
/** des
@param vx
@param vy
@param vz
**/
void audio_listener_velocity(double vx, double vy, double vz);

/** des
@param index The index of the audio asset.
@return
**/
double audio_sound_length(int index);
/** des
@param volume
@param time
**/
void audio_master_gain(float volume, double time);
/** des
@param volume
@param time
**/
void audio_music_gain(float volume, double time);
/** des
@param index The index of the audio asset.
@param volume
@param time
**/
void audio_sound_gain(int index, float volume, double time);

/** des
@param index The index of the audio asset.
@return
**/
int audio_get_type(int index);
/** des
@param num
**/
void audio_channel_num(int num);

/** des
@return
**/
int audio_system();
/** des
@param fname
@param type
@return
**/
int audio_add(string fname, int type);
/** des
@param sound The index of the audio asset.
**/
void audio_delete(int sound);
/** des
@param model
**/
void audio_falloff_set_model(int model);

/** des
@return
**/
int audio_emitter_create();
/** des
@param index The index of the audio emitter.
@return
**/
bool audio_emitter_exists(int index);
/** des
@param emitter The index of the audio emitter.
@param falloff_ref
@param falloff_max
@param falloff_factor
**/
void audio_emitter_falloff(int emitter, double falloff_ref, double falloff_max, double falloff_factor);
/** des
@param emitter The index of the audio emitter.
**/
void audio_emitter_free(int emitter);
/** des
@param emitter The index of the audio emitter.
@param gain
**/
void audio_emitter_gain(int emitter, double gain);
/** des
@param emitter The index of the audio emitter.
@param pitch
**/
void audio_emitter_pitch(int emitter, double pitch);
/** des
@param emitter The index of the audio emitter.
@param x
@param y
@param z
**/
void audio_emitter_position(int emitter, double x, double y, double z);
/** des
@param emitter The index of the audio emitter.
@param vx
@param vy
@param vz
**/
void audio_emitter_velocity(int emitter, double vx, double vy, double vz);
/** des
@param emitter The index of the audio emitter.
@param sound
@param loop
@param priority
**/
void audio_play_sound_on(int emitter, int sound, bool loop, double priority);

}

