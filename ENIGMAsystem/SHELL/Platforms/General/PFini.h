/** Copyright (C) 2008 Josh Ventura
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

namespace enigma_user
{

void ini_open(std::string fname);
void ini_close();
std::string ini_read_string(std::string section, std::string key, string defaultValue);
int ini_read_real(std::string section, std::string key, int defaultValue);
void ini_write_string(std::string section, std::string key, string value);
void ini_write_real(std::string section, std::string key, int value);
bool ini_key_exists(std::string section, std::string key);
bool ini_section_exists(std::string section);
void ini_key_delete(std::string section, std::string key);
void ini_section_delete(std::string section);

}

