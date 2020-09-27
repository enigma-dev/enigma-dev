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

#include "Platforms/General/PFwindow.h"

#include "videoplayer.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <mpv/client.h>

using std::string;

static std::map<video_t, string> vidmap;
static std::map<video_t, wid_t>  widmap;
static std::map<video_t, string> plymap;

void video_loop(mpv_handle *ctx) {
  #ifdef _WIN32
  video_t ind = std::to_string((unsigned long long)ctx);
  wid_t wid   = widmap.find(ind)->second;
  HWND window = (HWND)stoull(wid, nullptr, 10);
  #endif
  while (true) {
    #ifdef _WIN32
    LONG_PTR style = GetWindowLongPtr(window, GWL_STYLE);
    if (!(style & (WS_CLIPCHILDREN | WS_CLIPSIBLINGS)))
      SetWindowLongPtr(window, GWL_STYLE, style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    #endif
    mpv_event *event = mpv_wait_event(ctx, 10000);
    if (event->event_id == MPV_EVENT_END_FILE) break;
    if (event->event_id == MPV_EVENT_SHUTDOWN) break;
  }
  plymap[std::to_string((unsigned long long)ctx)] = "no";
  mpv_terminate_destroy(ctx);
}

namespace enigma_user {

video_t video_add(string fname) {
  mpv_handle *ctx = mpv_create();
  string ind = std::to_string((unsigned long long)ctx);
  vidmap.insert(std::make_pair(ind, fname)); 
  return ind;
}

void video_play(video_t ind, wid_t wid) {
  mpv_handle *ctx = (mpv_handle *)stoull(ind, nullptr, 10);
  mpv_set_option_string(ctx, "input-default-bindings", "no");
  mpv_set_option_string(ctx, "config", "no");
  mpv_set_option_string(ctx, "osc", "no");
  mpv_set_option_string(ctx, "wid", wid.c_str());
  widmap.insert(std::make_pair(ind, wid));
  plymap.erase(ind);
  plymap.insert(std::make_pair(ind, "yes"));
  mpv_initialize(ctx);
  const char *cmd[] = { "loadfile", vidmap.find(ind)->second.c_str(), NULL };
  mpv_command(ctx, cmd);
  std::thread stringhread(video_loop, ctx);
  stringhread.detach();
}

bool video_is_playing(video_t ind) {
  return (plymap[ind] == "yes");
}

wid_t video_get_winid(video_t ind) {
  return widmap.find(ind)->second;
}

bool video_exists(video_t ind) {
  return (!vidmap.find(ind)->second.empty());
}
 
void video_pause(video_t ind) {
  mpv_handle *ctx = (mpv_handle *)stoull(ind, nullptr, 10);
  const char *cmd[] = { "cycle", "pause", NULL };
  mpv_command_async(ctx, 0, cmd);
}

void video_stop(video_t ind) {
  mpv_handle *ctx = (mpv_handle *)stoull(ind, nullptr, 10);
  const char *cmd[] = { "quit", NULL, NULL };
  mpv_command_async(ctx, 0, cmd);
  plymap[ind] = "no";
}

void video_delete(video_t ind) {
  vidmap.erase(ind);
  widmap.erase(ind);
}

} // namespace enigma_user
