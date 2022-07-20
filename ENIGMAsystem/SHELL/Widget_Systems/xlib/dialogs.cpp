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

ifndef PROCESS_XQUARTZ_IMPL
#define PROCESS_XQUARTZ_IMPL
#endif

#if defined(__MACH__) && defined(__APPLE__)
ifndef PROCESS_GUIWINDOW_IMPL
#define PROCESS_GUIWINDOW_IMPL
#endif
#endif

#include "apiprocess/process.h"
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

// set dialog transient; set title caption.
static void modify_shell_dialog(ngs::proc::PROCID pid) {
  SetErrorHandlers(); int sz = 0;
  ngs::proc::WINDOWID *arr = nullptr;
  Display *display = XOpenDisplay(nullptr); Window wid = 0;
  ngs::proc::window_id_from_proc_id(pid, &arr, &sz);
  wid = (Window)ngs::proc::native_window_from_window_id(arr[sz - 1])
  ngs::proc::free_window_id(arr);
  if (!enigma_user::sprite_exists(enigma_user::window_get_icon_index())) {
    XSynchronize(display, true);
    unsigned long empty[] = { 1, 1, 0x0 };
    Atom property = XInternAtom(display, "_NET_WM_ICON", false);
    XChangeProperty(display, wid, property, XA_CARDINAL, 32, 
    PropModeReplace, (unsigned char *)empty, 3); XFlush(display);
  } else {
    XSetIconFromSprite(display, wid, enigma_user::window_get_icon_index(), enigma_user::window_get_icon_subimg());
  }
  XSetTransientForHint(display, wid, (Window)(std::intptr_t)enigma_user::window_handle());
  int len = enigma_user::widget_get_caption().length() + 1; char *buffer = new char[len]();
  strcpy(buffer, enigma_user::widget_get_caption().c_str()); XChangeProperty(display, wid,
  XInternAtom(display, "_NET_WM_NAME", false),
  XInternAtom(display, "UTF8_STRING", false),
  8, PropModeReplace, (unsigned char *)buffer, len);
  delete[] buffer; XCloseDisplay(display);
}

bool widget_system_initialize() {
  setenv("WAYLAND_DISPLAY", "", 1); // force xwayland fallback prevents bug.
  // Defaults to the GUI toolkit (GTK+/Qt) that matches Desktop Environment.
  current_widget_engine = kwin_running() ? kdialog_widgets : zenity_widgets;
  return true;
}

string create_shell_dialog(string command) {
  string output;
  ngs::proc::PROCID pid = ngs::proc::process_execute(command.c_str()); modify_shell_dialog(pid);
  output = ngs::proc::executed_process_read_from_standard_output(pid);
  ngs::proc::free_executed_process_standard_output(pid);
  while (!output.empty() && (output.back() == '\r' || output.back() == '\n'))
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

string widget_get_caption() {
  return enigma::current_widget_engine->widget_get_caption();
}

void widget_set_caption(string title) {
  enigma::current_widget_engine->widget_set_caption(title);
}

} // namespace enigma_user
