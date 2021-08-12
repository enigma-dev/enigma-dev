/** Copyright (C) 2011 Josh Ventura
*** Copyright (C) 2013-2014 Robert B. Colton
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

#ifndef ENIGMA_PLATFORM_MAIN
#define ENIGMA_PLATFORM_MAIN

#include "Universal_System/directoryglobals.h"

#include <string>

namespace enigma {
  extern bool game_isending;
  extern int game_return;
  extern int parameterc;
  extern std::string* parameters;
  extern int pausedSteps;
  extern int current_room_speed;
  extern int frames_count;
  extern unsigned long current_time_mcs;
  extern bool game_window_focused;

  int enigma_main(int argc, char** argv);
  int game_ending();
  void Sleep(int ms);
  void compute_window_scaling();
  void compute_window_size();
  void set_program_args(int argc, char** argv);
  void platform_focus_lost();
  void platform_focus_gained();
  void initTimer();
  int updateTimer();
  int gameWait();
  void set_room_speed(int rs);

  // This method should write the name of the running module to exenamehere.
  inline void windowsystem_write_exename(char **exenamehere) { 
    *exenamehere = (char *)enigma_user::program_pathname.c_str(); 
  }

}
  
namespace enigma_user {

extern std::string keyboard_string;
extern double fps;
extern unsigned long delta_time;
extern unsigned long current_time;

void sleep(int ms);
void game_end();
void game_end(int ret);
void action_end_game();

// Data type could be unsigned for the paramter since the collection is size_t, however this would make the function not behave as GM.
// show_message(parameter_string(-1)); in GM8.1 will show an empty string, if this function cast the parameter to unsigned that won't be the behavior.
std::string parameter_string(int x);
// This function does not work the same as GM8.1, while it returns the native argument count, GM8.1 reports an argument count of 0 for an empty game, even
// though the first parameter which is the filename is successfully returned from parameter_string. So the assumption is GM8.1's version always reports 1 less
// than the ENIGMA version.
int parameter_count();
unsigned long long disk_size(std::string drive);
unsigned long long disk_free(std::string drive);

void execute_shell(std::string fname, std::string args);
void execute_program(std::string fname, std::string args, bool wait);
std::string execute_shell_for_output(const std::string &command);
void url_open(std::string url);
void action_webpage(const std::string &url);

void set_program_priority(int value);
std::string filename_absolute(std::string fname);
std::string filename_join(std::string prefix, std::string suffix);
std::string environment_get_variable(std::string name);
bool environment_set_variable(const std::string &name, const std::string &value);
bool environment_unset_variable(const std::string &name);
bool set_working_directory(std::string dname);

} // namespace enigma_user

#endif //ENIGMA_PLATFORM_MAIN
