namespace {

int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  return 0;
}

int XIOErrorHandlerImpl(Display *display) {
  return 0;
}

void SetErrorHandlers() {
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
}

unsigned long XGetActiveWindow() {
  SetErrorHandlers();
  unsigned char *prop;
  unsigned long property;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  int screen = XDefaultScreen(display);
  window = RootWindow(enigma::x11::disp, screen);
  filter_atom = XInternAtom(enigma::x11::disp, "_NET_ACTIVE_WINDOW", True);
  status = XGetWindowProperty(enigma::x11::disp, window, filter_atom, 0, 1000, False,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != NULL) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  return property;
}

} // anonymous namespace

namespace enigma_user {

bool window_has_focus() {
  return (enigma::x11::win == XGetActiveWindow());
}

} // namespace enigma_user

