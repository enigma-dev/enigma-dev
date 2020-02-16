/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>
*** Copyright (C) 2013 Robert B. Colton
*** Copyright (C) 2014 Seth N. Hetu
*** Copyright (C) 2015 Harijs Grinbergs
*** Copyright (C) 2020 Samuel Venable
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

#include "XLIBmain.h"
#include "LINUXjoystick.h"
#include "XLIBwindow.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/var4.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>

#include <sys/types.h>  //getpid
#include <unistd.h>
#include <string>

using std::string;

using namespace enigma::x11;

namespace enigma_user {
const int os_type = os_linux;
}  // namespace enigma_user

namespace enigma {

void (*WindowResizedCallback)();
void WindowResized();

int handleEvents() {
  while (XQLength(enigma::x11::disp) || XPending(enigma::x11::disp)) {
    XEvent e;
    XNextEvent(disp, &e);
    int gk;
    unsigned char actualKey;
    switch (e.type) {
      case KeyPress: {
        gk = XLookupKeysym(&e.xkey, 0);
        if (gk == NoSymbol) continue;

        if (!(gk & 0xFF00))
          actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0xFF]);
        else
          actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0x1FF]);
        {  // Set keyboard_lastchar. Seems to work without
          char str[1];
          int len = XLookupString(&e.xkey, str, 1, NULL, NULL);
          if (len > 0) {
            enigma_user::keyboard_lastchar = string(1, str[0]);
            if (actualKey == enigma_user::vk_backspace) {
              if (!enigma_user::keyboard_string.empty()) enigma_user::keyboard_string.pop_back();
            } else {
              enigma_user::keyboard_string += enigma_user::keyboard_lastchar;
            }
          }
        }
        enigma_user::keyboard_lastkey = actualKey;
        enigma_user::keyboard_key = actualKey;
        if (enigma::last_keybdstatus[actualKey] == 1 && enigma::keybdstatus[actualKey] == 0) {
          enigma::keybdstatus[actualKey] = 1;
          continue;
        }
        enigma::last_keybdstatus[actualKey] = enigma::keybdstatus[actualKey];
        enigma::keybdstatus[actualKey] = 1;
        continue;
      }
      case KeyRelease: {
        enigma_user::keyboard_key = 0;
        gk = XLookupKeysym(&e.xkey, 0);
        if (gk == NoSymbol) continue;

        if (!(gk & 0xFF00))
          actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0xFF]);
        else
          actualKey = enigma_user::keyboard_get_map((int)enigma::keymap[gk & 0x1FF]);

        enigma::last_keybdstatus[actualKey] = enigma::keybdstatus[actualKey];
        enigma::keybdstatus[actualKey] = 0;
        continue;
      }
      case ButtonPress: {
        if (e.xbutton.button < 4)
          enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4 - e.xbutton.button] = 1;
        else
          switch (e.xbutton.button) {
            case 4:
              enigma_user::mouse_vscrolls++;
              break;
            case 5:
              enigma_user::mouse_vscrolls--;
              break;
            case 6:
              enigma_user::mouse_hscrolls++;
              break;
            case 7:
              enigma_user::mouse_hscrolls--;
              break;
            default:;
          }
        continue;
      }
      case ButtonRelease: {
        if (e.xbutton.button < 4)
          enigma::mousestatus[e.xbutton.button == 1 ? 0 : 4 - e.xbutton.button] = 0;
        else
          switch (e.xbutton.button) {
            case 4:
              enigma_user::mouse_vscrolls++;
              break;
            case 5:
              enigma_user::mouse_vscrolls--;
              break;
            case 6:
              enigma_user::mouse_hscrolls++;
              break;
            case 7:
              enigma_user::mouse_hscrolls--;
              break;
            default:;
          }
        continue;
      }
      case ConfigureNotify: {
        if (WindowResizedCallback != NULL) {
          windowX = e.xconfigure.x;
          windowY = e.xconfigure.y;
          windowWidth = e.xconfigure.width;
          windowHeight = e.xconfigure.height;
          compute_window_scaling();
          WindowResizedCallback();
        }
        continue;
      }
      case FocusIn:
        input_initialize();
        init_joysticks();
        game_window_focused = true;
        pausedSteps = 0;
        continue;
      case FocusOut:
        game_window_focused = false;
        continue;
      case ClientMessage:
        if ((Atom)e.xclient.data.l[0] ==
            wm_delwin)  //For some reason, this line warns whether we cast to unsigned or not.
          return 1;
        //else fall through
      default:
