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
#include "../OS_Switchboard.h"
#include "crawler.h"
#include "eyaml.h"

void parse_ide_settings(const char* eyaml)
{
  ey_data settree = parse_eyaml_str(eyaml);
  
  // Get target platform
  eyit it = settree.getit("target-windowing");
  if (it == settree.itend()) {
     cout << "ERROR! IDE has not specified a target windowing API!" << endl;
     extensions::targetAPI.windowSys = "";
  } else  extensions::targetAPI.windowSys = eyit_str(it);
  
  // Get best graphics system
  it = settree.getit("target-graphics");
  if (it == settree.itend()) {
     cout << "ERROR! IDE has not specified a graphics system!" << endl;
     extensions::targetAPI.graphicsSys = "";
  } else extensions::targetAPI.graphicsSys = eyit_str(it);
  
}

