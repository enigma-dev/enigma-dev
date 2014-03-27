/** Copyright (C) 2014 Robert B. Colton
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

void splash_show_video(string fname, bool loop);
void splash_show_text(string fname, long delay);
void splash_show_image(string fname, long delay);
void splash_show_web(string url, long delay);

void splash_set_main(bool main);
void splash_set_scale(bool scale);
void splash_set_cursor(bool visible);
void splash_set_color(int col);
void splash_set_caption(string cap);
void splash_set_fullscreen(bool full);
void splash_set_border(bool border);
void splash_set_size(unsigned width, unsigned height);
void splash_set_position(int x, int y);
void splash_set_adapt(bool adapt);
void splash_set_top(bool top);
void splash_set_interrupt(bool interrupt);
void splash_set_stop_key(bool stop);
void splash_set_stop_mouse(bool stop);
void splash_set_close_button(bool show);

}
