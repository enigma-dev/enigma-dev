/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2008-2011 IsmAvatar <cmagicj@nni.com>
*** Copyright (C) 2014 Seth N. Hetu
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

#include "Platforms/General/PFmain.h"       // For those damn vk_ constants, and io_clear().
#include "Platforms/platforms_mandatory.h"  // For type insurance
#include "Widget_Systems/widgets_mandatory.h"
#include "Universal_System/globalupdate.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/Resources/sprites.h"
#include "Universal_System/Resources/background.h"

#include "GameSettings.h"  // ABORT_ON_ALL_ERRORS (MOVEME: this shouldn't be needed here)
#include "XLIBmain.h"
#include "XLIBwindow.h"  // Type insurance for non-mandatory functions
#include "XLIBicon.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>   //printf, NULL
#include <stdlib.h>  //malloc
#include <time.h>    //clock
#include <unistd.h>  //usleep
#include <climits>
#include <map>
#include <string>  //Return strings without needing a GC

//////////
// INIT //
//////////

Cursor NoCursor, DefCursor;

using namespace enigma::x11;

namespace tmpSize {
    
int tmpW = enigma::windowWidth;
int tmpH = enigma::windowHeight;

} // namespace tmpSize

namespace enigma {

namespace x11 {
Display* disp;
Screen* screen;
Window win;
Atom wm_delwin;

void set_net_wm_pid(Window window) {
  pid_t pid = getpid();
  Atom cardinal = XInternAtom(disp, "CARDINAL", False);
  Atom net_wm_pid = XInternAtom(disp, "_NET_WM_PID", False);
  XChangeProperty(disp, window, net_wm_pid, cardinal, 32, PropModeReplace, (unsigned char*)&pid, sizeof(pid) / 4);
}

} // namespace x11;

bool initGameWindow()
{
  // Initiate display
  disp = XOpenDisplay(NULL);
  if (!disp) {
    DEBUG_MESSAGE("Display failed", MESSAGE_TYPE::M_FATAL_ERROR);
    return false;
  }

  // Identify components (close button, root pane)
  wm_delwin = XInternAtom(disp, "WM_DELETE_WINDOW", False);
  Window root = DefaultRootWindow(disp);

  // Defined in the appropriate graphics bridge.
  // Populates GLX attributes (or other graphics-system-specific properties).
  XVisualInfo* vi = enigma::CreateVisualInfo();

  // Window event listening and coloring
  XSetWindowAttributes swa;
  swa.border_pixel = 0;
  swa.background_pixel = (enigma::windowColor & 0xFF000000) | ((enigma::windowColor & 0xFF0000) >> 16) |
                         (enigma::windowColor & 0xFF00) | ((enigma::windowColor & 0xFF) << 16);
  swa.colormap = XCreateColormap(disp, root, vi->visual, AllocNone);
  swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |
                   FocusChangeMask | StructureNotifyMask;
  unsigned long valmask = CWColormap | CWEventMask | CWBackPixel;  // | CWBorderPixel;

  // Prepare window for display (center, caption, etc)
  screen = DefaultScreenOfDisplay(disp);
  int winw = enigma_user::room_width;
  int winh = enigma_user::room_height;
  // By default, if the room is too big, limit the size of the window to the
  // size of the screen. This is what 8.1 and Studio do; if the user wants to
  // manually override this they can do so using views/screen_set_viewport or
  // window_set_size/window_set_region_size.
  if (winw > screen->width) winw = screen->width;
  if (winh > screen->height) winh = screen->height;

  // Make the window
  win = XCreateWindow(disp, root, 0, 0, winw, winh, 0, vi->depth, InputOutput, vi->visual, valmask, &swa);
  set_net_wm_pid(win);
  XMoveWindow(disp, win, (screen->width - winw) / 2, (screen->height - winh) / 2);

  //register CloseButton listener
  Atom prots[] = {wm_delwin};
  if (!XSetWMProtocols(disp, win, prots, 1)) {
    DEBUG_MESSAGE("NoClose", MESSAGE_TYPE::M_ERROR);
    return false;
  }

  //Default cursor
  DefCursor = XCreateFontCursor(disp, 68);
  //Blank cursor
  XColor dummy;
  Pixmap blank = XCreateBitmapFromData(disp, win, "", 1, 1);
  if (blank == None) {  //out of memory
    DEBUG_MESSAGE("Failed to create no cursor. lulz", MESSAGE_TYPE::M_ERROR);
    NoCursor = DefCursor;
  } else {
    NoCursor = XCreatePixmapCursor(disp, blank, blank, &dummy, &dummy, 0, 0);
    XFreePixmap(disp, blank);
  }

  return true;
}

void destroyWindow() {
  XCloseDisplay(enigma::x11::disp);
}

}  //namespace enigma

