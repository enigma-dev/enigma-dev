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
#include <map>

#ifndef __APPLE__
  #include <chrono>
#endif

#include "videoplayer.h"

#ifdef _WIN32
  #include <windows.h>
#endif

#if defined(__linux__) || defined(__FreeBSD__)
  #include <X11/Xlib.h>
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

static bool splash_get_stop_mouse  = true;
static bool splash_get_stop_key  = true;

#ifdef __APPLE__
  #ifdef __MACH__
     extern "C" void cocoa_show_cursor();
     extern "C" const char *cocoa_window_get_contentview(const char *window);
     extern "C" void cocoa_process_run_loop(const char *video, const char *window, bool close_mouse, bool close_key);
     void video_stop(const char *ind) { enigma_user::video_stop(ind); }
  #endif
#endif

#if defined(__linux__) || defined(__FreeBSD__)
  static inline int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
    return 0;
  }

  static inline int XIOErrorHandlerImpl(Display *display) {
    return 0;
  }

  static inline void SetErrorHandlers() {
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);
  }

  static inline unsigned long XGetActiveWindow(Display *display) {
    SetErrorHandlers();
    unsigned char *prop;
    unsigned long property;
    Atom actual_type, filter_atom;
    int actual_format, status;
    unsigned long nitems, bytes_after;
    int screen = XDefaultScreen(display);
    window = RootWindow(display, screen);
    filter_atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", True);
    status = XGetWindowProperty(display, window, filter_atom, 0, 1000, False,
    AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
    if (status == Success && prop != NULL) {
      property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
      XFree(prop);
    }
    return property;
  }
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

void splash_set_stop_mouse(bool stop) {
  splash_get_stop_mouse = stop;
}

void splash_set_stop_key(bool stop) {
  splash_get_stop_key = stop;
}

void splash_set_volume(int vol) {
  splash_get_volume = vol;
}

void splash_show_video(string fname, bool loop) {
  string wid, lpng;

  #ifdef __APPLE__
    #ifdef __MACH__
      wid = cocoa_window_get_contentview(splash_get_window.c_str());
    #endif
  #else
    wid = splash_get_window;
  #endif

  lpng = loop ? "yes" : "no";

  if (video_exists(video)) {
    video_delete(video);
  }
 
  video = video_add(fname);
  #if defined(__linux__) || defined(__FreeBSD__)
    std::remove("/tmp/input.conf");
    std::ofstream file;
    file.open ("/tmp/input.conf");
    file << "CLOSE_WIN ignore\n";
    if (splash_get_stop_mouse) {
      file << "MBTN_LEFT quit\n";
      file << "MBTN_RIGHT quit\n";
    }
    file.close();
    video_set_option_string(video, "input-conf", "/tmp/input.conf");
  #endif

  video_set_option_string(video, "volume", std::to_string(splash_get_volume));
  video_set_option_string(video, "input-default-bindings", "no");
  video_set_option_string(video, "taskbar-progress", "no");
  video_set_option_string(video, "config", "no");
  video_set_option_string(video, "loop", lpng);
  video_set_option_string(video, "osc", "no");
  video_set_option_string(video, "wid", wid);

  video_play(video);
  bool hidden = false;
  while (video_is_playing(video)) {
    #ifdef __APPLE__
      #ifdef __MACH__
        cocoa_process_run_loop(video.c_str(), wid.c_str(),
          splash_get_stop_mouse, splash_get_stop_key);
      #endif
    #else
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      #if defined(__linux__) || defined(__FreeBSD__)
        XEvent report;
        Display *disp = XOpenDisplay(NULL);
        XNextEvent(dis, &report);
        if (wid == std::to_string(XGetActiveWindow(disp)) {
          if (report.type == KeyPress) {
            if (XLookupKeysym(&report.xkey, 0) == XK_Escape) {
              video_stop(video);
            }
          }
        }
        XCloseDisplay(disp);
      #endif
      #ifdef _WIN32
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
          if (wid == std::to_string((unsigned long long)msg.hwnd)) {
            if (splash_get_stop_mouse) {
              if (msg.message == WM_LBUTTONDOWN || msg.message == WM_RBUTTONDOWN ||
                msg.message == WM_NCLBUTTONDOWN || msg.message == WM_NCRBUTTONDOWN) {
                video_stop(video);
              }
            }
            if (splash_get_stop_key) {
              if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) {
                video_stop(vide);
              }
            }
            POINT point;
            GetCursorPos(&point);
            if (msg.hwnd == WindowFromPoint(point)) {  
              if (!hidden) {
                ShowCursor(false);
                hidden = true;
              }
            } else {
              if (hidden) {
                ShowCursor(true);
                hidden = false;
              }
            }
          }
        }
      #endif
    #endif
  }
  #ifdef _WIN32
  if (hidden) {
    ShowCursor(true);
    hidden = false;
  }
  #endif
  #ifdef __APPLE__
    #ifdef __MACH__
      cocoa_show_cursor();
    #endif
  #endif
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
  return "0";
}

void video_set_window_identifier(string ind, string wid) {
  wid = std::to_string(strtoull(wid.c_str(), NULL, 10));
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
