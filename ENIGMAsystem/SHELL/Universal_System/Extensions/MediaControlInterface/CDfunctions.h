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
using std::string;

namespace enigma_user {
    string MCI_command(string str);
	void cd_open_door();
    void cd_close_door();
    void cd_init();
    unsigned long cd_length();
    unsigned cd_number();
    void cd_play(unsigned first, unsigned last);
    void cd_pause();
	void cd_resume();
	void cd_stop();
    bool cd_paused();
    bool cd_playing();
    bool cd_present();
	unsigned long cd_position();
    void cd_set_position(unsigned long pos);
    void cd_set_track_position(unsigned long pos);
    unsigned cd_track();
    unsigned long cd_track_length(unsigned n);
    unsigned long cd_track_position();
	
	inline void action_cd_play(unsigned first, unsigned last) {
		cd_init(); // needs to be called once in a while and at least once otherwise pause and stop and seeking and tracks are all wrong
		cd_play(first, last);
	}
	
	inline void action_cd_stop() {
		cd_stop();
	}
	
	inline void action_cd_pause() {
		cd_pause();
	}
	
	inline void action_cd_resume() {
		cd_resume();
	}
	
	inline bool action_cd_present() {
		return cd_present();
	}
	
	inline bool action_cd_playing() {
		return cd_playing();
	}
}
