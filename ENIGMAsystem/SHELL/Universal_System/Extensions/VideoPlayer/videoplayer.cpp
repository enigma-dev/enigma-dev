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

#include <climits>
#include <fstream>
#include <cstdio>
#include <string>
#include <thread>
#include <chrono>
#include <map>

#include "videoplayer.h"

#ifdef _WIN32
  #include <windows.h>
#endif

#include <mpv/client.h>

using std::string;

struct VideoData {
  mpv_handle *mpv;
  std::map<string, string> option;
  string window_id;
  int volume;
  bool is_paused;
  bool is_playing;
  bool is_fullscreen;
};

static std::map<string, VideoData> videos;
static string video;

static string splash_get_window    = "-1";
static int splash_get_volume       = 100;
static string splash_get_caption   = "";
static unsigned splash_get_scale   = 0;

static bool splash_get_fullscreen  = false;
static bool splash_get_main        = true;
static bool splash_get_adapt       = true;
static bool splash_get_border      = true;

static bool splash_get_top         = true;
static bool splash_get_interupt    = true;
static bool splash_get_stop_mouse  = true;

static int splash_get_x            = INT_MAX;
static int splash_get_y            = INT_MAX;
static unsigned splash_get_width   = 640;
static unsigned splash_get_height  = 480;

#ifdef __APPLE__
  #ifdef __MACH__
     extern "C" const char *cocoa_window_get_contentview(const char *window);
  #endif
#endif

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

void splash_set_window(string wid) {
  splash_get_window = wid;
}

void splash_set_main(bool main) {
  splash_get_main = main;
}

void splash_set_caption(string cap) {
  splash_get_caption = cap;
}

void splash_set_fullscreen(bool full) {
  splash_get_fullscreen = full;
}

void splash_set_size(unsigned w, unsigned h) {
  splash_get_width  = w;
  splash_get_height = h;
}

void splash_set_position(int x, int y) {
  splash_get_x = x;
  splash_get_y = y;
}

void splash_set_border(bool border) {
  splash_get_border = border;
}

void splash_set_interupt(bool interupt) {
  splash_get_interupt = interupt;
}

void splash_set_stop_mouse(bool stop) {
  splash_get_stop_mouse = stop;
}

void splash_set_scale(unsigned scale) {
  splash_get_scale = scale;
}

void splash_set_adapt(bool adapt) {
  splash_get_adapt = adapt;
}

void splash_set_volume(int vol) {
  splash_get_volume = vol;
}

void splash_show_video(string fname, bool loop) {
  string wid, wstr, hstr, xstr, ystr, size, 
    pstn, geom, flls, brdr, ontp, lpng, adpt;

  if (splash_get_main) { // embeds inside game window
    #ifdef __APPLE__
      #ifdef __MACH__
        wid = cocoa_window_get_contentview(splash_get_window.c_str());
      #endif
    #else
      wid = splash_get_window;
    #endif
  } else { 
    // creates a new window to play splash
    // -1 is guaranteed to be not a window
    wid = "-1";
  }

  splash_get_main = (wid != "-1");
  splash_get_interupt = splash_get_main ? true : splash_get_interupt;
  flls = splash_get_main ? "no" : (splash_get_fullscreen ? "yes" : "no");
  brdr = splash_get_main ? "no" : (splash_get_border ? "yes" : "no");
  ontp = splash_get_main ? "no" : (splash_get_top ? "yes" : "no");
  adpt = splash_get_main ? "no" : (splash_get_adapt ? "yes" : "no");
  lpng = loop ? "yes" : "no";

  wstr = std::to_string(splash_get_width);
  hstr = std::to_string(splash_get_height);
  xstr = std::to_string(splash_get_x);
  ystr = std::to_string(splash_get_y);
  size = wstr + "x" + hstr;
  if (splash_get_x == INT_MAX || splash_get_y == INT_MAX) {
    pstn = "50%+50%";
    geom = size + "+" + pstn;
  } else {
    pstn = xstr + "+" + ystr;
    geom = size + "+" + pstn;
  }

  if (video_exists(video)) {
    video_delete(video);
  }
 
  video = video_add(fname);
  std::remove("input.conf");

  std::ofstream file;
  file.open ("input.conf");
  file << "CLOSE_WIN ignore\n";
  if (splash_get_stop_mouse) {
    file << "MBTN_LEFT quit\n";
    file << "MBTN_RIGHT quit\n";
  }
  file.close();

  video_set_option_string(video, "input-conf", "input.conf");
  video_set_option_string(video, "volume", std::to_string(splash_get_volume));
  video_set_option_string(video, "video-zoom", std::to_string(splash_get_scale));
  video_set_option_string(video, "input-default-bindings", "no");
  video_set_option_string(video, "title", splash_get_caption);
  video_set_option_string(video, "fs", flls);
  video_set_option_string(video, "border", brdr);
  video_set_option_string(video, "keepaspect-window", adpt);
  video_set_option_string(video, "taskbar-progress", "no");
  video_set_option_string(video, "ontop", ontp);
  video_set_option_string(video, "geometry", geom);
  video_set_option_string(video, "config", "no");
  video_set_option_string(video, "loop", lpng);
  video_set_option_string(video, "osc", "no");
  video_set_option_string(video, "wid", wid);

  video_play(video);
  if (splash_get_interupt) {
    while (video_is_playing(video)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      #ifdef _WIN32
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {  
          TranslateMessage(&msg);
          DispatchMessage(&msg);
          if (splash_get_stop_mouse && wid != "-1" &&
            wid == std::to_string((unsigned long long)msg.hwnd) &&
            (msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN)) {
            video_stop(video);
          }
        }
      #endif
    }
  }
}

