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

namespace enigma_user
{

extern std::string working_directory;
extern std::string program_directory;

void game_end(int ret=0);
void action_end_game();
void action_webpage(const std::string &url);

string parameter_string(int x);
int parameter_count();
unsigned long long disk_size(std::string drive);
unsigned long long disk_free(std::string drive);

void set_program_priority(int value);
void execute_shell(std::string fname, std::string args);
void execute_shell(std::string operation, std::string fname, std::string args);
void execute_program(std::string fname, std::string args, bool wait);
void execute_program(std::string operation, std::string fname, std::string args, bool wait);

std::string environment_get_variable(std::string name);

}

#endif //ENIGMA_PLATFORM_MAIN
