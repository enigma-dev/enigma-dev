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
#include <sys/stat.h>
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
  
  void set_working_directory() {
    // Set the working_directory
    char buffer[PATH_MAX + 1];
    if (getcwd(buffer, PATH_MAX + 1) != NULL)
      enigma_user::working_directory = buffer;

    // Set the program_directory
    buffer[0] = 0;
    char real_executable[PATH_MAX + 1];
    char *bundle_id;

    uint32_t bufsize = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &bufsize) == 0) {
      bundle_id = dirname(buffer);
      strcpy(real_executable, bundle_id);
      strcat(real_executable, "/");

      enigma_user::program_directory = real_executable;
    }

    // Set the temp_directory
    char const *env = getenv("TMPDIR");

    if (env == 0)
      env = "/tmp/";

    enigma_user::temp_directory = env; 
  }
}

namespace enigma_user {
  double set_working_directory(string dname) {
    if (!dname.empty()) {
      while (*dname.rbegin() == '/') {
        dname.erase(dname.size() - 1);
      }
    }

    struct stat sb;
    if (stat((char *)dname.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
      return (chdir((char *)dname.c_str()) == 0);

    return 0;
  }
}

int main(int argc,char** argv) {
  enigma::set_working_directory();
  
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

  string environment_get_variable(string name) {
    // TODO
    return string("");
  }

}
