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

#include "../General/ASbasic.h"

namespace enigma_user {
	// these functions can not be added for compatibility until we modify the dsound.h header from mingw
	// and redistribute it, wait until someone requests them before adding them if they are not requested
	// we will eventually deprecate them
	enum {
		se_none = 0,
		se_chorus = 1,
		se_echo = 2,
		se_flanger = 3,
		se_gargle = 4,
		se_reverb = 5,
		se_compressor = 6,
		se_equalizer = 7
	};
    void sound_3d_set_sound_cone(int snd, float x, float y, float z, double anglein, double angleout, long voloutside);
	void sound_3d_set_sound_distance(int snd, float mindist, float maxdist);
	void sound_3d_set_sound_position(int snd, float x, float y, float z);
	void sound_3d_set_sound_velocity(int snd, float x, float y, float z);
    void sound_effect_chorus(int snd, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase);
    void sound_effect_compressor(int snd, float gain, float attack, float release, float threshold, float ratio, float delay);
    void sound_effect_echo(int snd, float wetdry, float feedback, float leftdelay, float rightdelay, long pandelay);
    void sound_effect_equalizer(int snd, float center, float bandwidth, float gain);
    void sound_effect_flanger(int snd, float wetdry, float depth, float feedback, float frequency, long wave, float delay, long phase);
    void sound_effect_gargle(int snd, unsigned rate, unsigned wave);
    void sound_effect_reverb(int snd, float gain, float mix, float time, float ratio);
    void sound_effect_set(int snd, int effect);
}