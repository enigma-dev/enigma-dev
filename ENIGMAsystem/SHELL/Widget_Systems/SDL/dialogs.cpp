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

#include "Widget_Systems/SDL/filedialogs.hpp"
#include "Widget_Systems/SDL/dialogs.h"
#include "Widget_Systems/widgets_mandatory.h"
#include "Platforms/General/PFwindow.h"
#include "Universal_System/directoryglobals.h"
#include "Universal_System/estring.h"
#include "Universal_System/fileio.h"
#include "Platforms/SDL/Window.h"
#include "OpenGLHeaders.h"

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
  ::show_message(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return 1;
}

bool show_question(string message) {
  string result = ::show_question(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? true : false);
}

int show_question_ext(string message) {
  string result = ::show_question_ext(message.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  string yes = (environment_get_variable("IMGUI_YES").empty() ? "Yes" : environment_get_variable("IMGUI_YES"));
  string no = (environment_get_variable("IMGUI_No").empty() ? "No" : environment_get_variable("IMGUI_YES"));
  return ((result == yes) ? 1 : ((result == no) ? 0 : -1));
}

std::string get_string(std::string message, std::string defstr) {
  std::string result = ::get_string(message.c_str(), defstr.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

double get_number(std::string message, double defnum) {
  double result = ::get_number(message.c_str(), defnum);
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

double get_integer(std::string message, double defint) {
  return get_number(message, defint);
}

string get_open_filename(string filter, string fname) {
  string result = ::get_open_filename(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filename_ext(string filter, string fname, string title, string dir) {
  string result = ::get_open_filename_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames(string filter, string fname) {
  string result = ::get_open_filenames(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_open_filenames_ext(string filter, string fname, string title, string dir) {
  string result = ::get_open_filenames_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_save_filename(string filter, string fname) { 
  string result = ::get_save_filename(filter.c_str(), fname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_save_filename_ext(string filter, string fname, string title, string dir) {
  string result = ::get_save_filename_ext(filter.c_str(), fname.c_str(), title.c_str(), dir.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_directory(string dname) {
  string result = ::get_directory(dname.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

string get_directory_alt(string capt, string root) {
  string result = ::get_directory_alt(capt.c_str(), root.c_str());
  SDL_GL_MakeCurrent(enigma::windowHandle, enigma::sdl_gl_context);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(enigma::windowHandle);
  return result;
}

} // namespace enigma_user
