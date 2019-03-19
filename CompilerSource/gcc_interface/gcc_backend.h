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

#include <string>
#include <vector>
#include <map>
#include <sstream> // std::stringstream
#include <fstream> // std::ifstream
#include <iostream> // std::cerr

inline std::string fc(const char* fn) {
  std::ifstream t(fn);
  
  if (!t.is_open()) {
    std::cerr << "Failed to open " << fn << std::endl;
    return "";
  }
  
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

inline int rdir_system(std::string x, std::string y) { return system((x + " " + y).c_str()); }

static inline std::vector<std::string> explode(std::string n) {
  std::vector<std::string> ret;
  size_t pos = 0, epos;
  while (is_useless(n[pos])) pos++;
  for (epos = n.find(','); epos != std::string::npos; epos = n.find(',', pos)) {
    ret.push_back(n.substr(pos, epos - pos));
    pos = epos;
    while (is_useless(n[++pos]))
      ;
  }
  if (n.length() > pos) ret.push_back(n.substr(pos));
  return ret;
}

inline std::string tolower(std::string x) {
  for (size_t i = 0; i < x.length(); i++)
    if (x[i] >= 'A' and x[i] <= 'Z') x[i] -= 'A' - 'a';
  return x;
}

#endif