namespace enigma_user {

window_t window_handle() {
  unsigned long long window_uint64 = static_cast<unsigned long>(enigma::x11::win);
  return static_cast<window_t>(window_uint64);
}

// returns an identifier for the XLIB window
// this string can be used in shell scripts
string window_identifier() {
  return std::to_string(*(static_cast<unsigned long long *>(window_handle())));
}

// returns an identifier for certain window
// this string can be used in shell scripts
string window_get_identifier(window_t hwnd) {
  return std::to_string(*(static_cast<unsigned long long *>(hwnd)));
}

void window_set_visible(bool visible) {
  if (visible) {
    XMapRaised(disp, win);
  } else {
    XUnmapWindow(disp, win);
  }
}

static int currentIconIndex = -1;
static unsigned currentIconFrame;

int window_get_icon_index() {
  return currentIconIndex;
}

unsigned window_get_icon_subimg() {
  return currentIconFrame;
}

void window_set_icon(int ind, unsigned subimg) {
  // the line below prevents glitchy minimizing when 
  // icons are changed rapidly (i.e. for animation).
  if (window_get_minimized()) return;

  // needs to be visible first to prevent segfault
  if (!window_get_visible()) window_set_visible(true);
  enigma::XSetIconFromSprite(disp, win, ind, subimg);

  currentIconIndex = ind;
  currentIconFrame = subimg;
}

int window_get_visible() {
  XWindowAttributes wa;
  XGetWindowAttributes(disp, win, &wa);
  return wa.map_state != IsUnmapped;
}

void window_set_caption(const string &caption) { XStoreName(disp, win, caption.c_str()); }

string window_get_caption() {
  char* caption;
  XFetchName(disp, win, &caption);
  if (!caption) return string();
  string res = caption;
  XFree(caption);
  return res;
}

}  // namespace enigma_user

inline int getMouse(int i) {
  Window r1, r2;
  int rx, ry, wx, wy;
  unsigned int mask;
  XQueryPointer(disp, win, &r1, &r2, &rx, &ry, &wx, &wy, &mask);
  switch (i) {
    case 0:
      return rx;
    case 1:
      return ry;
    case 2:
      return wx;
    case 3:
      return wy;
    default:
      return -1;
  }
}

enum { _NET_WM_STATE_REMOVE, _NET_WM_STATE_ADD, _NET_WM_STATE_TOGGLE };

typedef struct {
  unsigned long flags;
  unsigned long functions;
  unsigned long decorations;
  long inputMode;
  unsigned long status;
} Hints;

template <int count>
static bool windowHasAtom(const Atom (&atom)[count]) {
  bool res = false;
  Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);

  //Return types, not really used.
  Atom actualType;
  int actualFormat;
  unsigned long bytesAfterReturn;

  //These are used.
  unsigned long numItems;
  unsigned char* data = NULL;

  if (XGetWindowProperty(disp, win, wmState, 0, LONG_MAX, False, AnyPropertyType, &actualType, &actualFormat, &numItems,
                         &bytesAfterReturn, &data) == Success) {
    for (unsigned long i = 0; i < numItems; ++i) {
      for (int j = 0; j < count; ++j) {
        if (atom[j] == reinterpret_cast<unsigned long*>(data)[i]) {
          res = true;
          break;
        }
      }
    }
  }

  // Reclaim memory.
  if (data) {
    XFree(data);
  }
  return res;
}