#ifdef DEBUG_MODE
        DEBUG_MESSAGE("Unhandled xlib event: " + std::to_string(e.type), MESSAGE_TYPE::M_INFO);
#endif
        continue;
    }
    //Move/Resize = ConfigureNotify
    //Min = UnmapNotify
    //Restore = MapNotify
  }

  return 0;
}

void initInput() {
  initkeymap();
  init_joysticks();
}

void handleInput() {
  handle_joysticks();
  input_push();
}

}  // namespace enigma

/*

Copyright (C) 2020 Samuel Venable

These features were contributed by Samuel Venable and 
may be relicensed under MIT for those who want to use 
it in their stuff permissively outside of enigma-dev;
it's the least he can do since it wasn't easily done:

int displayX;
int displayY;
int displayWidth;
int displayHeight;

int displayXGetter;
int displayYGetter;
int displayWidthGetter;
int displayHeightGetter;

void display_get_position(bool i, int *result);
void display_get_size(bool i, int *result);

int display_get_x();
int  display_get_y();
int  display_get_width();
int display_get_height();

*/

namespace {

int displayX            = -1;
int displayY            = -1;
int displayWidth        = -1;
int displayHeight       = -1;

int displayXGetter      = -1;
int displayYGetter      = -1;
int displayWidthGetter  = -1;
int displayHeightGetter = -1;

void display_get_position(bool i, int *result) {
  *result = 0; Rotation original_rotation; 
  Window root = DefaultRootWindow(enigma::x11::disp);
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, root);
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  if (XineramaIsActive (enigma::x11::disp)) {
    int m = 0; XineramaScreenInfo *xrrp = XineramaQueryScreens(enigma::x11::disp, &m);
    if (!i) *result = xrrp[original_size_id].x_org;
    else if (i) *result = xrrp[original_size_id].y_org;
    XFree(xrrp);
  }
}

void display_get_size(bool i, int *result) {
  *result = 0; int num_sizes; Rotation original_rotation; 
  Window root = DefaultRootWindow(enigma::x11::disp);
  int screen = XDefaultScreen(enigma::x11::disp);
  XRRScreenConfiguration *conf = XRRGetScreenInfo(enigma::x11::disp, root);
  SizeID original_size_id = XRRConfigCurrentConfiguration(conf, &original_rotation);
  if (XineramaIsActive (enigma::x11::disp)) {
    XRRScreenSize *xrrs = XRRSizes(enigma::x11::disp, screen, &num_sizes);
    if (!i) *result = xrrs[original_size_id].width;
    else if (i) *result = xrrs[original_size_id].height;
  } else if (!i) *result = XDisplayWidth(enigma::x11::disp, screen);
  else if (i) *result = XDisplayHeight(enigma::x11::disp, screen);
}

} // anonymous namespace

namespace enigma_user {

int display_get_x() {
  if (displayXGetter == displayX && displayX != -1)
    return displayXGetter;
  display_get_position(false, &displayXGetter);
  int result = displayXGetter;
  displayX = result;
  return result;
}

int display_get_y() { 
  if (displayYGetter == displayY && displayY != -1)
    return displayYGetter;
  display_get_position(true, &displayYGetter);
  int result = displayYGetter;
  displayY = result;
  return result;
}

int display_get_width() {
  if (displayWidthGetter == displayWidth && displayWidth != -1) 
    return displayWidthGetter;
  display_get_size(false, &displayWidthGetter);
  int result = displayWidthGetter;
  displayWidth = result;
  return result;
}

int display_get_height() {
  if (displayHeightGetter == displayHeight && displayHeight != -1)
    return displayHeightGetter;
  display_get_size(true, &displayHeightGetter);
  int result = displayHeightGetter;
  displayHeight = result;
  return result;
}

}  // namespace enigma_user
