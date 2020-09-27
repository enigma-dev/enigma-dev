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

static std::map<string, string> vidmap;
static std::map<string, string> widmap;

static std::map<string, string> plymap;
static std::map<string, string> gfxmap;

static std::map<string, bool>   fllmap;
static std::map<string, int>    volmap;

void video_loop(mpv_handle *mpv) {
  while (true) {
    mpv_event *event = mpv_wait_event(mpv, -1);
    if (event->event_id == MPV_EVENT_END_FILE) break;
    if (event->event_id == MPV_EVENT_SHUTDOWN) break;
  }
  plymap[std::to_string((unsigned long long)mpv)] = "no";
  mpv_terminate_destroy(mpv);
}

namespace enigma_user {

string video_add(string fname) {
  mpv_handle *mpv = mpv_create();
  string ind = std::to_string((unsigned long long)mpv);
  vidmap.insert(std::make_pair(ind, fname)); 
  return ind;
}

void video_set_option_string(string ind, string option, string value) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(mpv, option.c_str(), value.c_str());
}

void video_play(string ind) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  plymap.erase(ind);
  plymap.insert(std::make_pair(ind, "yes"));
  mpv_initialize(mpv);
  const char *cmd[] = { "loadfile", vidmap.find(ind)->second.c_str(), NULL };
  mpv_command(mpv, cmd);
  std::thread stringhread(video_loop, mpv);
  stringhread.detach();
}

bool video_is_playing(string ind) {
  return (plymap[ind] == "yes");
}

string video_get_graphics_system(string ind) {
  return gfxmap.find(ind)->second;
}

void video_set_graphics_system(string ind, string system) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(mpv, "gpu-api", system.c_str());
  gfxmap.erase(ind);
  gfxmap.insert(std::make_pair(ind, system));
}

bool video_get_fullscreen(string ind) {
  return fllmap.find(ind)->second;
}

void video_set_fullscreen(string ind, bool fullscreen) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(mpv, "fs", fullscreen ? "yes" : "no");
  fllmap.erase(ind);
  fllmap.insert(std::make_pair(ind, fullscreen));
}

int video_get_volume_percent(string ind) {
  return volmap.find(ind)->second;
}

void video_set_volume_percent(string ind, int volume) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(mpv, "volume", std::to_string(volume).c_str());
  volmap.erase(ind);
  volmap.insert(std::make_pair(ind, volume));
}

string video_get_window_identifier(string ind) {
  return widmap.find(ind)->second;
}

void video_set_window_identifier(string ind, string wid) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(mpv, "wid", wid.c_str());
  widmap.erase(ind);
  widmap.insert(std::make_pair(ind, wid));
}

bool video_exists(string ind) {
  return (!vidmap.find(ind)->second.empty());
}

void video_pause(string ind) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  const char *cmd[] = { "cycle", "pause", NULL };
  mpv_command_async(mpv, 0, cmd);
}

void video_stop(string ind) {
  mpv_handle *mpv = (mpv_handle *)stoull(ind, nullptr, 10);
  const char *cmd[] = { "quit", NULL, NULL };
  mpv_command_async(mpv, 0, cmd);
  plymap[ind] = "no";
}

void video_delete(string ind) {
  vidmap.erase(ind);
  widmap.erase(ind);
  plymap.erase(ind);
  gfxmap.erase(ind);
  fllmap.erase(ind);
  volmap.erase(ind);
}

} // namespace enigma_user
