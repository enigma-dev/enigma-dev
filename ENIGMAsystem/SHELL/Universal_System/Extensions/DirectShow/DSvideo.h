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
	void action_show_video(string fname, bool windowed, bool loop);
	int video_add(string fname);
	void video_delete(int id);
	bool video_exists(int id);
	void video_start(int id, bool loop);
	void video_pause(int id);
	void video_stop(int id);
	void video_set_seek(int id, long position);
	void video_set_rate(int id, double rate);
	void video_set_fullscreen(int id, bool fullscreen);
	void video_set_window_region(int id, long x, long y, long w, long h);
	void video_set_destination_region(int id, long x, long y, long w, long h);
	void video_set_source_region(int id, long x, long y, long w, long h);
	void video_set_scale(int id, bool scale);
	long video_get_seek(int id);
	double video_get_rate(int id);
	long video_get_duration(int id);
	long video_get_width(int id);
	long video_get_height(int id);
	double video_get_frame_time(int id);
	int video_get_frame_rate(int id);
}