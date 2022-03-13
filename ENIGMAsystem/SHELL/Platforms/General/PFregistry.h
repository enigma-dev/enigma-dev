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

#ifndef ENIGMA_PLATFORM_REGISTRY
#define ENIGMA_PLATFORM_REGISTRY

namespace enigma_user {

  bool registry_write_string(string name, string str);
  bool registry_write_real(string name, unsigned long val);
  string registry_read_string(string name);
  unsigned long registry_read_real(string name);
  bool registry_exists(string name);
  bool registry_write_string_ext(string subpath, string name, string str);
  bool registry_write_real_ext(string subpath, string name, unsigned long val);
  string registry_read_string_ext(string subpath, string name);
  unsigned long registry_read_real_ext(string subpath, string name);
  bool registry_exists_ext(string subpath, string name);
  string registry_get_path();
  bool registry_set_path(string subpath);
  string registry_get_key();
  bool registry_set_key(string keystr);

}

#endif //ENIGMA_PLATFORM_REGISTRY
