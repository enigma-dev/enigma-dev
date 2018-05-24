/** Copyright (C) 2017 Faissal I. Bensefia
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>, Josh Ventura
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
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

#include "fillin.h"

#include "Platforms/General/PFfilemanip.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"

#include "Platforms/General/PFmain.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/var4.h"

#include <stdio.h>
#include <stdlib.h>  //malloc
#include <stdlib.h>  //getenv and system
#include <sys/resource.h>
#include <sys/stat.h>
#include <time.h>  //clock
#include <unistd.h>
#include <cstdlib>
#include <map>
#include <string>

#include <time.h>

using uint = unsigned;

namespace enigma {
extern bool gameWindowFocused;
extern bool freezeOnLoseFocus;
extern int windowColor;
int current_room_speed;
int cursorInt;
//TODO: Implement pause events
unsigned long current_time_mcs = 0;  // microseconds since the start of the game

void set_room_speed(int rs) { current_room_speed = rs; }
void ENIGMA_events(void);  //TODO: Synchronize this with Windows by putting these two in a single header.

void (*WindowResizedCallback)();
void EnableDrawing();
void DisableDrawing();
void WindowResized();
}  // namespace enigma

namespace enigma_user {

std::string working_directory = "";
extern double fps;
unsigned long current_time = 0;  // milliseconds since the start of the game
unsigned long delta_time = 0;    // microseconds since the last step event
extern double fps;
const int os_type = os_linux;
extern int keyboard_key;
extern int keyboard_lastkey;
extern string keyboard_lastchar;
extern string keyboard_string;

string os_get_config() { return ""; }

int os_get_info() { return 0; }

string os_get_language() {
  char* s = getenv("LANG");
  if (!s || !*s || !s[1]) return "";
  if (!s[2] || s[2] == '.' || s[2] == '_') {
    return string(s, 2);
  }
  return s;  // It won't match people's ISO-639 checks, but it's better than "".
}

string os_get_region() {
  // Most distributions are only aware of location to the extent required to
  // give accurate time information; we can't accurately give an ISO 3166-1
  // compliant string for the device.
  return "";
}

bool os_is_network_connected() {
  return true;  // Please change to false should the year drop below 2010
}

bool os_is_paused() { return false; }
void os_lock_orientation(bool enable) {}
void os_powersave_enable(bool enable) {}

int display_mouse_get_x() { return -1; }
int display_mouse_get_y() { return -1; }
void display_mouse_set(int x, int y) {}
int display_get_width() { return 0; }
int display_get_height() { return 0; }

void window_set_visible(bool visible) {}
int window_get_visible() { return false; }
void window_set_caption(string caption) {}
string window_get_caption() { return ""; }
int window_mouse_get_x() { return -1; }
int window_mouse_get_y() { return -1; }
void window_set_stayontop(bool stay) {}
bool window_get_stayontop() { return false; }
void window_set_sizeable(bool sizeable) {}
bool window_get_sizeable() { return false; }
void window_set_showborder(bool show) {}
bool window_get_showborder() { return false; }
void window_set_showicons(bool show) {}
bool window_get_showicons() { return false; }
void window_set_minimized(bool minimized) {}
void window_set_maximized(bool maximized) {}
bool window_get_minimized() { return false; }
bool window_get_maximized() { return false; }
void window_default(bool center_size) {}
void window_mouse_set(int x, int y) {}
int window_get_x() { return -1; }
int window_get_y() { return -1; }
int window_get_width() { return -1; }
int window_get_height() { return -1; }
bool window_get_freezeonlosefocus() { return false; }
bool window_get_fullscreen() { return false; }
void window_set_position(int x, int y) {}
void window_set_size(unsigned int w, unsigned int h) {}
void window_set_rectangle(int x, int y, int w, int h) {}
void window_set_freezeonlosefocus(bool freeze) {}
void window_set_fullscreen(bool full) {}
int window_get_cursor() { return enigma::cursorInt; }
void window_set_region_scale(double scale, bool adaptwindow) {}
double window_get_region_scale() { return 0; }
void window_set_region_size(int w, int h, bool adaptwindow) {}
int window_get_region_width() { return 0; }
int window_get_region_height() { return 0; }
int window_get_region_width_scaled() { return 0; }
int window_get_region_height_scaled() { return 0; }
void window_set_color(int color) {}
int window_get_color() { return -1; }
void window_center() {}
int window_set_cursor(int c) {
  enigma::cursorInt = c;
  return 0;
}

void io_handle() {}

void clipboard_set_text(string text) {}
string clipboard_get_text() { return ""; }
bool clipboard_has_text() { return false; }

unsigned long get_timer() {  // microseconds since the start of the game
  return enigma::current_time_mcs;
}

}  // namespace enigma_user

int main(int argc, char** argv) {
  // Copy our parameters
  enigma::parameters = new string[argc];
  enigma::parameterc = argc;
  for (int i = 0; i < argc; i++) enigma::parameters[i] = argv[i];

  // Set the working_directory
  char buffer[1024];
  if (getcwd(buffer, sizeof(buffer)) != NULL)
    fprintf(stdout, "Current working dir: %s\n", buffer);
  else
    perror("getcwd() error");
  enigma_user::working_directory = string(buffer);

  // Copy our parameters
  enigma::parameters = new string[argc];
  enigma::parameterc = argc;
  for (int i = 0; i < argc; i++) enigma::parameters[i] = argv[i];

  //Call ENIGMA system initializers; sprites, audio, and what have you
  enigma::initialize_everything();

  struct timespec time_offset;
  struct timespec time_offset_slowing;
  struct timespec time_current;
  clock_gettime(CLOCK_MONOTONIC, &time_offset);
  time_offset_slowing.tv_sec = time_offset.tv_sec;
  time_offset_slowing.tv_nsec = time_offset.tv_nsec;
  int frames_count = 0;

  while (!enigma::game_isending) {
    using enigma::current_room_speed;
    clock_gettime(CLOCK_MONOTONIC, &time_current);
    {
      long passed_mcs = (time_current.tv_sec - time_offset.tv_sec) * 1000000 +
                        (time_current.tv_nsec / 1000 - +time_offset.tv_nsec / 1000);
      passed_mcs = enigma::clamp(passed_mcs, 0, 1000000);
      if (passed_mcs >= 1000000) {  // Handle resetting.

        enigma_user::fps = frames_count;
        frames_count = 0;
        time_offset.tv_sec += passed_mcs / 1000000;
        time_offset_slowing.tv_sec = time_offset.tv_sec;
        time_offset_slowing.tv_nsec = time_offset.tv_nsec;
      }
    }
    long spent_mcs = 0;
    long last_mcs = 0;
    if (current_room_speed > 0) {
      spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec) * 1000000 +
                  (time_current.tv_nsec / 1000 - time_offset_slowing.tv_nsec / 1000);
      spent_mcs = enigma::clamp(spent_mcs, 0, 1000000);
      long remaining_mcs = 1000000 - spent_mcs;
      long needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
      const int catchup_limit_ms = 50;
      if (needed_mcs > remaining_mcs + catchup_limit_ms * 1000) {
        // If more than catchup_limit ms is needed than is remaining, we risk running too fast to catch up.
        // In order to avoid running too fast, we advance the offset, such that we are only at most catchup_limit ms behind.
        // Thus, if the load is consistently making the game slow, the game is still allowed to run as fast as possible
        // without any sleep.
        // And if there is very heavy load once in a while, the game will only run too fast for catchup_limit ms.
        time_offset_slowing.tv_nsec += 1000 * (needed_mcs - (remaining_mcs + catchup_limit_ms * 1000));
        spent_mcs = (time_current.tv_sec - time_offset_slowing.tv_sec) * 1000000 +
                    (time_current.tv_nsec / 1000 - time_offset_slowing.tv_nsec / 1000);
        spent_mcs = enigma::clamp(spent_mcs, 0, 1000000);
        remaining_mcs = 1000000 - spent_mcs;
        needed_mcs = long((1.0 - 1.0 * frames_count / current_room_speed) * 1e6);
      }
      if (remaining_mcs > needed_mcs) {
        const long sleeping_time = std::min((remaining_mcs - needed_mcs) / 5, long(999999));
        usleep(std::max(long(1), sleeping_time));
        continue;
      }
    }

    //TODO: The placement of this code is inconsistent with Win32 because events are handled after, ask Josh.
    unsigned long dt = 0;
    if (spent_mcs > last_mcs) {
      dt = (spent_mcs - last_mcs);
    } else {
      //TODO: figure out what to do here this happens when the fps is reached and the timers start over
      dt = enigma_user::delta_time;
    }
    last_mcs = spent_mcs;
    enigma_user::delta_time = dt;
    enigma::current_time_mcs += enigma_user::delta_time;
    enigma_user::current_time += enigma_user::delta_time / 1000;

    enigma::ENIGMA_events();
    enigma::input_push();

    frames_count++;
  }

  enigma::game_ending();
  return enigma::game_return;
}