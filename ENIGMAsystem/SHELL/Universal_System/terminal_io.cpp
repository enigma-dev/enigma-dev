/** Copyright (C) 2010-2011 Josh Ventura
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

#include <stdio.h>
#include <string>
using namespace std;
#include "terminal_io.h"

#ifdef DEBUG_MODE
  #define debug_message(message)\
    puts(message.c_str());
#else
  #define debug_message(message)\
    (void)0;
#endif

void cons_show_message(string message) {
  puts(message.c_str());
}
void cons_print_overwritable(string message) {
  printf("%s\r",message.c_str());
}
void show_debug_message(string message) {
  debug_message(message);
}
char cons_get_byte()
{
  int c, first;
  first = c = getchar();
  while (c != '\n' && c != EOF)
  c = getchar();
  return first;
}
string cons_get_char()
{
  int c, first;
  first = c = getchar();
  while (c != '\n' && c != EOF)
  c = getchar();
  return string(first,1);
}
string cons_get_string()
{
  string res;
  int c = getchar();
  while (c != '\n' && c != EOF)
  {
    res += char(c);
    c = getchar();
  }
  return res;
}
