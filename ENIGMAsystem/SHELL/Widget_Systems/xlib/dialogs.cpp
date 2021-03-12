/** Copyright (C) 2019-2021 Samuel Venable
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

#include <string>
#include <chrono>
#include <thread>

#include "dialogs.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"

#include "Universal_System/estring.h"
#include "Platforms/PFwindow.h"

#include <sys/types.h>
#if defined (__APPLE__) && defined(__MACH__)
#include <sys/sysctl.h>
#include <libproc.h>
#elif defined(__linux__) && !defined(__ANDROID__)
#include <proc/readproc.h>
#elif defined(__FreeBSD__)
#include <sys/sysctl.h>
#include <sys/user.h>
#include <libutil.h>
#endif

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <signal.h>

using std::string;

namespace enigma {

CommandLineWidgetEngine *current_widget_engine = zenity_widgets;

static int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  return 0;
}

static int XIOErrorHandlerImpl(Display *display) {
  return 0;
}

static void SetErrorHandlers() {
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
}

static bool kwin_running() {
  SetErrorHandlers();
  Display *d = XOpenDisplay(nullptr);
  Atom aKWinRunning = XInternAtom(d, "KWIN_RUNNING", True);
  bool bKWinRunning = (aKWinRunning != None);
  XCloseDisplay(d);
  return bKWinRunning;
}

// get path from procId to get filename.
static string path_from_pid(pid_t pid) {
  if (kill(pid, 0) != 0) return "";
  #if defined (__APPLE__) && defined(__MACH__)
  char exe[PROC_PIDPATHINFO_MAXSIZE];
  if (proc_pidpath(pid, exe, sizeof(exe)) > 0) {
    return exe;
  }
  #elif defined(__linux__) && !defined(__ANDROID__)
  char exe[PATH_MAX];
  string symLink = string("/proc/") + to_string(pid) + string("/exe");
  if (realpath(symLink.c_str(), exe)) {
    return exe;
  }
  #elif defined(__FreeBSD__)
  int mib[4]; size_t s;
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = pid;
  if (sysctl(mib, 4, nullptr, &s, nullptr, 0) == 0) {
    string str; str.resize(s, '\0');
    char *exe = str.data();
    if (sysctl(mib, 4, exe, &s, nullptr, 0) == 0) {
      return exe;
    }
  }
  #endif
  return "";
}

// get fname to check if zenity/kdialog.
static string name_from_pid(pid_t pid) {
  return enigma_user::filename_name(path_from_pid(pid));
}

// get top wid / check fname.
static Window wid_from_top() {
  SetErrorHandlers();
  unsigned char *prop; 
  unsigned long property;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  Display *display = XOpenDisplay(nullptr);
  int screen = XDefaultScreen(display);
  Window window = RootWindow(display, screen);
  filter_atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", true);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, false,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != nullptr) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  XCloseDisplay(display);
  return (Window)window;
}

// check if wid from child pid.
pid_t pid_from_wid(Window window) {
  SetErrorHandlers();
  unsigned char *prop;
  unsigned long property;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  pid_t pid; if (!window) return pid;
  Display *display = XOpenDisplay(nullptr);
  filter_atom = XInternAtom(display, "_NET_WM_PID", true);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, false,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != nullptr) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  pid = property;
  XCloseDisplay(display);
  return (pid_t)pid;
}

// set window id to be the topmost.
static void wid_to_top(Window window) {
  SetErrorHandlers();
  Display *display = XOpenDisplay(nullptr);
  XRaiseWindow(display, window);
  XSetInputFocus(display, window, RevertToPointerRoot, CurrentTime);
  XCloseDisplay(display);
}

// set a window id to be transient for parent id.
static void wid_set_pwid(Window window, Window parent) {
  if (pwid == "-1") return;
  SetErrorHandlers();
  Display *display = XOpenDisplay(nullptr);
  XSetTransientForHint(display, window, parent);
  XCloseDisplay(display);
}

// wait for child process id of parent process id be procreated.
static bool WaitForChildPidOfPidToExist(pid_t pid, pid_t ppid) {
  if (pid == ppid) return true;
  while (pid != ppid) {
    if (pid <= 1) break;
    pid = ppid_from_pid(pid);
  }
  return (pid != ppid);
}

// set dialog transient and set caption.
static pid_t modify_dialog(pid_t ppid) {
  pid_t pid = 0;
  // fork gets child procId.
  if ((pid = fork()) == 0) {
    SetErrorHandlers();
    Display *display = XOpenDisplay(nullptr);
    
    // get winId from top winId.
    Window window = wid_from_top();
    pid_t pid = pid_from_wid(window);
 
    // wait for zenity/kdialog to create the window.
    while (WaitForChildPidOfPidToExist(pid, ppid) && 
      name_from_pid(pid) != "zenity" && name_from_pid(pid) != "kdialog") {
      window = wid_from_top();
      pid = pid_from_wid(window);
    }
    
    // force zneity/kdialog to stay on top of the game window.
    wid_set_pwid(window, (Window)enigma_user::window_handle());
    
    // remove "kdialog" from titlebar - only use titlebar text.
    Atom atom_name = XInternAtom(display,"_NET_WM_NAME", true);
    Atom atom_utf_type = XInternAtom(display,"UTF8_STRING", true);
    char *cstr_caption = (char *)enigma_user::message_get_caption().c_str();
    XChangeProperty(display, window, atom_name, atom_utf_type, 8, 
      PropModeReplace, (unsigned char *)cstr_caption, strlen(cstr_caption));
  
    XCloseDisplay(display);
    exit(0);
  }
  return pid;
}

// delete home folder :P jk
pid_t InitFork() {
  pid_t ppid = getpid();
  return modify_dialog(ppid);
}

// just in case no window
void KillFork(pid_t pid) {
  kill(pid, SIGTERM);
  bool died = false;
  for (unsigned i = 0; !died && i < 4; i++) {
    int status; 
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    if (waitpid(pid, &status, WNOHANG) == pid) died = true;
  }
  if (!died) kill(pid, SIGKILL);
}

bool widget_system_initialize() {
  setenv("WAYLAND_DISPLAY", "", 1); // force xwayland fallback prevents bug.
  // Defaults to the GUI toolkit (GTK+/Qt) that matches Desktop Environment.
  current_widget_engine = kwin_running() ? kdialog_widgets : zenity_widgets;
  return true;
}

} // namespace enigma

void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  pid_t pid = enigma::InitFork();
  enigma::current_widget_engine->show_debug_message(errortext, type);
  KillFork(pid);
}

namespace enigma_user {
    
string widget_get_system() {
  if (enigma::current_widget_engine == enigma::kdialog_widgets) return ws_x11_kdialog;
  return ws_x11_zenity;
}

void widget_set_system(string sys) {
  if (sys == ws_x11_kdialog) enigma::current_widget_engine = enigma::kdialog_widgets;
  else enigma::current_widget_engine = enigma::zenity_widgets;
}

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { 
  enigma::current_widget_engine->show_info(info, bgcolor, left, top, width, height, embedGameWindow, showBorder, allowResize, stayOnTop, pauseGame, caption); 
}

int show_message(const string &message) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->show_message(message);
  KillFork(pid);
  return result;
}

int show_message_cancelable(string message) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->show_message_cancelable(message);
  KillFork(pid);
  return result;
}

bool show_question(string message) {
  pid_t pid = enigma::InitFork();
  bool result = enigma::current_widget_engine->show_question(message);
  KillFork(pid);
  return result;
}

int show_question_cancelable(string message) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->show_question_cancelable(message);
  KillFork(pid);
  return result;
}

int show_attempt(string errortext) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->show_attempt(errortext);
  KillFork(pid);
  return result;
}

string get_string(string message, string def) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_string(message, def);
  KillFork(pid);
  return result;
}

string get_password(string message, string def) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_password(message, def);
  KillFork(pid);
  return result;
}

double get_integer(string message, var def) {
  pid_t pid = enigma::InitFork();
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  double result = enigma::current_widget_engine->get_integer(message, val);
  KillFork(pid);
  return result;
}

double get_passcode(string message, var def) {
  pid_t pid = enigma::InitFork();
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  double result = enigma::current_widget_engine->get_passcode(message, val);
  KillFork(pid);
  return result;
}

string get_open_filename(string filter, string fname) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_open_filename(filter, fname);
  KillFork(pid);
  return result;
}

string get_open_filenames(string filter, string fname) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_open_filenames(filter, fname);
  KillFork(pid);
  return result;
}

string get_save_filename(string filter, string fname) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_save_filename(filter, fname);
  KillFork(pid);
  return result;
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_open_filename_ext(filter, fname, dir, title);
  KillFork(pid);
  return result;
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_open_filenames_ext(filter, fname, dir, title);
  KillFork(pid);
  return result;
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_save_filename_ext(filter, fname, dir, title);
  KillFork(pid);
  return result;
}

string get_directory(string dname) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_directory(dname);
  KillFork(pid);
  return result;
}

string get_directory_alt(string capt, string root) {
  pid_t pid = enigma::InitFork();
  string result = enigma::current_widget_engine->get_directory_alt(capt, root);
  KillFork(pid);
  return result;
}

int get_color(int defcol) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->get_color(defcol);
  KillFork(pid);
  return result;
}

int get_color_ext(int defcol, string title) {
  pid_t pid = enigma::InitFork();
  int result = enigma::current_widget_engine->get_color_ext(defcol, title);
  KillFork(pid);
  return result;
}

string message_get_caption() {
  return enigma::current_widget_engine->message_get_caption();
}

void message_set_caption(string title) {
  enigma::current_widget_engine->message_set_caption(title);
}

} // namespace enigma_user
