/** Copyright (C) 2014, 2018 Robert B. Colton
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

#ifndef ENIGMA_ASYNCDIALOG_H
#define ENIGMA_ASYNCDIALOG_H

#include <string>
using std::string;

namespace enigma {

void extension_async_init();

} // namespace enigma

namespace enigma_user {
  extern unsigned async_load;

  int show_message_async(string str);
  int show_question_async(string str);
  int get_string_async(string str, string def);
  int get_integer_async(string str, double def);
  int get_login_async(string username, string password);
}

#endif // ENIGMA_ASYNCDIALOG_H
