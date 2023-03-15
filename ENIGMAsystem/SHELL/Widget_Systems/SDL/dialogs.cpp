/*

 MIT License

 Copyright © 2021-2022 Samuel Venable

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

#include <string>

#include <cstdlib>
#include <cstdio>

#include "Widget_Systems/SDL/dialogs.h"
#include "Platforms/General/PFexternals.h"
#include "Platforms/General/PFwindow.h"
#include "Universal_System/estring.h"
#include "Universal_System/fileio.h"
#include "Universal_System/directoryglobals.h"
#include "Platforms/SDL/Window.h"
#include "OpenGLHeaders.h"

#ifdef _WIN32
#define DLLEXT ".dll"
#elif (defined(__APPLE__) && defined(__MACH__))
#define DLLEXT ".dylib"
#else
#define DLLEXT ".so"
#endif

using std::string;

namespace enigma {

  bool widget_system_initialize() {
    return true;
  }

}

namespace enigma_user {

void show_info(string info, int bgcolor, int left, int top, int width, int height, bool embedGameWindow, bool showBorder, bool allowResize, bool stayOnTop, bool pauseGame, string caption) { }

void show_info() { }

void show_debug_message(string errortext, MESSAGE_TYPE type) {
  #ifndef DEBUG_MODE
  errortext += "\n";
  fputs(errortext.c_str(), stderr);
  fflush(stderr);
  #endif
  if (type == MESSAGE_TYPE::M_FATAL_ERROR || 
      type == MESSAGE_TYPE::M_FATAL_USER_ERROR) {
    string ok = environment_get_variable("IMGUI_YES");
    string caption = environment_get_variable("IMGUI_DIALOG_CAPTION");
    environment_set_variable("IMGUI_DIALOG_CAPTION", "Fatal Error");
    environment_set_variable("IMGUI_OK", "Abort");
    show_message(errortext);
    environment_set_variable("IMGUI_DIALOG_CAPTION", caption);
    environment_set_variable("IMGUI_OK", ok);
    abort();
  } else   if (type == MESSAGE_TYPE::M_ERROR || 
      type == MESSAGE_TYPE::M_USER_ERROR) {
    string yes = environment_get_variable("IMGUI_YES");
    string no = environment_get_variable("IMGUI_NO");
    string caption = environment_get_variable("IMGUI_DIALOG_CAPTION");
    environment_set_variable("IMGUI_DIALOG_CAPTION", "Error");
    environment_set_variable("IMGUI_YES", "Abort");
    environment_set_variable("IMGUI_NO", "Ignore");
    bool question = show_question(errortext);
    environment_set_variable("IMGUI_DIALOG_CAPTION", caption);
    environment_set_variable("IMGUI_YES", yes);
    environment_set_variable("IMGUI_NO", no);
    if (question) abort();
  }
}

int show_message(const string &message) { 
  external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "show_message", dll_cdecl, ty_string, 1, ty_string), message);
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return 1;
}

bool show_question(string message) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "show_question", dll_cdecl, ty_string, 1, ty_string), message).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? true : false);
}

int show_question_ext(string message) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "show_question_ext", dll_cdecl, ty_string, 1, ty_string), message).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  string no = (environment_get_variable("IMGUI_No").empty() ? "No" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? 1 : ((result == no) ? 0 : -1));
}
\
string get_open_filename(string filter, string fname) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filename_ext(string filter, string fname, string title, string dir) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames(string filter, string fname) {
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filenames", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames_ext(string filter, string fname, string title, string dir) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_open_filenames_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_save_filename(string filter, string fname) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_save_filename", dll_cdecl, ty_string, 2, ty_string, ty_string), filter, fname).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_save_filename_ext(string filter, string fname, string title, string dir) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_save_filename_ext", dll_cdecl, ty_string, 4, ty_string, ty_string, ty_string, ty_string), filter, fname, title, dir).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_directory(string dname) { 
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_directory", dll_cdecl, ty_string, 1, ty_string), dname).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_directory_alt(string capt, string root) {
  string result = external_call(external_define(filename_change_ext(program_pathname, DLLEXT), "get_directory_alt", dll_cdecl, ty_string, 2, ty_string, ty_string), capt, root).to_string();
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

} // namespace enigma_user
