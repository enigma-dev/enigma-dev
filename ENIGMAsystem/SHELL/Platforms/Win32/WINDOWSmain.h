/****\
**  *|
**  Copyright (C) 2011 Josh Ventura and Robert B. Colton
**
**  This file is a part of the ENIGMA Development Environment.
**
**
**  ENIGMA is free software: you can redistribute it and/or modify it under the
**  terms of the GNU General Public License as published by the Free Software
**  Foundation, version 3 of the license or any later version.
**
**  This application and its source code is distributed AS-IS, WITHOUT ANY
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
**  details.
**
**  You should have recieved a copy of the GNU General Public License along
**  with this code. If not, see <http://www.gnu.org/licenses/>
**  *|
\****/

#include <windows.h>

namespace enigma //TODO: Find where this belongs
{
  extern HINSTANCE hInstance;
  extern HWND hWndParent;
  extern HWND hWnd;
  extern HDC window_hDC;
}

namespace enigma_user
{

string parameter_string(int x);
int parameter_count();
unsigned long long disk_size(std::string drive);
unsigned long long disk_free(std::string drive);

void set_program_priority(int value);
void execute_shell(std::string fname, std::string args);
void execute_program(std::string fname, std::string args, bool wait);

std::string environment_get_variable(std::string name);

void registry_write_string(std::string name, std::string str);
void registry_write_real(std::string name, int x);
std::string registry_read_string(std::string name);
int registry_read_real(std::string name);
bool registry_exists(std::string name);
void registry_write_string_ext(std::string key, std::string name, std::string str);
void registry_write_real_ext(std::string key, std::string name, int x);
std::string registry_read_string_ext(std::string key, std::string name);
int registry_read_real_ext(std::string key, std::string name);
bool registry_exists_ext(std::string key, std::string name);
void registry_set_root(int root);
unsigned long long window_handle();

}

