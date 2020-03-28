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

#include "EvaluateShell.h"

#include <sys/stat.h>

#include <cstdio>
#include <cstdlib>

#include <string>

using std::string;

const char *integer_to_cstring(int n) {
  static string str_result;
  str_result = std::to_string(n);
  return str_result.c_str();
}

int cstring_to_integer(const char *s) {
  return (int)strtol(s, NULL, 10);
}

bool file_exists(const char *fname) {
  struct stat sb;
  return (stat(fname, &sb) == 0 &&
    S_ISREG(sb.st_mode) != 0);
}

const char *cpp_concat(const char *s1, const char *s2) {
  static string str_result;
  str_result = string(s1) + string(s2);
  return str_result.c_str();
}

const char *evaluate_shell(const char *command) {
  char *buffer = NULL;
  size_t buffer_size = 0;
  string str_buffer;

  FILE *file = popen(command, "r");
  while (getline(&buffer, &buffer_size, file) != -1)
    str_buffer += buffer;

  free(buffer);
  pclose(file);

  if (str_buffer.back() == '\n')
    str_buffer.pop_back();

  static string str_result;
  str_result = str_buffer;

  return str_result.c_str();
}
