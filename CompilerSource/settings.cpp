/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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

#include "settings.h"
#include "utility.h"

#include "eyaml/eyaml.h"

#include <fstream>
#include <iostream>

namespace extensions
{
  sdk_descriptor targetSDK;
  api_descriptor targetAPI;
}

namespace setting
{
  //Compatibility / Progess options
  bool use_cpp_strings = 0;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  bool use_cpp_escapes = 0;  // Defines what language strings are inherited from.    0 = GML,               1 = C++
  bool use_gml_equals = 0;   // Defines what language operator= is inherited from.   0 = C++,               1 = GML
  bool use_incrementals = 0; // Defines how operators ++ and -- are treated.         0 = GML,               1 = C++
  bool literal_autocast = 0; // Determines how literals are treated.                 0 = enigma::evariant,   1 = C++ scalars
  bool inherit_objects = 0;  // Determines whether objects should automatically inherit locals and events from their parents
  bool automatic_semicolons = 0; // Determines whether semicolons should automatically be added or if the user wants strict syntax
  COMPLIANCE_LVL compliance_mode = COMPL_STANDARD;
  std::string keyword_blacklist = "";
}

CompilerInfo compilerInfo;

void map_ey(ey_data& key, std::map<std::string, std::string>& map) {
  for (const auto& v : key) {
    map[toUpper(v.first)] = v.second->data().scalar().toString();
    std::cout << toUpper(v.first) << "=\""; // The makefiles expect caps
    std::cout << v.second->data().scalar().toString() << "\"" << std::endl;
  }
}

inline bool get_ey_field(ey_base* keyPtr, std::string field, std::string& str) {
  ey_data& key = keyPtr->data();
  str = key.get(field);
  if (str.empty()) {
    std::cerr << "Error: misssing required field: \"" << field << "\"" << std::endl; 
  } else if (str.length() > 2 && (str.front() == '"') && (str.back() == '"')) { // Josh's shitty yaml parser is copying quotes from quoted fields
    str = str.substr(1, str.length()-2);
  }
  return !str.empty();
}

bool load_compiler_ey(std::string fPath) {

  bool success = true; // ugly hacks cause josh can't program 

  std::cout << std::endl << "Loading compiler ey file: " << fPath << std::endl;

  // Clear old info
  compilerInfo = CompilerInfo(); 

  std::ifstream compiler_ifstream(fPath.c_str());

  // Bail if error
  if (!compiler_ifstream.is_open())
    return false;

  // Parse our compiler data file
  ey_data compiler_yaml = parse_eyaml(compiler_ifstream, fPath.c_str());

  // Write down our top level ey fields (Note yaml toLowers all field names)
  success &= get_ey_field(&compiler_yaml, "name", compilerInfo.name);
  success &= get_ey_field(&compiler_yaml, "maintainer", compilerInfo.maintainer);
  success &= get_ey_field(&compiler_yaml, "target-platform", compilerInfo.target_platform);

  std::string native;
  success &= get_ey_field(&compiler_yaml, "native", native);
  compilerInfo.native = toUpper(native) == "YES";

  // Write down required parser things
  ey_base* parserKeyPtr = compiler_yaml.values["parser-vars"];
  if (parserKeyPtr == nullptr || parserKeyPtr->is_scalar) {
    std::cerr << "Error: missing Parser-Vars group" << std::endl;
    return false;
  } 

  success &= get_ey_field(parserKeyPtr, "defines", compilerInfo.defines_cmd);
  success &= get_ey_field(parserKeyPtr, "searchdirs", compilerInfo.searchdirs_cmd);
  success &= get_ey_field(parserKeyPtr, "searchdirs-start", compilerInfo.searchdirs_start);
  success &= get_ey_field(parserKeyPtr, "searchdirs-end", compilerInfo.searchdirs_end);

  // Write down make vars which can be literally anything to maps
  ey_base* makeKeyPtr = compiler_yaml.values["make-vars"];
  if (makeKeyPtr == nullptr || makeKeyPtr->is_scalar) {
    std::cerr << "Warning: missing Make-Vars group" << std::endl;
  } else {
    map_ey(makeKeyPtr->data(), compilerInfo.make_vars);
  }

  // Write down exe and runtime args which can be literally anything to map
  ey_base* exeKeyPtr = compiler_yaml.values["exe-vars"];
  if (exeKeyPtr == nullptr || exeKeyPtr->is_scalar) {
    std::cerr << "Error: missing EXE-Vars group" << std::endl;
  } else {
    map_ey(exeKeyPtr->data(), compilerInfo.exe_vars);
  }

  // TODO: everything in exe-vars should be optional but for now the compiler will break if some things aren't set
  if (compilerInfo.exe_vars.find("RESOURCES") == compilerInfo.exe_vars.end()) {
    std::cerr << "Error: misssing required field: Resources" << std::endl; 
    success = false;
  }

  return success;
}
