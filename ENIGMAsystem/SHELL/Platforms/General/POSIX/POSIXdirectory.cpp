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

#include "Platforms/General/PFmain.h"
#include "Platforms/General/PFfilemanip.h"
#include "Universal_System/estring.h"

#include <limits.h>
#include <unistd.h>
#include <string>

using std::string;

using enigma_user::file_exists;
using enigma_user::directory_exists;

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

namespace enigma_user {

std::string filename_join(std::string prefix, std::string suffix) {
  return add_slash(prefix) + suffix;
}

string environment_get_variable(string name) {
  char *env = getenv(name.c_str());
  return env ? env : "";
}

// deletes the environment variable if set to empty string
bool environment_set_variable(const string &name, const string &value) {
  if (value == "") return (unsetenv(name.c_str()) == 0);
  return (setenv(name.c_str(), value.c_str(), 1) == 0);
}

} // namespace enigma_user
