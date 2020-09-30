/*

 MIT License
 
 Copyright © 2020 Samuel Venable
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
*/

#include <string>

namespace enigma_user {

// GameMaker 8.1 splash video functions
void splash_set_main(bool main);
void splash_set_caption(std::string cap);
void splash_set_fullscreen(bool full);
void splash_set_size(unsigned w, unsigned h);
void splash_set_position(int x, int y);
void splash_set_border(bool border);
void splash_set_interupt(bool interupt);
void splash_set_stop_mouse(bool stop);
void splash_show_video(std::string fname, bool loop, std::string window_id = "-1");

// ENIGMA/GM extension video playback functions
void splash_set_volume(int vol);
std::string video_add(std::string fname);
bool video_get_option_was_set(std::string ind, std::string option);
std::string video_get_option_string(std::string ind, std::string option);
void video_set_option_string(std::string ind, std::string option, std::string value);
void video_play(std::string ind);
bool video_is_playing(std::string ind);
std::string video_get_graphics_system(std::string ind);
void video_set_graphics_system(std::string ind, std::string system);
bool video_get_fullscreen(std::string ind);
void video_set_fullscreen(std::string ind, bool fullscreen);
int video_get_volume_percent(std::string ind);
void video_set_volume_percent(std::string ind, int volume);
std::string video_get_window_identifier(std::string ind);
void video_set_window_identifier(std::string ind, std::string wid);
void video_pause(std::string ind);
void video_stop(std::string ind);
bool video_exists(std::string ind);
void video_delete(std::string ind);

} // namespace enigma_user
