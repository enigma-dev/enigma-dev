/*

 MIT License

 Copyright © 2021-2025 Samuel Venable

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

namespace enigma_user {

int widget_get_theme();
void widget_set_theme(int theme);
string widget_get_caption();
void widget_set_caption(string title);
bool show_question(std::string message);
int show_message_ext(std::string message, std::string button1 = "", std::string button2 = "", std::string button3 = "");
std::string get_string(std::string message, std::string defstr);
double get_number(std::string message, double defnum);
double get_integer(std::string message, double defint);
std::string get_open_filename(std::string filter, std::string fname);
std::string get_open_filename_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_open_filenames(std::string filter, std::string fname);
std::string get_open_filenames_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_save_filename(std::string filter, std::string fname);
std::string get_save_filename_ext(std::string filter, std::string fname, std::string title, std::string dir);
std::string get_directory(std::string dname);
std::string get_directory_alt(std::string capt, std::string root);
inline bool action_if_question(std::string message) {
  return show_question(message);
}

} // namespace enigma_user
