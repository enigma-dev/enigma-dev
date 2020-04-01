/** Copyright (C) 2008-2017 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
*** Copyright (C) 2020 Samuel Venable
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

#include "procmanip.h"
#include <string>

using std::string;

namespace enigma_user {

string path_from_pid(pid_t pid) {
  return proc_manip::path_from_pid(pid);
}

pid_t ppid_from_pid(pid_t pid) {
  return proc_manip::ppid_from_pid(pid);
}

pid_t pid_from_wid(string wid) {
  return proc_manip::pid_from_wid(wid);
}

}
