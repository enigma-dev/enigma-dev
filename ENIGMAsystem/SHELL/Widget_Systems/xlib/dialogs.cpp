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
#include <thread>
#include <chrono>
#include <vector>

#include "dialogs.h"

#include "../../../../CompilerSource/OS_Switchboard.h"

#include "Widget_Systems/widgets_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"

#include "Universal_System/estring.h"
#include "Platforms/xlib/XLIBicon.h"
#include "Universal_System/Resources/sprites.h"
#include "Platforms/General/PFwindow.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <pthread.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#if CURRENT_PLATFORM_ID == OS_MACOSX
#include <sys/proc_info.h>
#include <libproc.h>
#elif CURRENT_PLATFORM_ID == OS_LINUX
#include <proc/readproc.h>
#elif CURRENT_PLATFORM_ID == OS_FREEBSD
#include <sys/user.h>
#include <libutil.h>
#elif CURRENT_PLATFORM_ID == OS_DRAGONFLY
#include <sys/param.h>
#include <sys/sysctl.h>
#include <sys/user.h>
#include <libutil.h>
#include <kvm.h>
#endif

using std::string;
using std::vector;

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

static unsigned long GetActiveWidOrWindowPid(Display *display, Window window, bool wid) {
  SetErrorHandlers();
  unsigned char *prop;
  unsigned long property = 0;
  Atom actual_type, filter_atom;
  int actual_format, status;
  unsigned long nitems, bytes_after;
  filter_atom = XInternAtom(display, wid ? "_NET_ACTIVE_WINDOW" : "_NET_WM_PID", true);
  status = XGetWindowProperty(display, window, filter_atom, 0, 1000, false,
  AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
  if (status == Success && prop != nullptr) {
    property = prop[0] + (prop[1] << 8) + (prop[2] << 16) + (prop[3] << 24);
    XFree(prop);
  }
  return property;
}

static Window WidFromTop(Display *display) {
  SetErrorHandlers();
  int screen = XDefaultScreen(display);
  Window window = RootWindow(display, screen);
  return (Window)GetActiveWidOrWindowPid(display, window, true);
}

static pid_t PidFromWid(Display *display, Window window) {
  return (pid_t)GetActiveWidOrWindowPid(display, window, false);
}

// create dialog process
static pid_t ProcessCreate(const char *command, int *infp, int *outfp) {
  int p_stdin[2];
  int p_stdout[2];
  pid_t pid;
  if (pipe(p_stdin) == -1)
    return -1;
  if (pipe(p_stdout) == -1) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    return -1;
  }
  pid = fork();
  if (pid < 0) {
    close(p_stdin[0]);
    close(p_stdin[1]);
    close(p_stdout[0]);
    close(p_stdout[1]);
    return pid;
  } else if (pid == 0) {
    close(p_stdin[1]);
    dup2(p_stdin[0], 0);
    close(p_stdout[0]);
    dup2(p_stdout[1], 1);
    dup2(open("/dev/null", O_RDONLY), 2);
    for (int i = 3; i < 4096; i++)
      close(i);
    setsid();
    execl("/bin/sh", "/bin/sh", "-c", command, nullptr);
    exit(0);
  }
  close(p_stdin[0]);
  close(p_stdout[1]);
  if (infp == nullptr) {
    close(p_stdin[1]);
  } else {
    *infp = p_stdin[1];
  }
  if (outfp == nullptr) {
    close(p_stdout[0]);
  } else {
    *outfp = p_stdout[0];
  }
  return pid;
}

#if CURRENT_PLATFORM_ID == OS_MACOSX
static void PpidFromPid(pid_t procId, pid_t *parentProcId) {
  proc_bsdinfo proc_info;
  if (proc_pidinfo(procId, PROC_PIDTBSDINFO, 0, &proc_info, sizeof(proc_info)) > 0) {
    *parentProcId = proc_info.pbi_ppid;
  }
}
#endif

#if CURRENT_PLATFORM_ID == OS_DRAGONFLY
static kvm_t *kd = nullptr;
#endif

static std::vector<pid_t> PidFromPpid(pid_t parentProcId) {
  std::vector<pid_t> vec;
  #if CURRENT_PLATFORM_ID == OS_MACOSX
  int cntp = proc_listpids(PROC_ALL_PIDS, 0, nullptr, 0);
  std::vector<pid_t> proc_info(cntp);
  std::fill(proc_info.begin(), proc_info.end(), 0);
  proc_listpids(PROC_ALL_PIDS, 0, &proc_info[0], sizeof(pid_t) * cntp);
  for (int j = cntp; j > 0; j--) {
    if (proc_info[j] == 0) { continue; }
    pid_t ppid; PpidFromPid(proc_info[j], &ppid);
    if (ppid == parentProcId) {
      vec.push_back(proc_info[j]);
    }
  }
  #elif CURRENT_PLATFORM_ID == OS_LINUX
  PROCTAB *proc = openproc(PROC_FILLSTAT);
  while (proc_t *proc_info = readproc(proc, nullptr)) {
    if (proc_info->ppid == parentProcId) {
      vec.push_back(proc_info->tgid);
    }
    freeproc(proc_info);
  }
  closeproc(proc);
  #elif CURRENT_PLATFORM_ID == OS_FREEBSD
  int cntp; if (kinfo_proc *proc_info = kinfo_getallproc(&cntp)) {
    for (int j = 0; j < cntp; j++) {
      if (proc_info[j].ki_ppid == parentProcId) {
        vec.push_back(proc_info[j].ki_pid);
      }
    }
    free(proc_info);
  }
  #elif CURRENT_PLATFORM_ID == OS_DRAGONFLY
  char errbuf[_POSIX2_LINE_MAX];
  kinfo_proc *proc_info = nullptr; 
  const char *nlistf, *memf; nlistf = memf = "/dev/null";
  kd = kvm_openfiles(nlistf, memf, NULL, O_RDONLY, errbuf); if (!kd) return vec;
  int cntp = 0; if ((proc_info = kvm_getprocs(kd, KERN_PROC_ALL, 0, &cntp))) {
    for (int j = 0; j < cntp; j++) {
      if (proc_info[j].kp_pid >= 0 && proc_info[j].kp_ppid >= 0 && 
        proc_info[j].kp_ppid == parentProcId) {
        vec.push_back(proc_info[j].kp_pid);
      }
    }
    free(proc_info);
  }
  #endif
  return vec;
}

