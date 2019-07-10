/********************************************************************************\
**                                                                              **
**  Copyright (C) 2012 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
**  Copyright (C) 2019 Samuel Venable                                           **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#include <windows.h>
#include <cwchar>
#include <string>

using std::to_string;
using std::string;

#include "PFini.h"
#include "Universal_System/estring.h"

static tstring iniFilename = L"";

namespace enigma_user {

void ini_open(string fname) {
  wchar_t rpath[MAX_PATH];
  tstring tstr_fname = widen(fname);
  GetFullPathNameW(tstr_fname.c_str(), MAX_PATH, rpath, NULL);
  iniFilename = rpath;
}

void ini_close() {
  iniFilename = L"";
}

string ini_read_string(string section, string key, string defaultValue) {
  wchar_t buffer[1024];
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  tstring tstr_defaultValue = widen(defaultValue);
  GetPrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), tstr_defaultValue.c_str(), buffer, 1024, iniFilename.c_str());

  return shorten(buffer);
}

float ini_read_real(string section, string key, float defaultValue) {
  wchar_t res[255];
  wchar_t def[255];
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  GetPrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), def, res, 255, iniFilename.c_str());
  string result = shorten(res);
  return (float)atof(result.c_str());
}

void ini_write_string(string section, string key, string value) {
  tstring tstr_value = widen(value);
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  WritePrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), tstr_value.c_str(), iniFilename.c_str());
}

void ini_write_real(string section, string key, float value) {
  string str_value = to_string(value);
  tstring tstr_value = widen(str_value);
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  WritePrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), tstr_value.c_str(), iniFilename.c_str());
}

bool ini_key_exists(string section, string key) {
  wchar_t buffer[1024];
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  return GetPrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), L"", buffer, 1024, iniFilename.c_str()) != 0;
}

bool ini_section_exists(string section) {
  wchar_t buffer[1024];
  tstring tstr_section = widen(section);
  return GetPrivateProfileSectionW(tstr_section.c_str(), buffer, 1024, iniFilename.c_str()) != 0;
}

void ini_key_delete(string section, string key) {
  tstring tstr_section = widen(section);
  tstring tstr_key = widen(key);
  WritePrivateProfileStringW(tstr_section.c_str(), tstr_key.c_str(), NULL, iniFilename.c_str());
}

void ini_section_delete(string section) {
  tstring tstr_section = widen(section);
  WritePrivateProfileStringW(tstr_section.c_str(), NULL, NULL, iniFilename.c_str());
}

} // namespace enigma_user
