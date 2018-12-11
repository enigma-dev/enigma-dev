/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>
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

#include "XLIBmain.h"
#include "LINUXjoystick.h"
#include "XLIBwindow.h"

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFsystem.h"
#include "Platforms/platforms_mandatory.h"

#include "Universal_System/roomsystem.h"
#include "Universal_System/var4.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <sys/types.h>  //getpid
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

using namespace enigma::x11;

namespace enigma_user {
  const int os_type = os_linux;

  // Set the working_directory
  char buffer[PATH_MAX + 1];
  if (getcwd(buffer, PATH_MAX + 1) != NULL)
    working_directory = buffer;
  else
    working_directory = "";

  // Set the program_directory
  buffer[0] = 0;
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX + 1);
  if (count !=  -1)
    program_directory = dirname(buffer) + string("/");
  else
    program_directory = "";

  // Set the temp_directory
  char const *env = getenv("TMPDIR");

  if (env == 0)
    env = "/tmp/";

  temp_directory = env;

  double set_working_directory(string dname) {
    if (!dname.empty()) {
      while (*dname.rbegin() == '/') {
        dname.erase(dname.size() - 1);
      }
    }

    struct stat sb;
    if (stat((char *)dname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
      return (chdir((char *)dname.c_str()) == 0);

    return 0;
  }
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
              enigma_user::keyboard_string =
                  enigma_user::keyboard_string.substr(0, enigma_user::keyboard_string.length() - 1);
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
        enigma::windowWidth = e.xconfigure.width;
        enigma::windowHeight = e.xconfigure.height;

        if (WindowResizedCallback != NULL) {
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
        printf("Unhandled xlib event: %d\n", e.type);
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

namespace enigma_user {

int display_get_width() { return XWidthOfScreen(enigma::x11::screen); }
int display_get_height() { return XHeightOfScreen(enigma::x11::screen); }

}  // namespace enigma_user
