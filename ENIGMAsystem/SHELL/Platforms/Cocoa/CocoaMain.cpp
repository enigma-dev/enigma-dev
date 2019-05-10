/*
 * Copyright (C) 2008 IsmAvatar <cmagicj@nni.com>
 * Copyright (C) 2010 Alasdair Morrison <tgmg@g-java.com>
 * Copyright (C) 2014 Seth N. Hetu
 *
 * This file is part of ENIGMA.
 *
 * ENIGMA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ENIGMA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License (COPYING) for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <stdio.h>
#include <mach-o/dyld.h>
#include <limits.h>
#include "CocoaMain.h"
#include "ObjectiveC.h"
#include <cstdlib>
#include <string>

#include "../General/PFwindow.h"
#include "../General/PFfilemanip.h"

#include "Platforms/General/PFmain.h"
#include "Universal_System/roomsystem.h"

using std::string;

namespace enigma {

void SetResizeFptr();

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

void initialize_directory_globals() {
  // Set the working_directory
  char buffer[PATH_MAX + 1];
  if (getcwd(buffer, PATH_MAX + 1) != NULL)
    enigma_user::working_directory = add_slash(buffer);

  // Set the program_directory
  buffer[0] = 0;

  uint32_t bufsize = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &bufsize) == 0) {
    enigma_user::program_directory = add_slash(dirname(buffer));
  }

  // Set the temp_directory
  char *env = getenv("TMPDIR");

  if (env != NULL)
    enigma_user::temp_directory = add_slash(env);
  else
    enigma_user::temp_directory = "/tmp/";
}

} // namespace enigma

int main(int argc,char** argv) {
  enigma::initialize_directory_globals();
  enigma::parameters=new char* [argc];
  for (int i=0; i<argc; i++)
    enigma::parameters[i]=argv[i];

   enigma::SetResizeFptr();

  return mainO(argc, argv);
}

namespace enigma_user {

void sleep(int ms) {
  if (ms > 1000) ::sleep(ms/1000);
  usleep((ms % 1000) *1000);
};

int parameter_count() {
  // TODO
  return 0;
}

string parameter_string(int n) {
  // TODO
  return string("");
}

}
