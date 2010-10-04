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
#include <fstream>
#include <string>
#include <list>

using namespace std;

#include "../filesystem/file_find.h"
#include "../OS_Switchboard.h"
#include "crawler.h"
#include "eyaml.h"

inline string tolower(string x) {
  for (size_t i = 0; i < x.length(); i++)
    if (x[i] >= 'A' and x[i] <= 'Z') x[i] -= 'A' - 'a';
  return x;
}

void parse_ide_settings(const char* eyaml)
{
  ey_data settree = parse_eyaml_str(eyaml);
  eyit it;
  
  #define ey_cp(v,x,y) \
  it = settree.getit("target-" #x); \
  if (it == settree.itend()) { \
     cout << "ERROR! IDE has not specified a target " #x " " #y "!" << endl; \
     extensions::targetAPI.v ## Sys = ""; \
  } else  extensions::targetAPI.v ## Sys = eyit_str(it);
  
  // Get target's windowing api
  ey_cp(window,   windowing,api)
  // Get requested graphics system
  ey_cp(graphics,  graphics,system)
  // Get requested audio system
  ey_cp(audio,     audio,system)
  // Get requested collision system
  ey_cp(collision, collision,system)
  // Get requested networking library
  ey_cp(network,   networking,library)
  
  ifstream ifs; string eyname;
  ifs.open((eyname = "ENIGMAsystem/SHELL/Platforms/" + extensions::targetAPI.windowSys + "/About.ey").c_str());
  if (ifs.is_open()) { ey_data l = parse_eyaml(ifs, eyname.c_str());
    it = l.getit("links"); if (it != l.itend()) extensions::targetAPI.windowLinks = eyit_str(it);
    ifs.close();
  }
  string platn = tolower(extensions::targetAPI.windowSys);
  #define eygl(fn,v) \
  ifs.open((eyname = "ENIGMAsystem/SHELL/" #fn "/" + extensions::targetAPI.v ## Sys + "/Config/" + platn + ".ey").c_str()); \
  if (ifs.is_open()) { ey_data l = parse_eyaml(ifs, eyname.c_str()); cout << "Opened " << eyname << endl; \
    it = l.getit("links"); if (it != l.itend()) extensions::targetAPI.v ## Links = eyit_str(it); else cout << "Links not named in " << eyname << endl; ifs.close(); \
  } else cout << "Could not open " << eyname << ".\n";
  eygl(Graphics_Systems, graphics);
  eygl(Audio_Systems, audio);
}

