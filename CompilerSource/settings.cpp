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
  bool literal_autocast = 0; // Determines how literals are treated.                 0 = enigma::variant,   1 = C++ scalars
  bool inherit_objects = 0;  // Determines whether objects should automatically inherit locals and events from their parents
  bool automatic_semicolons = 0; // Determines whether semicolons should automatically be added or if the user wants strict syntax
  COMPLIANCE_LVL compliance_mode = COMPL_STANDARD;
  std::string keyword_blacklist = "";
}

CompilerInfo compilerInfo;

void map_ey(ey_data& key, std::map<std::string, std::string>& map) {
  for (const auto& v : key) {
    map[toUpper(v.first)] = v.second->data().scalar().toString();
    std::cout << toUpper(v.first) << "=\""; 
    std::cout << v.second->data().scalar().toString() << "\"" << std::endl;
  }
}

bool load_compiler_ey(std::string fPath) {

  std::cout << std::endl << "Loading compiler ey file: " << fPath << std::endl;

  // Clear old info
  compilerInfo = CompilerInfo(); 

  std::ifstream compiler_ifstream(fPath.c_str());

  // Bail if error
  if (!compiler_ifstream.is_open())
    return false;

  // Parse our compiler data file
  ey_data compiler_yaml = parse_eyaml(compiler_ifstream, fPath.c_str());

  // Write down our top level ey fields (Note yaml toLowers all fields)
  compilerInfo.name = compiler_yaml.get("name");
  compilerInfo.maintainer = compiler_yaml.get("maintainer");
  compilerInfo.target_platform = compiler_yaml.get("target-platform");

  // Write down grouped fields 
  const std::map<std::string, std::map<std::string, std::string>&> pairs = {
      {"parser-vars", compilerInfo.parser_vars},
      {"make-vars", compilerInfo.make_vars},
      {"exe-vars", compilerInfo.exe_vars}};
  
  for (const auto& p : pairs) {
    std::cout << "[" << p.first << "]" << std::endl;
    ey_base* keyPtr = compiler_yaml.values[p.first];
    if (keyPtr != nullptr && !keyPtr->is_scalar)  {
      ey_data& key = keyPtr->data();
      map_ey(key, p.second);
    }
  }

  return true;
}

std::string compiler_map_get(std::string key, const std::map<std::string, std::string>& map) {
  auto it = map.find(key);
  if (it != map.end()) return it->second;
  return "";
}