static pid_t PidFromPpidRecursive(pid_t parentProcId) {
  std::vector<pid_t> pidVec = PidFromPpid(parentProcId);
  if (pidVec.size()) {
    parentProcId = PidFromPpidRecursive(pidVec[0]);
  }
  return parentProcId;
}

// set dialog transient; set title caption.
static void *modify_shell_dialog(void *pid) {
  SetErrorHandlers();
  Display *display = XOpenDisplay(nullptr); Window wid;
  pid_t child = PidFromPpidRecursive((pid_t)(std::intptr_t)pid);
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    wid = WidFromTop(display);
    if (PidFromWid(display, wid) == child) {
      break;
    }
  }
  XSynchronize(display, true);
  Atom property = XInternAtom(display, "_NET_WM_ICON", false);
  XChangeProperty(display, wid, property, XA_CARDINAL, 32, PropModeReplace, (unsigned char *)nullptr, 0);
  XFlush(display);
  XSetTransientForHint(display, wid, (Window)(std::intptr_t)enigma_user::window_handle());
  int len = enigma_user::message_get_caption().length() + 1; char *buffer = new char[len]();
  strcpy(buffer, enigma_user::message_get_caption().c_str()); XChangeProperty(display, wid,
  XInternAtom(display, "_NET_WM_NAME", false),
  XInternAtom(display, "UTF8_STRING", false),
  8, PropModeReplace, (unsigned char *)buffer, len);
  delete[] buffer; XCloseDisplay(display);
  return nullptr;
}

bool widget_system_initialize() {
  setenv("WAYLAND_DISPLAY", "", 1); // force xwayland fallback prevents bug.
  // Defaults to the GUI toolkit (GTK+/Qt) that matches Desktop Environment.
  current_widget_engine = kwin_running() ? kdialog_widgets : zenity_widgets;
  return true;
}

string create_shell_dialog(string command) {
  string output; char buffer[BUFSIZ];
  int outfp = 0, infp = 0; ssize_t nRead = 0;
  pid_t pid = ProcessCreate(command.c_str(), &infp, &outfp);
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); pthread_t thread;
  pthread_create(&thread, nullptr, modify_shell_dialog, (void *)(std::intptr_t)pid);
  while ((nRead = read(outfp, buffer, BUFSIZ)) > 0) {
    buffer[nRead] = '\0';
    output.append(buffer, nRead);
  }
  pthread_cancel(thread);
  while (output.back() == '\r' || output.back() == '\n')
    output.pop_back();
  return output;
}

} // namespace enigma

void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  enigma::current_widget_engine->show_debug_message(errortext, type);
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
  return enigma::current_widget_engine->show_message(message);
}

int show_message_cancelable(string message) {
  return enigma::current_widget_engine->show_message_cancelable(message);
}

bool show_question(string message) {
  return enigma::current_widget_engine->show_question(message);
}

int show_question_cancelable(string message) {
  return enigma::current_widget_engine->show_question_cancelable(message);
}

int show_attempt(string errortext) {
  return enigma::current_widget_engine->show_attempt(errortext);
}

string get_string(string message, string def) {
  return enigma::current_widget_engine->get_string(message, def);
}

string get_password(string message, string def) {
  return enigma::current_widget_engine->get_password(message, def);
}

double get_integer(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  return enigma::current_widget_engine->get_integer(message, val);
}

double get_passcode(string message, var def) {
  double val = (strtod(def.c_str(), NULL)) ? : (double)def;
  return enigma::current_widget_engine->get_passcode(message, val);
}

string get_open_filename(string filter, string fname) {
  return enigma::current_widget_engine->get_open_filename(filter, fname);
}

string get_open_filenames(string filter, string fname) {
  return enigma::current_widget_engine->get_open_filenames(filter, fname);
}

string get_save_filename(string filter, string fname) {
  return enigma::current_widget_engine->get_save_filename(filter, fname);
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return enigma::current_widget_engine->get_open_filename_ext(filter, fname, dir, title);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return enigma::current_widget_engine->get_open_filenames_ext(filter, fname, dir, title);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return enigma::current_widget_engine->get_save_filename_ext(filter, fname, dir, title);
}

string get_directory(string dname) {
  return enigma::current_widget_engine->get_directory(dname);
}

string get_directory_alt(string capt, string root) {
  return enigma::current_widget_engine->get_directory_alt(capt, root);
}

int get_color(int defcol) {
  return enigma::current_widget_engine->get_color(defcol);
}

int get_color_ext(int defcol, string title) {
  return enigma::current_widget_engine->get_color_ext(defcol, title);
}

string message_get_caption() {
  return enigma::current_widget_engine->message_get_caption();
}

void message_set_caption(string title) {
  enigma::current_widget_engine->message_set_caption(title);
}

} // namespace enigma_user
