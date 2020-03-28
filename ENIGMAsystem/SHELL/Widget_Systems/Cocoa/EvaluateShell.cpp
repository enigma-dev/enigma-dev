/** Copyright (C) 2019 Samuel Venable
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

#include "EvaluateShell.h"
#include "Platforms/General/PFmain.h"

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

const char *cpp_concat(const char *s1, const char *s2) {
  static string str_result;
  str_result = string(s1) + string(s2);
  return str_result.c_str();
}

const char *evaluate_shell(const char *command) {
  string str_buffer = execute_shell_for_output(command);
  if (str_buffer.back() == '\n') str_buffer.pop_back();
  static string str_result; str_result = str_buffer;
  return str_result.c_str();
}
