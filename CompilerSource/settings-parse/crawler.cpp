/**
  @file  crawler.cpp
  @brief Implements functions for extension descriptor file iteration.
  
  This file lays out the framework for reading extensions from the host language.
  The host language provides the dir(s) to crawl for extensions, and also the method
  to finish parsing and loading the extensions once they have been read in.
  
  Members of this file simply load the skeletons into memory.
  
  @section License
    Copyright (C) 2008-2012 Josh Ventura
    This file is a part of the ENIGMA Development Environment.

    ENIGMA is free software: you can redistribute it and/or modify it under the
    terms of the GNU General Public License as published by the Free Software
    Foundation, version 3 of the license or any later version.

    This application and its source code is distributed AS-IS, WITHOUT ANY WARRANTY; 
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have recieved a copy of the GNU General Public License along
    with this code. If not, see <http://www.gnu.org/licenses/>
**/

#include <iostream>
#include <string>
#include <list>

using namespace std;

#include "filesystem/file_find.h"

#include "crawler.h"
#include "eyaml.h"

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
#include <general/estring.h>

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
  void parse_extensions(language_adapter *lang)
  {
    lang->parsed_extensions.clear();
    for (unsigned i = 0; i < lang->requested_extensions.size(); i++)
    {
      parsed_extension pe;
      pe.pathname = lang->requested_extensions[i];
      size_t pos = lang->requested_extensions[i].find_last_of("/\\");
      pe.name = pos == string::npos ? lang->requested_extensions[i] : lang->requested_extensions[i].substr(pos + 1);
      pe.path = pos == string::npos ? "" : lang->requested_extensions[i].substr(0, pos + 1);
      for (pos = 0; pos < pe.path.length(); pos++)
        if (pe.path[pos] == '\\')
          pe.path[i] = '/';
      
      ifstream iey(("ENIGMAsystem/SHELL/" + lang->requested_extensions[i]+"/About.ey").c_str());
      if (!iey.is_open())
        cout << "ERROR! Failed to open extension descriptor for " << lang->requested_extensions[i] << endl;
      ey_data about = parse_eyaml(iey,lang->requested_extensions[i]);
      pe.implements = about.get("implement");
      
      lang->parsed_extensions.push_back(pe);
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
      cout << " - ENIGMAsystem/SHELL/Platforms/" + ef + "/Info/About.ey: ";
      ifstream ext(("ENIGMAsystem/SHELL/Platforms/" + ef + "/Info/About.ey").c_str(), ios_base::in);
      if (ext.is_open())
      {
        cout << "Opened.\n";
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
      } else cout << "Failed!\n";
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
