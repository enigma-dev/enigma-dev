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

//Sometimes I wonder if licenses waste space. *Whistles*
#ifndef GCC_BACKEND_H
#define GCC_BACKEND_H
#include "general/parse_basics_old.h"

extern string MAKE_flags, MAKE_paths, MAKE_tcpaths, MAKE_location, TOPLEVEL_cflags, TOPLEVEL_cppflags, TOPLEVEL_cxxflags, TOPLEVEL_links, TOPLEVEL_rcflags, CXX_override, CC_override, WINDRES_location, TOPLEVEL_ldflags;

inline string fc(const char* fn)
{
    FILE *pt = fopen(fn,"rb");
    if (pt==NULL) return "";
    else {
      fseek(pt,0,SEEK_END);
      size_t sz = ftell(pt);
      fseek(pt,0,SEEK_SET);

      char a[sz+1];
      sz = fread(a,1,sz,pt);
      fclose(pt);

      a[sz] = 0;
      return a;
    }
}

inline int rdir_system(string x, string y)
{
  return system((x + " " + y).c_str());
}

static inline vector<string> explode(string n) {
  vector<string> ret;
  size_t pos = 0, epos;
  while (is_useless(n[pos])) pos++;
  for (epos = n.find(','); epos != string::npos; epos = n.find(',',pos)) {
    ret.push_back(n.substr(pos,epos-pos));
    pos = epos; while (is_useless(n[++pos]));
  }
  if (n.length() > pos)
    ret.push_back(n.substr(pos));
  return ret;
}

inline string tolower(string x) {
  for (size_t i = 0; i < x.length(); i++)
    if (x[i] >= 'A' and x[i] <= 'Z') x[i] -= 'A' - 'a';
  return x;
}


#endif