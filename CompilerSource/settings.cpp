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

#include <yaml-cpp/yaml.h>

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

void map_ey(const YAML::Node& node, std::map<std::string, std::string>& map) {
  //std::cout << node << std::endl;
  for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
    std::string val = (it->second.IsNull()) ? "" : it->second.as<std::string>();
    map[toUpper(it->first.as<std::string>())] = val;
  }
}

inline bool get_ey_field(const YAML::Node& node, const std::string& field, std::string& str) {
  str = node[field].as<std::string>();
  if (str.empty()) {
    std::cerr << "Failed to lookup yaml field: " << field << std::endl;
    return false;
  }
  return true;
}

bool load_compiler_ey(const std::filesystem::path& fPath) {

  bool success = true; // ugly hacks cause josh can't program

  std::cout << std::endl << "Loading compiler ey file: " << fPath << std::endl;

  // Parse our compiler data file
  const YAML::Node compiler_yaml = YAML::LoadFile(fPath.u8string());

  // Bail if error
  if (!compiler_yaml.IsDefined())
    return false;

  // Clear old info
  compilerInfo = CompilerInfo();

  // Write down our top level ey fields (Note yaml toLowers all field names)
  success &= get_ey_field(compiler_yaml, "name", compilerInfo.name);
  success &= get_ey_field(compiler_yaml, "maintainer", compilerInfo.maintainer);
  success &= get_ey_field(compiler_yaml, "target-platform", compilerInfo.target_platform);

  std::string native;
  success &= get_ey_field(compiler_yaml, "native", native);
  compilerInfo.native = toUpper(native) == "YES";

  // Write down required parser things
  const YAML::Node& parser_node = compiler_yaml["parser-vars"];
  if (!parser_node.IsDefined() || !parser_node.IsMap()) {
    std::cerr << "Error: missing Parser-Vars group" << std::endl;
    return false;
  }

  success &= get_ey_field(parser_node, "defines", compilerInfo.defines_cmd);
  success &= get_ey_field(parser_node, "searchdirs", compilerInfo.searchdirs_cmd);
  success &= get_ey_field(parser_node, "searchdirs-start", compilerInfo.searchdirs_start);
  success &= get_ey_field(parser_node, "searchdirs-end", compilerInfo.searchdirs_end);

  // Write down make vars which can be literally anything to maps
  const YAML::Node& make_node = compiler_yaml["make-vars"];
  if (!make_node.IsDefined() || !make_node.IsMap()) {
    std::cerr << "Warning: missing Make-Vars group" << std::endl;
  } else {
    map_ey(make_node, compilerInfo.make_vars);
  }

  // Write down exe and runtime args which can be literally anything to map
  const YAML::Node& exe_node = compiler_yaml["exe-vars"];
  if (!exe_node.IsDefined() || !exe_node.IsMap()) {
    std::cerr << "Error: missing EXE-Vars group" << std::endl;
  } else {
    map_ey(exe_node, compilerInfo.exe_vars);
  }

  // TODO: everything in exe-vars should be optional but for now the compiler will break if some things aren't set
  if (compilerInfo.exe_vars.find("RESOURCES") == compilerInfo.exe_vars.end()) {
    std::cerr << "Error: misssing required field: Resources" << std::endl;
    success = false;
  }

  return success;
}