namespace enigma_user {

int display_mouse_get_x() { return getMouse(0); }
int display_mouse_get_y() { return getMouse(1); }
int window_mouse_get_x() { return getMouse(2); }
int window_mouse_get_y() { return getMouse(3); }

void window_set_stayontop(bool stay) {
  Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
  Atom aStay = XInternAtom(disp, "_NET_WM_STATE_ABOVE", False);
  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event = True;
  xev.xclient.window = win;
  xev.xclient.message_type = wmState;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = (stay ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aStay;
  xev.xclient.data.l[2] = 0;
  XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

bool window_get_stayontop() {
  Atom a[] = {XInternAtom(disp, "_NET_WM_STATE_ABOVE", False)};
  return windowHasAtom(a);
}

void window_set_sizeable(bool sizeable) {
  if (window_get_maximized()) return;
  if (window_get_fullscreen()) return;
  enigma::isSizeable = sizeable;
  XSizeHints *sh = XAllocSizeHints();
  sh->flags = PMinSize | PMaxSize;
  
  if (enigma::window_min_width == -1)
    enigma::window_min_width = 1;
  if (enigma::window_max_width == -1) 
    enigma::window_max_width = INT_MAX;
  if (enigma::window_min_height == -1) 
    enigma::window_min_height = 1;
  if (enigma::window_max_height == -1) 
    enigma::window_max_height = INT_MAX;
  
  if (sizeable) {
    sh->min_width = enigma::window_min_width;
    sh->max_width = enigma::window_max_width;
    sh->min_height = enigma::window_min_height;
    sh->max_height = enigma::window_max_height;
  } else {
    sh->min_width = sh->max_width = window_get_width();
    sh->min_height = sh->max_height = window_get_height();
  }
  XSetWMNormalHints(disp, win, sh);
  XFree(sh);

  XResizeWindow(disp, win, enigma::windowWidth, enigma::windowHeight);
}

void window_set_min_width(int width) {
  enigma::window_min_width = width;
  window_set_sizeable(enigma::isSizeable);
}

void window_set_min_height(int height) {
  enigma::window_min_height = height;
  window_set_sizeable(enigma::isSizeable);
}

void window_set_max_width(int width) {
  enigma::window_max_width = width;
  window_set_sizeable(enigma::isSizeable);
}

void window_set_max_height(int height) {
  enigma::window_max_height = height;
  window_set_sizeable(enigma::isSizeable);
}

bool window_get_sizeable() { return enigma::isSizeable; }

void window_set_showborder(bool show) {
  if (window_get_maximized()) return;
  if (window_get_fullscreen()) return;
  if (window_get_showborder() == show) return;
  enigma::showBorder = show;
  Atom aKWinRunning = XInternAtom(disp, "KWIN_RUNNING", True);
  bool bKWinRunning = (aKWinRunning != None);
  XWindowAttributes wa;
  Window root, parent, *child; uint children;
  XWindowAttributes pwa;
  for (;;) {
    XGetWindowAttributes(disp, win, &wa);
    XQueryTree(disp, win, &root, &parent, &child, &children);
    XGetWindowAttributes(disp, parent, &pwa);
    // allow time for the titlebar and border sizes to be measured for proper window positioning...
    if ((bKWinRunning ? pwa.x : wa.x) || (bKWinRunning ? pwa.y : wa.y) || !window_get_showborder())
      break;
  }
  static const int xoffset = bKWinRunning ? pwa.x : wa.x;
  static const int yoffset = bKWinRunning ? pwa.y : wa.y;
  Hints hints;
  Atom property = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
  hints.flags = 2;
  hints.decorations = show;
  XChangeProperty(disp, win, property, property, 32, PropModeReplace, (unsigned char *)&hints, 5);
  int xpos = show ? enigma::windowX - xoffset : enigma::windowX;
  int ypos = show ? enigma::windowY - yoffset : enigma::windowY;
  XResizeWindow(disp, win, enigma::windowWidth + 1, enigma::windowHeight + 1); // trigger ConfigureNotify event
  XResizeWindow(disp, win, enigma::windowWidth - 1, enigma::windowHeight - 1); // set window back to how it was
  XMoveResizeWindow(disp, win, xpos, ypos, bKWinRunning ? wa.width : wa.width + xoffset, bKWinRunning ? wa.height : wa.height + yoffset);
}

bool window_get_showborder() {
  Atom type;
  int format;
  unsigned long bytes;
  unsigned long items;
  unsigned char *data = NULL;
  bool ret = true;
  Atom property = XInternAtom(disp, "_MOTIF_WM_HINTS", False);
  if (XGetWindowProperty(disp, win, property, 0, LONG_MAX, False, AnyPropertyType, &type, &format, &items, &bytes, &data) == Success && data != NULL) {
    Hints *hints = (Hints *)data;
    ret = hints->decorations;
    XFree(data);
  }
  return ret;
}

void window_set_showicons(bool show) {
  if (enigma::showIcons == show) return;
  enigma::showIcons = show;

  Atom wmState = XInternAtom(disp, "_NET_WM_WINDOW_TYPE", False);
  Atom aShow = XInternAtom(disp, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);
  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event = True;
  xev.xclient.window = win;
  xev.xclient.message_type = wmState;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = (show ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aShow;
  xev.xclient.data.l[2] = 0;
  XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}

bool window_get_showicons() { return enigma::showIcons; }

void window_set_minimized(bool minimized) {
  XClientMessageEvent ev;
  Atom prop;

  prop = XInternAtom(disp, "WM_CHANGE_STATE", False);
  if (prop == None) return;

  // TODO: When restored after a minimize the window may not have focus.
  ev.type = ClientMessage;
  ev.window = win;
  ev.message_type = prop;
  ev.format = 32;
  ev.data.l[0] = minimized ? IconicState : NormalState;
  XSendEvent(disp, RootWindow(disp, 0), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
}

bool window_get_minimized() {
  Atom a[] = {XInternAtom(disp, "_NET_WM_STATE_HIDDEN", False)};
  return windowHasAtom(a);
}

void window_set_maximized(bool maximized) {
  XClientMessageEvent ev;

  if (maximized) {
    Atom wm_state, max_horz, max_vert;
    wm_state = XInternAtom(disp, "_NET_WM_STATE", False);
    if (wm_state == None) return;

    max_horz = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
    max_vert = XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_VERT", False);

    // TODO: When restored after a minimize the window may not have focus.
    ev.type = ClientMessage;
    ev.window = win;
    ev.message_type = wm_state;
    ev.format = 32;
    ev.data.l[0] = _NET_WM_STATE_ADD;
    ev.data.l[1] = max_horz;
    ev.data.l[2] = max_vert;
    XSendEvent(disp, RootWindow(disp, 0), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
  } else {
    Atom prop;
    prop = XInternAtom(disp, "WM_CHANGE_STATE", False);
    if (prop == None) return;

    // TODO: When restored after a minimize the window may not have focus.
    ev.type = ClientMessage;
    ev.window = win;
    ev.message_type = prop;
    ev.format = 32;
    ev.data.l[0] = NormalState;
    XSendEvent(disp, RootWindow(disp, 0), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent*)&ev);
  }
}

bool window_get_maximized() {
  Atom maximized[] = {XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_VERT", False),
                      XInternAtom(disp, "_NET_WM_STATE_MAXIMIZED_HORZ", False)};
  return windowHasAtom(maximized);
}

void window_mouse_set(int x, int y) { XWarpPointer(disp, None, win, 0, 0, 0, 0, (int)x, (int)y); }

void display_mouse_set(int x, int y) { XWarpPointer(disp, None, DefaultRootWindow(disp), 0, 0, 0, 0, (int)x, (int)y); }

////////////
// WINDOW //
////////////

//Getters
int window_get_x() { return enigma::windowX; }
int window_get_y() { return enigma::windowY; }
int window_get_width() { return enigma::windowWidth; }
int window_get_height() { return enigma::windowHeight; }

//Setters
void window_set_position(int x, int y) {
  if (window_get_fullscreen()) return;
  enigma::windowX = x;
  enigma::windowY = y;
  XWindowAttributes wa;
  XGetWindowAttributes(disp, win, &wa);
  XMoveWindow(disp, win, (int)x - wa.x, (int)y - wa.y);
}

void window_set_size(unsigned int w, unsigned int h) {
  if (window_get_fullscreen()) return;
  enigma::windowWidth = w;
  enigma::windowHeight = h;
  enigma::compute_window_size();
}

void window_set_rectangle(int x, int y, int w, int h) {
  if (window_get_fullscreen()) return;
  enigma::windowX = x;
  enigma::windowY = y;
  enigma::windowWidth = w;
  enigma::windowHeight = h;
  XMoveResizeWindow(disp, win, x, y, w, h);
}

////////////////
// FULLSCREEN //
////////////////

void window_set_fullscreen(bool full) {
  if (enigma::isFullScreen == full && !full) return;
  enigma::isFullScreen = full;
  if (full) {
    tmpSize::tmpW = enigma::windowWidth;
    tmpSize::tmpH = enigma::windowHeight;
  }
  Atom wmState = XInternAtom(disp, "_NET_WM_STATE", False);
  Atom aFullScreen = XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False);
  XEvent xev;
  xev.xclient.type = ClientMessage;
  xev.xclient.serial = 0;
  xev.xclient.send_event = True;
  xev.xclient.window = win;
  xev.xclient.message_type = wmState;
  xev.xclient.format = 32;
  xev.xclient.data.l[0] = (full ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE);
  xev.xclient.data.l[1] = aFullScreen;
  xev.xclient.data.l[2] = 0;
  XSendEvent(disp, DefaultRootWindow(disp), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
  if (!full) XResizeWindow(disp, win, tmpSize::tmpW, tmpSize::tmpH);
}

bool window_get_fullscreen() {
  Atom fullscreen[] = {XInternAtom(disp, "_NET_WM_STATE_FULLSCREEN", False)};
  return windowHasAtom(fullscreen);
}

}  // namespace enigma_user

//default    +   -5   I    \    |    /    -    ^   ...  drg  no  -    |  drg3 ...  X  ...  ?   url  +
short curs[] = {68, 68, 68, 130, 52, 152, 135, 116, 136, 108, 114, 150, 90, 68, 108, 116, 90, 150, 0, 150, 92, 60, 52};

namespace enigma {
std::map<int, int> keybdmap;

inline unsigned short highbyte_allornothing(short x) { return x & 0xFF00 ? x | 0xFF00 : x; }

unsigned char keymap[512];
unsigned short keyrmap[256];
void initkeymap() {
  using namespace enigma_user;

  for (size_t i = 0; i < 512; ++i) keymap[i] = 0;
  for (size_t i = 0; i < 256; ++i) keyrmap[i] = 0;

  // Pretend this part doesn't exist
  keymap[0x151] = vk_left;
  keymap[0x153] = vk_right;
  keymap[0x152] = vk_up;
  keymap[0x154] = vk_down;
  keymap[0x1E3] = vk_control;
  keymap[0x1E4] = vk_control;
  keymap[0x1E9] = vk_alt;
  keymap[0x1EA] = vk_alt;
  keymap[0x1E1] = vk_shift;
  keymap[0x1E2] = vk_shift;
  keymap[0x10D] = vk_enter;
  keymap[0x185] = vk_lsuper;
  keymap[0x186] = vk_rsuper;
  keymap[0x117] = vk_tab;
  keymap[0x142] = vk_caps;
  keymap[0x14E] = vk_scroll;
  keymap[0x17F] = vk_pause;
  keymap[0x19E] = vk_numpad0;
  keymap[0x19C] = vk_numpad1;
  keymap[0x199] = vk_numpad2;
  keymap[0x19B] = vk_numpad3;
  keymap[0x196] = vk_numpad4;
  keymap[0x19D] = vk_numpad5;
  keymap[0x198] = vk_numpad6;
  keymap[0x195] = vk_numpad7;
  keymap[0x197] = vk_numpad8;
  keymap[0x19A] = vk_numpad9;
  keymap[0x1AF] = vk_divide;
  keymap[0x1AA] = vk_multiply;
  keymap[0x1AD] = vk_subtract;
  keymap[0x1AB] = vk_add;
  keymap[0x19F] = vk_decimal;
  keymap[0x1BE] = vk_f1;
  keymap[0x1BF] = vk_f2;
  keymap[0x1C0] = vk_f3;
  keymap[0x1C1] = vk_f4;
  keymap[0x1C2] = vk_f5;
  keymap[0x1C3] = vk_f6;
  keymap[0x1C4] = vk_f7;
  keymap[0x1C5] = vk_f8;
  keymap[0x1C6] = vk_f9;
  keymap[0x1C7] = vk_f10;
  keymap[0x1C8] = vk_f11;
  keymap[0x1C9] = vk_f12;
  keymap[0x108] = vk_backspace;
  keymap[0x11B] = vk_escape;
  keymap[0x150] = vk_home;
  keymap[0x157] = vk_end;
  keymap[0x155] = vk_pageup;
  keymap[0x156] = vk_pagedown;
  keymap[0x1FF] = vk_delete;
  keymap[0x163] = vk_insert;

  // Set up identity map...
  //for (int i = 0; i < 255; i++)
  //  usermap[i] = i;

  for (int i = 0; i < 'a'; i++) keymap[i] = i;
  for (int i = 'a'; i <= 'z'; i++)  // 'a' to 'z' wrap to 'A' to 'Z'
    keymap[i] = i + 'A' - 'a';
  for (int i = 'z' + 1; i < 255; i++) keymap[i] = i;

  for (size_t i = 0; i < 512; ++i) keyrmap[keymap[i]] = highbyte_allornothing(i);
}
}  // namespace enigma

namespace enigma_user {

void io_handle() {
  enigma::input_push();
  while (XQLength(disp)) {
    DEBUG_MESSAGE("processing an event...", MESSAGE_TYPE::M_INFO);
    if (enigma::handleEvents() > 0) exit(0);
  }
  enigma::update_mouse_variables();
}

int window_set_cursor(int c) {
  enigma::cursorInt = c;
  XUndefineCursor(disp, win);
  XDefineCursor(disp, win, (c == -1) ? NoCursor : XCreateFontCursor(disp, curs[-c]));
  return 0;
}

int window_get_cursor() { return enigma::cursorInt; }

bool keyboard_check_direct(int key) {
  char keyState[32];

  if (XQueryKeymap(enigma::x11::disp, keyState)) {
    //for (int x = 0; x < 32; x++)
    //keyState[x] = 0;
  } else {
    //TODO: print error message.
    return 0;
  }

  if (key == vk_anykey) {
    // next go through each member of keyState array
    for (unsigned i = 0; i < 32; i++) {
      const char& currentChar = keyState[i];

      // iterate over each bit and check if the bit is set
      for (unsigned j = 0; j < 8; j++) {
        // AND current char with current bit we're interested in
        bool isKeyPressed = ((1 << j) & currentChar) != 0;

        if (isKeyPressed) {
          return 1;
        }
      }
    }
    return 0;
  }
  if (key == vk_nokey) {
    // next go through each member of keyState array
    for (unsigned i = 0; i < 32; i++) {
      const char& currentChar = keyState[i];

      // iterate over each bit and check if the bit is set
      for (unsigned j = 0; j < 8; j++) {
        // AND current char with current bit we're interested in
        bool isKeyPressed = ((1 << j) & currentChar) != 0;

        if (isKeyPressed) {
          return 0;
        }
      }
    }
    return 1;
  }

  key = XKeysymToKeycode(enigma::x11::disp, enigma::keyrmap[key]);
  return (keyState[key >> 3] & (1 << (key & 7)));
}

void clipboard_set_text(string text) {
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  XChangeProperty(disp, RootWindow(disp, 0), XA_CLIPBOARD, XA_UTF8, 8, PropModeReplace,
                  reinterpret_cast<unsigned char*>(const_cast<char*>(text.c_str())), text.length() + 1);
}

string clipboard_get_text() {
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  //Atom XA_UNICODE = XInternAtom(disp, "UNICODE", 0);
  Atom actual_type;
  int actual_format;
  unsigned long nitems, leftover;
  unsigned char* buf;

  if (XGetWindowProperty(disp, RootWindow(disp, 0), XA_CLIPBOARD, 0, 10000000L, False, XA_UTF8, &actual_type,
                         &actual_format, &nitems, &leftover, &buf) == Success) {
    ;
    if (buf != NULL) {
      //free(buf);
      return string(reinterpret_cast<char*>(buf));
    } else {
      return "";
    }
  } else {
    return "";
  }
}

bool clipboard_has_text() {
  Atom XA_UTF8 = XInternAtom(disp, "UTF8", 0);
  Atom XA_CLIPBOARD = XInternAtom(disp, "CLIPBOARD", False);
  //Atom XA_UNICODE = XInternAtom(disp, "UNICODE", 0);
  Atom actual_type;
  int actual_format;
  unsigned long nitems, leftover;
  unsigned char* buf;

  if (XGetWindowProperty(disp, RootWindow(disp, 0), XA_CLIPBOARD, 0, 10000000L, False, XA_UTF8, &actual_type,
                         &actual_format, &nitems, &leftover, &buf) == Success) {
    ;
    return buf != NULL;
  } else {
    return false;
  }
}

}  // namespace enigma_user
