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

#include "Platforms/platforms_mandatory.h"
#include "Platforms/General/include.h"

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

namespace enigma {
  bool initGameWindow() { return true; }
  void destroyWindow() {}
  void initInput() {}
  void handleInput() { input_push(); }
  void EnableDrawing(void* handle) {};
  void DisableDrawing(void* handle) {};
  int handleEvents() { return 0; }
}

namespace enigma_user {

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

void os_lock_orientation(bool enable) {}
void os_powersave_enable(bool enable) {}

int display_mouse_get_x() { return -1; }
int display_mouse_get_y() { return -1; }
void display_mouse_set(int x, int y) {}
int display_get_x() { return 0; }
int display_get_y() { return 0; }
int display_get_width() { return 0; }
int display_get_height() { return 0; }

void window_set_visible(bool visible) {}
int window_get_visible() { return false; }
void window_set_caption(const string &caption) {}
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
void window_mouse_set(int x, int y) {}
int window_get_x() { return -1; }
int window_get_y() { return -1; }
int window_get_width() { return -1; }
int window_get_height() { return -1; }
bool window_get_fullscreen() { return false; }
void window_set_position(int x, int y) {}
void window_set_size(unsigned int w, unsigned int h) {}
void window_set_fullscreen(bool full) {}
void window_set_rectangle(int, int, int, int) {}
int window_set_cursor(int c) {
  enigma::cursorInt = c;
  return 0;
}

void clipboard_set_text(string text) {}
string clipboard_get_text() { return ""; }
bool clipboard_has_text() { return false; }


}  // namespace enigma_user
