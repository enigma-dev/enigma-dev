/*

 MIT License
 
 Copyright © 2022 Samuel Venable
 
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>

#include <mutex>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <unordered_map>

#include "dialogs.h"
#include "cproc/cproc.hpp"

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

using std::string;
using std::vector;

namespace enigma {

CommandLineWidgetEngine *current_widget_engine = zenity_widgets;
 
static bool modifyInit = false;

static inline int XErrorHandlerImpl(Display *display, XErrorEvent *event) {
  return 0;
}

static inline int XIOErrorHandlerImpl(Display *display) {
  return 0;
}

static inline void SetErrorHandlers() {
  XSetErrorHandler(XErrorHandlerImpl);
  XSetIOErrorHandler(XIOErrorHandlerImpl);
}

static inline bool kwin_running() {
  SetErrorHandlers();
  Display *d = XOpenDisplay(nullptr);
  Atom aKWinRunning = XInternAtom(d, "KWIN_RUNNING", True);
  bool bKWinRunning = (aKWinRunning != None);
  XCloseDisplay(d);
  return bKWinRunning;
}

static int index = -1;
static std::unordered_map<int, XPROCID> child_proc_id;
static std::unordered_map<int, bool> proc_did_execute;
static std::unordered_map<CPROCID, std::intptr_t> stdipt_map;
static std::unordered_map<CPROCID, std::string> stdopt_map;
static std::unordered_map<CPROCID, bool> complete_map;
static std::mutex stdopt_mutex;

static inline const char *executed_process_read_from_standard_output(CPROCID proc_index) {
  if (stdopt_map.find(proc_index) == stdopt_map.end()) return "\0";
  std::lock_guard<std::mutex> guard(stdopt_mutex);
  return stdopt_map.find(proc_index)->second.c_str();
}

static inline void free_executed_process_standard_input(CPROCID proc_index) {
  if (stdipt_map.find(proc_index) == stdipt_map.end()) return;
  stdipt_map.erase(proc_index);
}

static inline void free_executed_process_standard_output(CPROCID proc_index) {
  if (stdopt_map.find(proc_index) == stdopt_map.end()) return;
  stdopt_map.erase(proc_index);
}

static inline bool completion_status_from_executed_process(CPROCID proc_index) {
  if (complete_map.find(proc_index) == complete_map.end()) return false;
  return complete_map.find(proc_index)->second;
}

static inline XPROCID process_execute_helper(const char *command, int *infp, int *outfp) {
  int p_stdin[2];
  int p_stdout[2];
  XPROCID pid = -1;
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

static inline void output_thread(std::intptr_t file, CPROCID proc_index) {
  ssize_t nRead = 0; char buffer[BUFSIZ];
  while ((nRead = read((int)file, buffer, BUFSIZ)) > 0) {
    buffer[nRead] = '\0';
    std::lock_guard<std::mutex> guard(stdopt_mutex);
    stdopt_map[proc_index].append(buffer, nRead);
  }
}

static inline XPROCID proc_id_from_fork_proc_id(XPROCID proc_id) {
  XPROCID *pid = nullptr; int pidsize = 0;
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  ngs::cproc::proc_id_from_parent_proc_id(proc_id, &pid, &pidsize);
  if (pid) { if (pidsize) { proc_id = pid[pidsize - 1]; }
  ngs::cproc::free_proc_id(pid); }
  return proc_id;
}

static inline CPROCID process_execute(const char *command) {
  index++;
  int infd = 0, outfd = 0;
  XPROCID proc_id = 0, fork_proc_id = 0, wait_proc_id = 0;
  fork_proc_id = process_execute_helper(command, &infd, &outfd);
  proc_id = fork_proc_id; wait_proc_id = proc_id;
  std::this_thread::sleep_for(std::chrono::milliseconds(5));
  if (fork_proc_id != -1) {
    while ((proc_id = proc_id_from_fork_proc_id(proc_id)) == wait_proc_id) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      int status; wait_proc_id = waitpid(fork_proc_id, &status, WNOHANG);
      char **cmd = nullptr; int cmdsize; ngs::cproc::cmdline_from_proc_id(fork_proc_id, &cmd, &cmdsize);
      if (cmd) { if (cmdsize && strcmp(cmd[0], "/bin/sh") == 0) {
      if (wait_proc_id > 0) proc_id = wait_proc_id; } ngs::cproc::free_cmdline(cmd); }
    }
  } else { proc_id = 0; }
  child_proc_id[index] = proc_id; std::this_thread::sleep_for(std::chrono::milliseconds(5));
  proc_did_execute[index] = true; CPROCID proc_index = (CPROCID)proc_id;
  stdipt_map.insert(std::make_pair(proc_index, (std::intptr_t)infd));
  std::thread opt_thread(output_thread, (std::intptr_t)outfd, proc_index);
  opt_thread.join();
  free_executed_process_standard_input(proc_index);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  complete_map[proc_index] = true;
  return proc_index;
}

static inline CPROCID process_execute_async(const char *command) {
  int prevIndex = index;
  std::thread proc_thread(process_execute, command);
  while (prevIndex == index) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  while (proc_did_execute.find(index) == proc_did_execute.end() || !proc_did_execute[index]) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  CPROCID proc_index = (CPROCID)child_proc_id[index];
  complete_map[proc_index] = false;
  proc_thread.detach();
  return proc_index;
}

// set dialog transient; set title caption.
static inline void modify_shell_dialog(XPROCID pid) {
  SetErrorHandlers(); int sz = 0;
  WINDOWID *arr = nullptr; Window wid = 0;
  Display *display = XOpenDisplay(nullptr);
  ngs::cproc::window_id_from_proc_id(pid, &arr, &sz);
  if (sz) {
    wid = (Window)ngs::cproc::native_window_from_window_id(arr[sz - 1]);
    if (!enigma_user::sprite_exists(enigma_user::window_get_icon_index())) {
      XSynchronize(display, true);
      Atom property = XInternAtom(display, "_NET_WM_ICON", false);
      XChangeProperty(display, wid, property, XA_CARDINAL, 32, 
      PropModeReplace, (unsigned char *)xwindow_icon_default, 2 + (64 * 64)); 
      XFlush(display);
    } else {
      XSetIconFromSprite(display, wid, enigma_user::window_get_icon_index(), enigma_user::window_get_icon_subimg());
    }
    XSetTransientForHint(display, wid, (Window)(std::intptr_t)enigma_user::window_handle());
    int len = enigma_user::widget_get_caption().length() + 1; char *buffer = new char[len]();
    strcpy(buffer, enigma_user::widget_get_caption().c_str()); XChangeProperty(display, wid,
    XInternAtom(display, "_NET_WM_NAME", false),
    XInternAtom(display, "UTF8_STRING", false),
    8, PropModeReplace, (unsigned char *)buffer, len);
    delete[] buffer; Window focus; int revert; while (!modifyInit) { 
    XSynchronize(display, true); XRaiseWindow(display, wid);
    XSetInputFocus(display, wid, RevertToParent, CurrentTime);
    XFlush(display); XGetInputFocus(display, &focus, &revert);
    if (wid == focus) modifyInit = true; }
    ngs::cproc::free_window_id(arr); 
  }
  XCloseDisplay(display);
}

bool widget_system_initialize() {
  setenv("WAYLAND_DISPLAY", "", 1); // force xwayland fallback prevents bug.
  // Defaults to the GUI toolkit (GTK+/Qt) that matches Desktop Environment.
  current_widget_engine = kwin_running() ? kdialog_widgets : zenity_widgets;
  return true;
}

string create_shell_dialog(string command) {
  string output; modifyInit = false;
  XPROCID pid = process_execute_async(command.c_str());
  if (pid) {
    while (!completion_status_from_executed_process(pid)) {
      modify_shell_dialog(pid); std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    output = executed_process_read_from_standard_output(pid);
    free_executed_process_standard_output(pid);
    while (!output.empty() && (output.back() == '\r' || output.back() == '\n')) {
      output.pop_back();
    }
  }
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

string widget_get_caption() {
  return enigma::current_widget_engine->widget_get_caption();
}

void widget_set_caption(string title) {
  enigma::current_widget_engine->widget_set_caption(title);
}

} // namespace enigma_user
