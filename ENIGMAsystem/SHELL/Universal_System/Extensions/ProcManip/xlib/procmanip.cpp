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

#include "../procmanip.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <cstring>

namespace proc_manip {

pid_t pid_from_wid(string wid) {
  unsigned long window = stoul(wid, nullptr, 10);
  if (window == 0) return 0; 
  unsigned char *prop;

  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;

  Display *display = XOpenDisplay(NULL);
  filter_atom = XInternAtom(display, "_NET_WM_PID", True);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, False, 
    AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

  if (status == Success && prop != NULL) {
    unsigned long long_property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);

    XCloseDisplay(display);
    return reinterpret_cast<pid_t>(long_property);
  }
  
  XCloseDisplay(display);
  return 0;
}

} // namespace proc_manip
