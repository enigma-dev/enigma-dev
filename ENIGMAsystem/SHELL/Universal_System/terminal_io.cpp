/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <stdio.h>
#include <string>
using namespace std;

void cons_show_message(string message) {
  puts(message.c_str());
}
void cons_print_overwritable(string message) {
  printf("%s\r",message.c_str());
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
  int c, first;
  first = c = getchar();
  while (c != '\n' && c != EOF)
  {
    res += char(c);
    c = getchar();
  }
  return res;
}
