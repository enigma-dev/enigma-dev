#include <string>
#include <ostream>
#include <cstdio>

#include "Platforms/General/PFwindow.h"
#include "Platforms/platforms_mandatory.h"
#include "Widget_Systems/General/WSdialogs.h"
#include "Universal_System/directoryglobals.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/General/PFexternals.h"
#include "strings_util.h"

// libdlgmod.dll source code available here free:
// https://github.com/time-killer-games/libdlgmod
// the two includes below are binary dumps of dll
// one is the 32-bit dll the other is the 64-bit.

#if !defined(_WIN64)
#include "libdlgmod32.h"
#else
#include "libdlgmod64.h"
#endif

using std::string;

namespace enigma {

bool widget_system_initialize() { 
  FILE *file = nullptr;
  std::wstring widen(enigma_user::temp_directory + "libdlgmod.dll");
  std::wofstream strm(wdll.c_str());
  strm.close();
  if (_wfopen_s(&file, wdll.c_str(), L"wb") == 0) {
    #if !defined(_WIN64)
    fwrite((char *)libdlgmod32_dll, sizeof(char), sizeof(libdlgmod32_dll), file);
    #else
    fwrite((char *)libdlgmod64_dll, sizeof(char), sizeof(libdlgmod64_dll), file);
    #endif
    fclose(file);
  }
  enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "widget_set_owner", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), enigma_user::window_identifier());
  return true;
}

} // namespace enigma

void show_debug_message_helper(string errortext, MESSAGE_TYPE type) {
  enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_error", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), errortext, (type == MESSAGE_TYPE::M_FATAL_ERROR || type == MESSAGE_TYPE::M_FATAL_USER_ERROR));
}

namespace enigma_user {

void show_info(string text, int bgcolor, int left, int top, int width, int height,
	bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop,
	bool pauseGame, string caption) {}

string widget_get_system() {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "widget_get_system", enigma_user::dll_cdecl, enigma_user::ty_string, 0));
}

void widget_set_system(string sys) {
  enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "widget_set_system", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), sys);
}

string message_get_caption() {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "widget_get_caption", enigma_user::dll_cdecl, enigma_user::ty_string, 0));
}

void message_set_caption(string title) {
  enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "widget_set_caption", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), title);
}

int show_message(const string &message) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_message", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), message);
}

int show_message_cancelable(string message) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_message_cancelable", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), message);
}

bool show_question(string message) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_question", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), message);
}

int show_question_cancelable(string message) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_question_cancelable", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), message);
}

int show_attempt(string errortext) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "show_attempt", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_string), errortext);
}

string get_string(string message, string def) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_string", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), message, def);
}

string get_password(string message, string def) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_password", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), message, def);
}

double get_integer(string message, var def) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_integer", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), message, def);
}

double get_passcode(string message, var def) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_passcode", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_string, enigma_user::ty_real), message, def);
}

string get_open_filename(string filter, string fname) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_open_filename", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), filter, fname);
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_open_filename_ext", enigma_user::dll_cdecl, enigma_user::ty_string, 4, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string), filter, fname, dir, title);
}

string get_open_filenames(string filter, string fname) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_open_filenames", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), filter, fname);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_open_filenames_ext", enigma_user::dll_cdecl, enigma_user::ty_string, 4, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string), filter, fname, dir, title);
}

string get_save_filename(string filter, string fname) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_save_filename", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), filter, fname);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_save_filename_ext", enigma_user::dll_cdecl, enigma_user::ty_string, 4, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string, enigma_user::ty_string), filter, fname, dir, title);
}

string get_directory(string dname) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_directory", enigma_user::dll_cdecl, enigma_user::ty_string, 1, enigma_user::ty_string), dname);
}

string get_directory_alt(string capt, string root) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_directory_alt", enigma_user::dll_cdecl, enigma_user::ty_string, 2, enigma_user::ty_string, enigma_user::ty_string), capt, root);
}

int get_color(int defcol) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_color", enigma_user::dll_cdecl, enigma_user::ty_real, 1, enigma_user::ty_real), defcol);
}

int get_color_ext(int defcol, string title) {
  return enigma_user::external_call(enigma_user::external_define(enigma_user::temp_directory + "libdlgmod.dll", "get_color_ext", enigma_user::dll_cdecl, enigma_user::ty_real, 2, enigma_user::ty_real, enigma_user::ty_string), defcol, title);
}

} // namespace enigma_user
