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

#include "../../backend/ideprint.h"

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../compile_common.h"

#include "../event_reader/event_parser.h"

struct foundevent { int mid, id, count; foundevent(): mid(0),id(0),count(0) {} void f2(int m,int i) { id = i, mid = m; } void inc(int m,int i) { mid=m,id=i,count++; } void operator++(int) { count++; } };
map<string,foundevent> used_events;
typedef map<string,foundevent>::iterator evfit;

int compile_writeDefraggedEvents(EnigmaStruct* es)
{
  ofstream wto("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_evparent.h");
  wto << license;
  
  used_events.clear();
  for (int i = 0; i < es->gmObjectCount; i++)
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        const int mid = es->gmObjects[i].mainEvents[ii].id, id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        if (event_is_instance(mid,id))
          used_events[event_stacked_get_root_name(mid)].inc(mid,id);
        else
          used_events[event_get_function_name(mid,id)].inc(mid,id);
      }
  
  for (size_t i=0; i<event_sequence.size(); i++)
  {
    const int mid = event_sequence[i].first, id = event_sequence[i].second;
    if (event_has_default_code(mid,id)) // We may not be using this event, but if it's persistent... (constant or defaulted: mandatory)
    { 
      used_events[event_get_function_name(mid,id)].f2(mid,id); // ...Reserve it anyway.
      
      for (po_i it = parsed_objects.begin(); it != parsed_objects.end(); it++) // Then shell it out into the other objects.
      {
        bool exists = false;
        for (unsigned i = 0; i < it->second->events.size; i++)
          if (it->second->events[i].mainId == mid and it->second->events[i].id == id)
            { exists = true; break; }
        if (!exists)
          it->second->events[it->second->events.size] = parsed_event(mid, id, it->second);
      }
    }
  }
  
  wto << "namespace enigma" << endl << "{" << endl;
  wto << "  struct event_parent: " << system_get_uppermost_tier() << endl;
  wto << "  {" << endl;
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
  {
    const bool eii = event_is_instance(it->second.mid, it->second.id);
    wto << (eii ? "    virtual void    myevent_" : "    virtual variant myevent_") << it->first << "()";
    edbg << "Checking for default code in ev[" << it->second.mid << ", " << it->second.id << ".\n";
    if (event_has_default_code(it->second.mid,it->second.id))
      wto << endl << "    {" << endl << "  " << event_get_default_code(it->second.mid,it->second.id) << endl << (eii ? "    }" : "    return 0;\n    }") << endl;
    else wto << (eii ? " { } // No default " : " { return 0; } // No default ") << event_get_human_name(it->second.mid,it->second.id) << " code." << endl;
  }
  wto << "    //virtual void unlink() {} // This is already declared at the super level." << endl;
  wto << "    event_parent() {}" << endl;
  wto << "    event_parent(unsigned x, int y): " << system_get_uppermost_tier() << "(x,y) {}" << endl;
  wto << "  };" << endl;
  wto << "}" << endl;
  wto.close();
  
  
  wto.open("ENIGMAsystem/SHELL/Preprocessor_Environment_Editable/IDE_EDIT_events.h");
  wto << license;
  wto << "namespace enigma" << endl << "{" << endl;
  
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto  << "  event_iter *event_" << it->first << "; // Defined in " << it->second.count << " objects" << endl;
  
  wto << "  int event_system_initialize()" << endl << "  {" << endl;
    wto  << "    events = new event_iter[" << used_events.size() << "]; // Allocated here; not really meant to change." << endl;
    int o_hiid = parsed_objects.rbegin() != parsed_objects.rend() ? parsed_objects.rbegin()->first : 1;
    wto  << "    objects = new objectid_base[" << (o_hiid+1) << "]; // Allocated here; not really meant to change." << endl;
    
    int ind = 0;
    for (evfit it = used_events.begin(); it != used_events.end(); it++)
      wto  << "    event_" << it->first << " = events + " << ind++ << ";  event_" << it->first << "->name = \"" << event_get_human_name(it->second.mid,it->second.id) << "\";" << endl;
    wto << "    return 0;" << endl;
  wto << "  }" << endl;
  
  for (evfit it = used_events.begin(); it != used_events.end(); it++)
    wto << event_get_super_check_function(it->second.mid, it->second.id);
  
  wto << "  int ENIGMA_events()" << endl << "  {" << endl;
    for (size_t i=0; i<event_sequence.size(); i++)
    {
      const int mid = event_sequence[i].first, id = event_sequence[i].second;
      evfit it = used_events.find(event_is_instance(mid,id) ? event_stacked_get_root_name(mid) : event_get_function_name(mid,id));
      if (it == used_events.end()) continue;
      
      if (event_has_instead(mid,id))
      {
        wto << "    " << event_get_instead(mid,id) << endl;
        wto << "    " << endl;
        wto << "    enigma::update_globals();" << endl;
        wto << "    " << endl;
      }
      else
        if (event_execution_uses_default(mid,id))
        {
          if (event_has_super_check(mid,id) and !event_is_instance(mid,id))
            wto << "    if (" << event_get_super_check_condition(mid,id) << ")" << endl
                << "      for (instance_event_iterator = event_" << it->first << "->next; instance_event_iterator != NULL; instance_event_iterator = instance_event_iterator->next)" << endl
                << "        ((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" << it->first << "();" << endl;
          else
            wto << "    for (instance_event_iterator = event_" << it->first << "->next; instance_event_iterator != NULL; instance_event_iterator = instance_event_iterator->next)" << endl
                << "      ((enigma::event_parent*)(instance_event_iterator->inst))->myevent_" << it->first << "();" << endl;
          wto << "    " << endl;
          wto << "    enigma::update_globals();" << endl;
          wto << "    " << endl;
        }
    }
    wto << "    enigma::dispose_destroyed_instances();" << endl;
    wto << "    enigma::sleep_for_framerate(room_speed);" << endl;
    wto << "    " << endl;
    wto << "    return 0;" << endl;
  wto << "  } // event function" << endl;
  
  // Done, end the namespace
  wto << "} // namespace enigma" << endl;
  wto.close();
  
  return 0;
}

bool event_used_by_something(string name)
{
  return used_events.find(name) != used_events.end();
}
