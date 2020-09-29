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
#include <thread>
#include <map>

#include "videoplayer.h"

#include <mpv/client.h>

using std::string;

struct VideoData {
  mpv_handle *mpv;
  std::string window_id;
  std::string gpu_api;
  int volume;
  bool is_playing;
  bool is_fullscreen;
};

static std::map<string, VideoData> videos;

static void video_loop(string ind, mpv_handle *mpv) {
  while (true) {
    mpv_event *event = mpv_wait_event(mpv, -1);
    if (event->event_id == MPV_EVENT_END_FILE) break;
    if (event->event_id == MPV_EVENT_SHUTDOWN) break;
  }
  videos[ind].is_playing = false;
  mpv_terminate_destroy(mpv);
}

namespace enigma_user {

string video_add(string fname) {
  VideoData data;
  data.mpv            = mpv_create();
  data.window_id      = "-1";
  data.gpu_api        = "opengl";
  data.volume         = 100;
  data.is_playing     = false;
  data.is_fullscreen = false;
  videos.insert(std::make_pair(fname, data)); 
  return fname;
}

void video_set_option_string(string ind, string option, string value) {
  mpv_set_option_string(videos[ind].mpv, option.c_str(), value.c_str());
}

void video_play(string ind) {
  videos[ind].is_playing = true;
  mpv_initialize(videos[ind].mpv);
  const char *cmd[] = { "loadfile", ind.c_str(), NULL };
  mpv_command(videos[ind].mpv, cmd);
  std::thread stringhread(video_loop, ind, videos[ind].mpv);
  stringhread.detach();
}

bool video_is_playing(string ind) {
  return videos[ind].is_playing;
}

string video_get_graphics_system(string ind) {
  return videos[ind].gpu_api;
}

void video_set_graphics_system(string ind, string system) {
  videos[ind].gpu_api = system;
  mpv_set_option_string(videos[ind].mpv, "gpu-api", system.c_str());
}

bool video_get_fullscreen(string ind) {
  return videos[ind].is_fullscreen;
}

void video_set_fullscreen(string ind, bool fullscreen) {
  videos[ind].is_fullscreen = fullscreen;
  mpv_set_option_string(videos[ind].mpv, "fs", std::to_string(fullscreen).c_str());
}

int video_get_volume_percent(string ind) {
  return videos[ind].volume;
}

void video_set_volume_percent(string ind, int volume) {
  videos[ind].volume = volume;
  mpv_set_option_string(videos[ind].mpv, "volume", std::to_string(volume).c_str());
}

string video_get_window_identifier(string ind) {
  return videos[ind].window_id;
}

void video_set_window_identifier(string ind, string wid) {
  videos[ind].window_id = wid;
  mpv_set_option_string(videos[ind].mpv, "wid", wid.c_str());
}

bool video_exists(string ind) {
  return (videos.find(ind) != videos.end());
}

void video_pause(string ind) {
  const char *cmd[] = { "cycle", "pause", NULL };
  mpv_command_async(videos[ind].mpv, 0, cmd);
}

void video_stop(string ind) {
  const char *cmd[] = { "quit", NULL, NULL };
  mpv_command_async(videos[ind].mpv, 0, cmd);
  videos[ind].is_playing = false;
}

void video_delete(string ind) {
  videos.erase(ind);
}

} // namespace enigma_user