string splash_get_video() {
  return video;
}

string video_add(string fname) {
  VideoData data;
  data.mpv           = mpv_create();
  data.window_id     = "-1";
  data.volume        = 100;
  data.is_paused     = false;
  data.is_playing    = false;
  data.is_fullscreen = false;
  videos.insert(std::make_pair(fname, data)); 
  return fname;
}

bool video_get_option_was_set(string ind, string option) {
  return (videos[ind].option.find(option) != videos[ind].option.end());
} 

string video_get_option_string(string ind, string option) {
  bool was_set = video_get_option_was_set(ind, option);
  return was_set ? videos[ind].option.find(option)->second : "";
}

void video_set_option_string(string ind, string option, string value) {
  mpv_set_option_string(videos[ind].mpv, option.c_str(), value.c_str());
  videos[ind].option[option] = value;
}

string video_get_property_string(string ind, string property) {
  return mpv_get_property_string(videos[ind].mpv, property.c_str());
}

void video_play(string ind) {
  videos[ind].is_playing = true;
  mpv_initialize(videos[ind].mpv);
  const char *cmd[] = { "loadfile", ind.c_str(), NULL };
  mpv_command(videos[ind].mpv, cmd);
  std::thread stringhread(video_loop, ind, videos[ind].mpv);
  stringhread.detach();
}

bool video_is_paused(string ind) {
  if (videos.find(ind) != videos.end())
    return videos[ind].is_paused;
  return false;
}

bool video_is_playing(string ind) {
  if (videos.find(ind) != videos.end())
    return videos[ind].is_playing;
  return false;
}

bool video_get_fullscreen(string ind) {
  if (video_get_option_was_set(ind, "fs"))
    return videos[ind].is_fullscreen;
  return false;
}

void video_set_fullscreen(string ind, bool fullscreen) {
  videos[ind].is_fullscreen = fullscreen;
  mpv_set_option_string(videos[ind].mpv, "fs", std::to_string(fullscreen).c_str());
}

int video_get_volume_percent(string ind) {
  if (video_get_option_was_set(ind, "volume"))
    return videos[ind].volume;
  return 100;
}

void video_set_volume_percent(string ind, int volume) {
  videos[ind].volume = volume;
  mpv_set_option_string(videos[ind].mpv, "volume", std::to_string(volume).c_str());
}

string video_get_window_identifier(string ind) {
  if (video_get_option_was_set(ind, "wid"))
    return videos[ind].window_id;
  return "-1";
}

void video_set_window_identifier(string ind, string wid) {
  #ifdef __APPLE__ 
    #ifdef __MACH__ 
      wid = cocoa_window_get_contentview(wid.c_str());
    #endif
  #endif
  mpv_set_option_string(videos[ind].mpv, "wid", wid.c_str());
}

bool video_exists(string ind) {
  return (videos.find(ind) != videos.end());
}

void video_pause(string ind) {
  const char *cmd[] = { "cycle", "pause", NULL };
  mpv_command_async(videos[ind].mpv, 0, cmd);
  videos[ind].is_paused = !video_is_paused(ind);
}

void video_seek(string ind, double seek) {
  static string arg;
  arg = std::to_string(seek);
  const char *cmd[] = { "seek", arg.c_str(), NULL };
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

unsigned video_get_width(string ind) {
  long long result;
  mpv_get_property(videos[ind].mpv, "width", MPV_FORMAT_INT64, &result);
  return (unsigned)result;
}

unsigned video_get_height(string ind) {
  long long result;
  mpv_get_property(videos[ind].mpv, "height", MPV_FORMAT_INT64, &result);
  return (unsigned)result;
}

double video_get_duration(string ind) {
  double result;
  mpv_get_property(videos[ind].mpv, "duration", MPV_FORMAT_DOUBLE, &result);
  return result;
}

} // namespace enigma_user
