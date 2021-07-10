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
#ifndef ENIGMA_SETTINGS_H
#define ENIGMA_SETTINGS_H

#include "strings_util.h"

#include <string>
#include <map>
#include <filesystem>
#include <iostream>

namespace extensions
{
  struct sdk_descriptor {
    std::string name, identifier, represents, description, author, build_platforms;
  };
  
  struct api_descriptor
  {
    std::string
      windowSys,   graphicsSys,   audioSys,   collisionSys,   widgetSys,   networkSys;
    std::string
      windowLinks, graphicsLinks, audioLinks, collisionLinks, widgetLinks, networkLinks;
  };

  extern sdk_descriptor targetSDK;
  extern api_descriptor targetAPI;
}

namespace setting {

//Compliance levels. These enforce various settings specific to certain versions of GM.
//Prefer generalized solutions; use these for direct incompatibilities. (Add other GM versions as needed).
enum COMPLIANCE_LVL {
  COMPL_GM5 = 50,       //GM5 compliance. timeline_running will default to "true". exit will abort single code actions. Sprite and background alphas default to 1
  COMPL_GM6 = 60,       //Same as GM5 for now
  COMPL_GM7 = 70,       //Same as GM5 for now
  COMPL_GM8 = 80,         //GM8 compliance. exit will abort single code actions. Sprite and background alphas default to 1.
  COMPL_STANDARD = 65535,    //Standard (enigma) compliance. Default and recommended. High so we can do things like compliance_mode<=8
};

//Compatibility / Progess options
struct CompatibilityOptions {
  bool use_cpp_strings =  0; //< Defines what language single-quote strings are inherited from.     0 = GML, 1 = C++
  bool use_cpp_literals = 0; //< Defines what language numeric literals are inherited from.         0 = GML, 1 = C++
  bool use_cpp_escapes =  0; //< Defines what language string escape sequences are inherited from.  0 = GML, 1 = C++
  bool use_gml_equals =   0; //< Defines what language operator= is inherited from.                 0 = GML, 1 = C++
  bool use_incrementals = 0; //< Defines how operators ++ and -- are treated (assuming GM6).        0 = GML, 1 = C++
  bool inherit_objects =  0; //< Determines whether objects should automatically inherit locals and events from their parents
  bool strict_syntax =    0; //< Determines whether semicolons and parentheses should be inferred or if the user prefers strict syntax.
  COMPLIANCE_LVL compliance_mode  = COMPL_STANDARD; //< How to resolve differences between GM versions.
  std::string keyword_blacklist;  //< Words to blacklist from user scripts, separated by commas.
};

}  // namespace setting

struct CompilerInfo {

  // these are  needed or the ide will self destruct
  std::string name;
  std::string maintainer;
  std::string target_platform;
  bool native;

  // these are needed in every ey or jdi will self destruct
  std::string defines_cmd;
  std::string searchdirs_cmd;
  std::string searchdirs_start;
  std::string searchdirs_end;

  std::map<std::string, std::string> make_vars;
  std::map<std::string, std::string> exe_vars;

  std::string MAKE_location;
};

extern CompilerInfo compilerInfo;
bool load_compiler_ey(std::string fPath);

extern bool codegen_only;
extern std::filesystem::path enigma_root;
extern std::filesystem::path eobjs_directory;
extern std::filesystem::path codegen_directory;

inline std::string escapeEnv(std::string str) {
  size_t i = str.find_first_of('%');
  while (i != std::string::npos) {
    size_t j = str.find_first_of('%', i + 1);
    if (j == std::string::npos) {
      std::cerr << "Unmatched %s in " << str << std::endl;
      break;
    }
    char* val = getenv(str.substr(i + 1, j - i - 1).c_str());
    std::string repl = val == nullptr ? "" : val;
    str.replace(i, j - i + 1, repl);
    i = str.find_first_of('%');
  }
  
  return str;
}

inline std::string unixfy_path(const std::filesystem::path& path) {
  return string_replace_all(path.u8string(), "\\", "/");
}

#endif
