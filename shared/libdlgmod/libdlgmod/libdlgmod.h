/*

 MIT License

 Copyright © 2021-2024 Samuel Venable

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

namespace dialog_module {

  int show_message(char *str);
  int show_message_cancelable(char *str);
  int show_question(char *str);
  int show_question_cancelable(char *str);
  int show_attempt(char *str);
  int show_error(char *str, bool abort);
  char *get_string(char *str, char *def);
  char *get_password(char *str, char *def);
  double get_integer(char *str, double def);
  double get_passcode(char *str, double def);
  char *get_open_filename(char *filter, char *fname);
  char *get_open_filename_ext(char *filter, char *fname, char *dir, char *title);
  char *get_open_filenames(char *filter, char *fname);
  char *get_open_filenames_ext(char *filter, char *fname, char *dir, char *title);
  char *get_save_filename(char *filter, char *fname);
  char *get_save_filename_ext(char *filter, char *fname, char *dir, char *title);
  char *get_directory(char *dname);
  char *get_directory_alt(char *capt, char *root);
  int get_color(int defcol);
  int get_color_ext(int defcol, char *title);
  char *widget_get_caption();
  void widget_set_caption(char *str);
  char *widget_get_owner();
  void widget_set_owner(char *hwnd);
  char *widget_get_icon();
  void widget_set_icon(char *icon);
  char *widget_get_system();
  void widget_set_system(char *sys);
  void widget_set_button_name(int type, char *name);
  char *widget_get_button_name(int type);
  bool widget_get_canceled();
  
} // namespace dialog_module


