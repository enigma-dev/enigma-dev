/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008 Josh Ventura                                             **
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
#include "crawler.h"
#include "utility.h"

#include "parser/object_storage.h"
#include "OS_Switchboard.h"
#include "languages/language_adapter.h"

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <string>
#include <list>

using namespace std;

namespace extensions
{
  static inline string unmangled_type_pre(string str) {
    size_t pm = str.find_first_of(")[");
    return pm == string::npos ? str : str.substr(0,pm);
  }
  static inline string unmangled_type_suf(string str) {
    size_t pm = str.find_first_of(")[");
    return pm == string::npos ? "" : str.substr(pm);
  }
  void parse_extensions(vector<string> exts) {
    parsed_extensions.clear();

    for (unsigned i = 0; i < exts.size(); i++)
    {
      parsed_extension pe;
      pe.pathname = exts[i];
      size_t pos = exts[i].find_last_of("/\\");
      pe.name = pos == string::npos ? exts[i] : exts[i].substr(pos + 1);
      pe.path = pos == string::npos ? ""      : exts[i].substr(0, pos + 1);
      for (pos = 0; pos < pe.path.length(); pos++)
        if (pe.path[pos] == '\\')
          pe.path[i] = '/';

      YAML::Node iey = YAML::LoadFile(enigma_root/"ENIGMAsystem/SHELL"/exts[i]/"About.ey");
      if (!iey.IsDefined())
        cout << "ERROR! Failed to open extension descriptor for " << exts[i] << endl;

      pe.implements = iey["implement"].as<std::string>("");
      pe.init = iey["init"].as<std::string>("");
      parsed_extensions.push_back(pe);
    }
  }

  map<string, sdk_descriptor> all_platforms;
  typedef map<string, sdk_descriptor>::iterator platIter;

  string uname_s = CURRENT_PLATFORM_NAME;

  void determine_target()
  {
    all_platforms.clear();
    cout << "\n\n\n\nStarting platform inspection\n";
    for (auto& ef : std::filesystem::directory_iterator(enigma_root/"ENIGMAsystem/SHELL/Platforms/"))
    {
      if (!ef.is_directory() || ef.path().stem() == "General") continue;
      const std::filesystem::path ef_path = ef.path()/"Info/About.ey";
      const YAML::Node ext = YAML::LoadFile(ef_path);
      if (!ext.IsDefined()) continue;

      cout << " - " << ef_path << ": Opened.\n";
      if (!ext["represents"].IsDefined()) {
        cout << "Skipping invalid platform API under `" << ef.path().u8string() << "': File does not specify an OS it represents.";
        continue;
      }

      sdk_descriptor& sdk = all_platforms[toUpper(ef.path().u8string())];
      sdk.name = ext["name"].as<std::string>();
      sdk.author = ext["author"].as<std::string>();
      if (ext["depends"].IsDefined())
        sdk.depends = ext["depends"]["build-platforms"].as<std::string>("");
      sdk.represents = ext["represents"]["build-platforms"].as<std::string>();
      sdk.description = ext["description"].as<std::string>();
      sdk.identifier  = ext["identifier"].as<std::string>();
    }

    if (targetAPI.windowSys != "")
      targetSDK = all_platforms[toUpper(targetAPI.windowSys)];
    else
    {
      string iun = toUpper(uname_s);
      for (platIter i = all_platforms.begin(); i != all_platforms.end(); i++)
        if (toUpper(i->second.represents) == iun)
      {
        targetSDK = i->second;
        break;
      }
    }
  }
}
