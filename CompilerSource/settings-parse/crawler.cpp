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

#include <iostream>
#include <string>
#include <list>

using namespace std;

#include "filesystem/file_find.h"

#include "crawler.h"
#include "utility.h"

#include "eyaml/eyaml.h"

namespace settings
{
  list<string> systems;
  
  int scour_settings()
  {
    string bdir = "ENIGMAsystem/SHELL/";
    for (string mdir = file_find_first("ENIGMAsystem/SHELL/*",fa_sysfile | fa_readonly | fa_directory | fa_nofiles); mdir != ""; mdir = file_find_next())
    {
      FILE *module_descriptor = fopen((bdir+mdir).c_str(),"rt");
      if (module_descriptor)
      {
        
      }
    }
    return 0;
  }
}

#include "parser/object_storage.h"
#include "OS_Switchboard.h"
#include "languages/language_adapter.h"

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
      
      ifstream iey(("ENIGMAsystem/SHELL/" + exts[i]+"/About.ey").c_str());
      if (!iey.is_open())
        cout << "ERROR! Failed to open extension descriptor for " << exts[i] << endl;
      ey_data about = parse_eyaml(iey,exts[i]);
      pe.implements = about.get("implement");
      pe.init = about.get("init");

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
    for (string ef = file_find_first("ENIGMAsystem/SHELL/Platforms/*",fa_sysfile | fa_readonly | fa_directory | fa_nofiles); ef != ""; ef = file_find_next())
    {
      const string ef_path = "ENIGMAsystem/SHELL/Platforms/" + ef + "/Info/About.ey";
      ifstream ext(ef_path.c_str(), ios_base::in);
      if (!ext.is_open()) continue;

      cout << " - " << ef_path << ": Opened.\n";
      ey_data dat = parse_eyaml(ext,ef);
      eyit hasname = dat.values.find("represents");
      if (hasname == dat.values.end()) {
        cout << "Skipping invalid platform API under `" << ef << "': File does not specify an OS it represents.";
        continue;
      }
      
      sdk_descriptor& sdk = all_platforms[toUpper(ef)];
      sdk.name   = dat.get("name");
      sdk.author = dat.get("author");
      sdk.build_platforms = dat.get("build-platforms");
      sdk.description = dat.get("description");
      sdk.identifier  = dat.get("identifier");
      sdk.represents  = dat.get("represents");
    }
    file_find_close();
    
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
