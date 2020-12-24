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

#include <fstream>
#include <cstdio>
#include <string>
#include <cwchar>

#include "Platforms/General/PFexternals.h"
#include "Platforms/General/PFwindow.h"
#include "Platforms/General/PFmain.h"
#include "Universal_System/estring.h"

#include "libdlgmod32.h"
#include "libdlgmod64.h"

#define byte __windows_byte_workaround
#include <windows.h>
#undef byte

using std::string;
using std::wstring;

using enigma_user::environment_get_variable;
using enigma_user::window_identifier;
using enigma_user::external_call;
using enigma_user::external_define;
using enigma_user::external_free;
using enigma_user::dll_cdecl;
using enigma_user::dll_stdcall;
using enigma_user::ty_string;
using enigma_user::ty_real;

static string libdlgmod;

static inline bool IsX86Process(HANDLE process) {
  BOOL isWow = true;
  SYSTEM_INFO systemInfo = { 0 };
  GetNativeSystemInfo(&systemInfo);
  if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
    return isWow;
  IsWow64Process(process, &isWow);
  return isWow;
}

namespace enigma {

void widget_system_initialize() {
  if (IsX86Process(GetCurrentProcess())) {
    libdlgmod = environment_get_variable("TEMP") + "\\libdlgmod32.dll";
  } else {
    libdlgmod = environment_get_variable("TEMP") + "\\libdlgmod64.dll";
  }
  wstring wlibdlgmod = widen(libdlgmod);
  FILE *file = NULL;
  std::wofstream strm(wlibdlgmod.c_str());
  strm.close();
  if (_wfopen_s(&file, wlibdlgmod.c_str(), L"wb") == 0) {
    if (IsX86Process(GetCurrentProcess())) {
      fwrite((char *)libdlgmod32_dll, sizeof(char), sizeof(libdlgmod32_dll), file);
    } else {
      fwrite((char *)libdlgmod64_dll, sizeof(char), sizeof(libdlgmod64_dll), file);
    }
    fclose(file);
  }
  external_call(external_define(libdlgmod, "widget_set_owner", dll_cdecl, ty_real, 1, ty_string), window_identifier());
}

} // namespace enigma

namespace enigma_user {

string widget_get_system() {
  return external_call(external_define(libdlgmod, "widget_get_system", dll_cdecl, ty_string, 0));
}

void widget_set_system(string sys) {
  external_call(external_define(libdlgmod, "widget_set_system", dll_cdecl, ty_real, 1, ty_string), sys);
}

wid_t widget_get_owner() {
  return external_call(external_define(libdlgmod, "widget_get_owner", dll_cdecl, ty_string, 0));
}

void widget_set_owner(wid_t hwnd) {
  external_call(external_define(libdlgmod, "widget_set_owner", dll_cdecl, ty_real, 1, ty_string), hwnd);
}

string widget_get_icon() {
  return external_call(external_define(libdlgmod, "widget_get_icon", dll_cdecl, ty_string, 0));
}

void widget_set_icon(string icon) {
  external_call(external_define(libdlgmod, "widget_set_icon", dll_cdecl, ty_real, 1, ty_string), icon);
}

string widget_get_caption() {
  return external_call(external_define(libdlgmod, "widget_get_caption", dll_cdecl, ty_string, 0));
}

void widget_set_caption(wid_t str) {
  external_call(external_define(libdlgmod, "widget_set_caption", dll_cdecl, ty_real, 1, ty_string), str);
}

string widget_get_button_name(int type) {
  return external_call(external_define(libdlgmod, "widget_get_button_name", dll_cdecl, ty_string, 1, ty_real), type);
}

void widget_set_button_name(int type, string name) {
  external_call(external_define(libdlgmod, "widget_set_button_name", dll_cdecl, ty_real, 2, ty_real, ty_string), type, name);
}

int show_message(string str) {
  return external_call(external_define(libdlgmod, "show_message", dll_cdecl, ty_real, 1, ty_string), str);
}

int show_message_cancelable(string str) {
  return external_call(external_define(libdlgmod, "show_message_cancelable", dll_cdecl, ty_real, 1, ty_string), str);
}

int show_question(string str) {
  return external_call(external_define(libdlgmod, "show_question", dll_cdecl, ty_real, 1, ty_string), str);
}

int show_question_cancelable(string str) {
  return external_call(external_define(libdlgmod, "show_question_cancelable", dll_cdecl, ty_real, 1, ty_string), str);
}

int show_attempt(string str) {
  return external_call(external_define(libdlgmod, "show_attempt", dll_cdecl, ty_real, 1, ty_string), str);
}

int show_error(string str, bool abort) {
  return external_call(external_define(libdlgmod, "show_error", dll_cdecl, ty_real, 2, ty_string, ty_real), str, abort);
}

string get_string(string str, string def) {
  return external_call(external_define(libdlgmod, "get_string", dll_cdecl, ty_string, 2, ty_string, ty_string), str, def);
}

string get_password(string str, string def) {
  return external_call(external_define(libdlgmod, "get_password", dll_cdecl, ty_string, 2, ty_string, ty_string), str, def);
}

double get_integer(string str, double def) {
  return external_call(external_define(libdlgmod, "get_integer", dll_cdecl, ty_real, 2, ty_string, ty_real), str, def);
}

double get_passcode(string str, double def) {
  return external_call(external_define(libdlgmod, "get_passcode", dll_cdecl, ty_real, 2, ty_string, ty_real), str, def);
}

string get_open_filename(string filter, string fname) {
  return external_call(external_define(libdlgmod, "get_open_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
}

string get_open_filename_ext(string filter, string fname, string dir, string title) {
  return external_call(external_define(libdlgmod, "get_open_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
}

string get_open_filenames(string filter, string fname) {
  return external_call(external_define(libdlgmod, "get_open_filenames", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
}

string get_open_filenames_ext(string filter, string fname, string dir, string title) {
  return external_call(external_define(libdlgmod, "get_open_filenames_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
}

string get_save_filename(string filter, string fname) {
  return external_call(external_define(libdlgmod, "get_save_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname);
}

string get_save_filename_ext(string filter, string fname, string dir, string title) {
  return external_call(external_define(libdlgmod, "get_save_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, dir, title);
}

string get_directory(string dname) {
  return external_call(external_define(libdlgmod, "get_directory", dll_cdecl, ty_string, 1, ty_string), dname);
}

string get_directory_alt(string capt, string root) {
  return external_call(external_define(libdlgmod, "get_directory_alt", dll_cdecl, ty_string, 2, ty_string, ty_string), capt, root);
}

int get_color(int defcol) {
  return external_call(external_define(libdlgmod, "get_color", dll_cdecl, ty_real, 1, ty_real), defcol);
}

int get_color_ext(int defcol, string title) {
  return external_call(external_define(libdlgmod, "get_color_ext", dll_cdecl, ty_real, 2, ty_real, ty_string), defcol, title);
}

} // namespace enigma_user
