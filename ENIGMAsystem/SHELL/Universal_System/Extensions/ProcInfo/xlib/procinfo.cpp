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

#include "../procinfo.h"
#include <X11/Xlib.h>

using std::to_string;

static inline int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
    return 0;
}

static inline int XIOErrorHandlerImpl(Display *display) {
    return 0;
}

static inline unsigned long get_wid_or_pid(Display *display, Window window, bool wid) {
  unsigned char *prop;
  unsigned long property;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  filter_atom = XInternAtom(display, wid ? "_NET_ACTIVE_WINDOW" : "_NET_WM_PID", True);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, False,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != NULL) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  return property;
}

namespace procinfo {

bool wid_exists(wid_t wid) {
  bool result = false;
  Display *display = XOpenDisplay(NULL);
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
  Window window = XDefaultRootWindow(display);
  unsigned char *prop;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  filter_atom = XInternAtom(display, "_NET_CLIENT_LIST", True);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1024, False, 
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != NULL && nitems) {
    if (actual_format == 32) {
      unsigned long *array = (unsigned long *)prop;
      for (unsigned i = 0; i < nitems; i++) {
        if (stoul(wid, nullptr, 10) == (unsigned long)array[i]) {
          result = true;
          break;
        }
      }
    }
    XFree(prop);
  }
  XCloseDisplay(display);
  return result;
}

window_t window_from_wid(wid_t wid) {
  return stoull(wid, nullptr, 10);
}

wid_t wid_from_window(window_t window) {
  return to_string(reinterpret_cast<unsigned long long>(window));
}

process_t pid_from_wid(wid_t wid) {
  process_t pid; Window window;
  window = stoul(wid, nullptr, 10);
  if (!window) return pid;
  Display *display = XOpenDisplay(NULL);
  pid = get_wid_or_pid(display, window, false);
  XCloseDisplay(display);
  return pid;
}

wid_t wid_from_top() {
  wid_t wid; Window window;
  Display *display = XOpenDisplay(NULL);
  int screen = XDefaultScreen(display);
  window = RootWindow(display, screen);
  wid = to_string(get_wid_or_pid(display, window, true));
  XCloseDisplay(display);
  return wid;
}

process_t pid_from_top() {
  return pid_from_wid(wid_from_top());
}

void wid_to_top(wid_t wid) {
  if (wid_exists(wid)) {
    unsigned long window = stoul(wid, nullptr, 10);
    Display *display = XOpenDisplay(NULL);
    XRaiseWindow(display, window);
    XSetInputFocus(display, window, RevertToPointerRoot, CurrentTime);
    XCloseDisplay(display);
  }
}

void wid_set_pwid(wid_t wid, wid_t pwid) {
  Display *display = XOpenDisplay(NULL);
  unsigned long window = stoul(wid, nullptr, 10);
  unsigned long parent = stoul(pwid, nullptr, 10);
  XSetTransientForHint(display, window, parent);
  XCloseDisplay(display);
}

} // namespace procinfo
