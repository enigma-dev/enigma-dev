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
#include "Platforms/General/PFfilemanip.h"
#include "Universal_System/roomsystem.h"
#include "Universal_System/estring.h"

/* Uncomment the 2 lines below for testing generate_working_directory() success/failure */
// #include "Widget_Systems/widgets_mandatory.h"
// #include "Widget_Systems/Cocoa/dialogs.h"

using std::string;

using enigma_user::set_working_directory;
using enigma_user::directory_exists;
using enigma_user::filename_name;
using enigma_user::filename_dir;
using enigma_user::filename_path;

namespace enigma {

void SetResizeFptr();

static inline void generate_working_directory() {
  /* This function will set the working directory to the app bundle's Resources folder 
  like GM4Mac 7.5, GMStudio 1.4, GMS 2.x and most Mac apps do, if the executable is in
  an app bundle. If the executable is not in an app bundle, use the getcwd function */
  bool success; string macos_bname; string contents_bname; string resources_path;
  success = false; macos_bname = filename_name(filename_dir(get_program_directory()));
  contents_bname = filename_name(filename_dir(filename_dir(enigma_user::program_directory)));
  resources_path = filename_path(filename_dir(enigma_user::program_directory)) + "Resources/";
  if (directory_exists(resources_path) && macos_bname == "MacOS" && contents_bname == "Contents")
  { success = set_working_directory(resources_path); enigma_user::working_directory = resources_path; }
  if (!success) { char buffer[PATH_MAX]; success = (getcwd(buffer, PATH_MAX) != NULL);
  enigma_user::working_directory = add_slash(buffer); } if (!success) 
  { set_working_directory(""); enigma_user::working_directory = ""; }
  /* if (success) enigma_user::show_message("Success!"); else enigma_user::show_message("Failure!");
  enigma_user::get_string("The current value of working_directory equals:", enigma_user::working_directory); */
  /* ONLY use working_directory for loading read-only included files! When SAVING, use game_save_id */
}

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

void initialize_directory_globals() {
  /* TODO: Add global "game_save_id" (which is the 
  *required* sandbox directory for Mac apps)... */
  
  // Set the program_directory
  char buffer[PATH_MAX];
  uint32_t bufsize = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &bufsize) == 0) {
    enigma_user::program_directory = filename_path(buffer);
  }
  
  // Set the working_directory
  generate_working_directory();

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
