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
using enigma_user::filename_ext;

namespace enigma {

void SetResizeFptr();

static inline void initialize_working_directory() {
  /*
    This function will set the working directory to the app bundle's Resources folder 
    like GM4Mac 7.5, GMStudio 1.4, GMS 2.x and most Mac apps do, if the executable is in
    an app bundle. If the executable is not in an app bundle, use the getcwd function
    
    ONLY use working_directory for loading read-only included files! When SAVING, use game_save_id
    
    *_bname = base name - removes the full path from the string leaving just the file or folder name
    *_dname = directory name - removes final slash and base name from full path to file or folder name
    *_pname = path name - removes the base name from a full path while keeping the dir and final slash
    *_ename = extension name - includes everything in bname at and following the period if one exists
  */
  
  bool success = false; 
  const string exe_pname = enigma_user::program_directory;      // = "/Path/To/YourAppBundle.app/Contents/MacOS/";
  const string macos_dname = filename_dir(exe_pname);           // = "/Path/To/YourAppBundle.app/Contents/MacOS";
  const string macos_bname = filename_name(macos_dname);        // = "MacOS";
  const string contents_dname = filename_dir(macos_dname);      // = "/Path/To/YourAppBundle.app/Contents";
  const string contents_bname = filename_name(contents_dname);  // = "Contents";
  const string app_dname = filename_dir(contents_dname);        // = "/Path/To/YourAppBundle.app";
  const string app_ename = filename_ext(app_dname);             // = ".app";
  const string contents_pname = filename_path(macos_dname);     // = "/Path/To/YourAppBundle.app/Contents/";
  const string resources_pname = contents_pname + "Resources/"; // = "/Path/To/YourAppBundle.app/Contents/Resources/";
  
  // if "/Path/To/YourAppBundle.app/Contents/MacOS/YourExe" and "/Path/To/YourAppBundle.app/Contents/Resources/" exists
  if (macos_bname == "MacOS" && contents_bname == "Contents" && app_ename == ".app" && directory_exists(resources_pname)) {
    // set working directory to "/Path/To/YourAppBundle.app/Contents/Resources/" and allow loading normal included files
    success = set_working_directory(resources_pname);
  }
  
  if (!success) { // if the app bundle is not structured correctly
    // then assume set working directory to the Unix working directory
    char buffer[PATH_MAX];
    if (getcwd(buffer, PATH_MAX) != nullptr)
      success = set_working_directory(buffer);
  }
  
  // should getcwd() and chdir() fail, then
  // set working directory to empty string:
  if (!success) success = set_working_directory("");
  
  /* 
    if (success) enigma_user::show_message("Success!"); else enigma_user::show_message("Failure!");
    enigma_user::get_string("The current value of working_directory equals:", enigma_user::working_directory); 
  */
}

static inline string add_slash(const string& dir) {
  if (dir.empty() || *dir.rbegin() != '/') return dir + '/';
  return dir;
}

void initialize_directory_globals() {
  /* TODO: Add global "game_save_id" (which is the 
  *required* sandbox directory for Mac apps)... */

  // Set the working_directory
  char buffer[PATH_MAX];
  if (getcwd(buffer, PATH_MAX) != nullptr)
    enigma_user::working_directory = add_slash(buffer);

  // Set the program_directory
  char buffer[PATH_MAX];
  uint32_t bufsize = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &bufsize) == 0) {
    std::string symLink = buffer;
    if (realpath(symLink.c_str(), buffer)) {
      enigma_user::program_directory = filename_path(buffer);
      enigma_user::program_directory = enigma_user::filename_path(buffer);
    }
  }
  
  // Set the working_directory
  initialize_working_directory();

  // Set the temp_directory
  char *env = getenv("TMPDIR");
  enigma_user::temp_directory = env ? add_slash(env) : "/tmp/";
  
  // Set the game_save_id
  enigma_user::game_save_id = add_slash(enigma_user::environment_get_variable("HOME")) + 
    string(".config/") + add_slash(std::to_string(enigma_user::game_id));
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
