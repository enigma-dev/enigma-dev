#include "Platforms/xlib/XLIBwindow.h"
#include <X11/Xlib.h>

namespace enigma_user {

pid_t pid_from_wid(string wid) {
  unsigned long window = stoul(wid, nullptr, 10);
  if (window == 0) return 0; 
  unsigned char *prop;

  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;

  filter_atom = XInternAtom(display, "_NET_WM_PID", True);
  status = XGetWindowProperty(enigma::x11::disp, window, filter_atom, 0, 1000, False, 
    AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

  if (status == Success && prop != NULL) {
    unsigned long long_property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);

    return (pid_t)long_property;
  }
  
  return 0;
}

}
