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

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../compile_common.h"

#include "../event_reader/event_parser.h"

struct foundevent { int mid, id, count; foundevent(): mid(0),id(0),count(0) {} void operator++(int) { count++; } };

int compile_writeDefraggedEvents(EnigmaStruct* es)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_evparent.h");
  wto << license;
  
  map<string,foundevent> used_events;
  for (int i = 0; i < es->gmObjectCount; i++)
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
        used_events[event_get_function_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id)]++;
  
  wto << "struct event_parent: " << system_get_uppermost_tier() << endl;
  wto << "{" << endl;
  for (map<string,foundevent>::iterator it = used_events.begin(); it != used_events.end(); it++)
  {
    wto << "  virtual enigma::variant myevent_" << it->first << "()";
    if (event_has_default_code(it->second.mid,it->second.id))
      wto << endl << "  {" << endl << "  " << event_get_default_code(it->second.mid,it->second.id) << endl << "  }" << endl;
    else wto << " {}" << endl;
  }
  wto << "};" << endl;
  
  return 0;
}
