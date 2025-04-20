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

#include "Widget_Systems/None/dialogs.h"
#include "Widget_Systems/widgets_mandatory.h"

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
}

int show_message(const string &message) {
  return 0;
}

bool show_question(string message) {
  return false;
}

int show_question_ext(string message) {
  return 0;
}

std::string get_string(std::string message, std::string defstr) {
  return "";
}

double get_number(std::string message, double defnum) {
  return 0;
}

double get_integer(std::string message, double defint) {
  return 0;
}

string get_open_filename(string filter, string fname) {
  return "";
}

string get_open_filename_ext(string filter, string fname, string title, string dir) {
  return "";
}

string get_open_filenames(string filter, string fname) {
  return "";
}

string get_open_filenames_ext(string filter, string fname, string title, string dir) {
  return "";
}

string get_save_filename(string filter, string fname) { 
  return "";
}

string get_save_filename_ext(string filter, string fname, string title, string dir) {
  return "";
}

string get_directory(string dname) {
  return "";
}

string get_directory_alt(string capt, string root) {
  return "";
}

} // namespace enigma_user
