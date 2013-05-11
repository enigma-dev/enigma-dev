/** Copyright (C) 2008-2013 Cheeseboy, Robert B. Colton 
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

// Memory Shit //
bool sound_exists(int sound);
int sound_add(std::string fname, int kind, bool preload);
void sound_delete(int sound);
bool sound_replace(int sound, std::string fname, int kind, bool preload);

// Baics //
bool sound_play(int sound);
#define sound_resume sound_play
bool sound_loop(int sound);
void sound_stop(int sound);
void sound_stop_all();
bool sound_pause(int sound);
void sound_pause_all();
void sound_resume_all();

// Gets //
bool sound_isplaying(int sound);
bool sound_ispaused(int sound);
float sound_get_position(int sound);
float sound_get_length(int sound);
int sound_get_samplerate(int sound);
int sound_get_channels(int sound);

// Sets //
void sound_global_volume(float volume);
void sound_set_volume(int sound, float volume);
void sound_seek(int sound, float position);
void sound_seek_all(float position);
void sound_set_pitch(int sound, float pitch);
void sound_3d_set_sound_position(int sound,int x,int y,int z);
void sound_3d_set_sound_distance(int sound, float mindist, float maxdist);
int sound_get_x(int sound);
int sound_get_y(int sound);
int sound_get_z(int sound);
float sound_get_volume(int sound);
float sound_get_pitch(int sound);
