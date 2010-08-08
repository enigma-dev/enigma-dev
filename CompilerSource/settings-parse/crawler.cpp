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

#include "../filesystem/file_find.h"
#include "crawler.h"
#include "eyaml.h"

namespace settings
{
  list<string> systems;
  
  int scour_settings()
  {
    string bdir = "ENIGMAsystem/SHELL/";
    for (string mdir = file_find_first("ENIGMAsystem/SHELL/*",fa_directory | fa_nofiles); mdir != ""; mdir = file_find_next())
    {
      FILE *module_descriptor = fopen((bdir+mdir).c_str(),"rt");
      if (module_descriptor)
      {
        
      }
    }
    return 0;
  }
}

#include "../parser/object_storage.h"

namespace extensions
{
  map<string, string> locals;
  string unmangled_type_pre(string str) { 
    size_t pm = str.find_first_of(")[");
    return pm == string::npos ? str : str.substr(0,pm);
  }
  string unmangled_type_suf(string str) {
    size_t pm = str.find_first_of(")[");
    return pm == string::npos ? "" : str.substr(pm);
  }
  string compile_local_string()
  {
    string res;
    for (map<string, string>::iterator it = locals.begin(); it != locals.end(); it++)
      res += unmangled_type_pre(it->second) + " " + it->first + unmangled_type_suf(it->second) + ";\n";
    return res;
  }
  void dump_read_locals(map<string,int> &lmap)
  {
    for (map<string,string>::iterator it = locals.begin(); it != locals.end(); it++)
      lmap[it->first]++;
  }
  void crawl_for_locals()
  {
    locals.clear();
    for (string ef = file_find_first("ENIGMAsystem/Extensions/*",0); ef != ""; ef = file_find_next())
    {
      ifstream ext(("ENIGMAsystem/Extensions/" + ef).c_str(), ios_base::in);
      if (ext.is_open())
      {
        ey_data dat = parse_eyaml(ext, ef);
        for (eyit it = dat.values.begin(); it != dat.values.end(); it++)
        {
          if (it->second->is_scalar)
            continue;
          
          eyit locs = ((ey_data*)it->second)->values.find("locals");
          if (locs != ((ey_data*)it->second)->values.end())
          {
            if (locs->second->is_scalar)
              continue;
            ey_data *ld = (ey_data*)locs->second;
            for (eycit cit = ld->values_order.next; cit; cit = cit->next)
              locals[cit->value->name] = ((ey_string*)cit->value)->value;
          }
        }
      }
    }
  }
}
