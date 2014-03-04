/**
  @file  parse_and_link.cpp
  @brief Declares the all-important part of the compiler that iterates through
         everything, parsing it.
  
  @section License
    Copyright (C) 2008-2014 Josh Ventura
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

#include <stdio.h>
#include <iostream>
#include "backend/ideprint.h"

using namespace std;


#include "syntax/syncheck.h"
#include "parser/parser.h"

#include "backend/EnigmaStruct.h" //LateralGM interface structures
#include "parser/object_storage.h"
#include "compiler/compile_common.h"
#include "compiler/event_reader/event_parser.h"

#include <math.h> //log2 to calculate passes.

#include <languages/lang_CPP.h>
#include <General/parse_basics.h>

#include "compiler/compile_includes.h"

inline string safe_name(string name) {
  bool good_name = true;
  if (!is_letter(name[0]))
    good_name = false;
  else for (size_t i = 1; i < name.length(); ++i)
    if (!is_letterd(name[i])) { good_name = false; break; }
  if (good_name) return name;
  name = "ENC_" + name;
  for (size_t i = 4; i < name.length(); ++i)
    if (!is_letterd(name[i]))
      name.replace(i,1,"_C" + tostring(int(name[i])) + "_");
  return name;
}

int compile_parseAndLink(compile_context &ctex)
{
  //First we just parse the scripts to add semicolons and collect variable names
  for (int i = 0; i < ctex.es->scriptCount; i++)
  {
    // Keep a record of this script
    parsed_script *cur_scr = ctex.parsed_scripts[ctex.es->scripts[i].name] = new parsed_script(main_context, ctex.es->scripts + i, ctex.global);
    if (!cur_scr->parse())
      return 1;
    edbg << "Parsed `" << ctex.es->scripts[i].name << "': " << cur_scr->locals.members.size() << " locals" << flushl;
    fflush(stdout);
  }
  
  edbg << "\"Linking\" scripts" << flushl;
  
  //Next we traverse the scripts for dependencies.
  unsigned nec_iters = 0;
  if (ctex.es->scriptCount > 0)
    nec_iters = lrint(ceilf(log2(ctex.es->scriptCount)));
  edbg << "`Linking' " << ctex.es->scriptCount << " scripts in " << nec_iters << " passes...\n";
  for (unsigned necit = 0; necit < nec_iters; necit++) //We will iterate the list of scripts just enough times to copy everything
  {
    for (ps_i it = ctex.parsed_scripts.begin(); it != ctex.parsed_scripts.end(); ++it) //For each script
    {
      parsed_script* curscript = it->second; //At this point, what we have is this:     for each script as curscript
      for (script_it subscr = curscript->sh.scripts.begin(); subscr != curscript->sh.scripts.end(); ++subscr) //For each function called by each script
        curscript->sh.copy_scripts_from(subscr->second.script->sh);
    }
  }
  
  edbg << "Completing script \"Link\"" << flushl;
  
  for (ps_i psit = ctex.parsed_scripts.begin(); psit != ctex.parsed_scripts.end(); ++psit) //For each script
  {
    string curscrname = psit->first;
    parsed_script* curscript = psit->second; //At this point, what we have is this:     for each script as curscript
    edbg << "Linking `" << curscrname << "':\n";
    for (script_it scrit = curscript->sh.scripts.begin(); scrit != curscript->sh.scripts.end(); ++scrit) //For each script called by each script
      curscript->sh.copy_from(scrit->second.script->sh,  "script `" + scrit->first + "'",  "script `" + curscrname + "'");
    cout << endl;
  }
  edbg << "Done." << flushl;


  edbg << ctex.es->gmObjectCount << " Objects:\n";
  for (int i = 0; i < ctex.es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    parsed_object* p_obj = ctex.parsed_objects[ctex.es->gmObjects[i].id] =
      new parsed_object(safe_name(ctex.es->gmObjects[i].name), &ctex.es->gmObjects[i]);

    edbg << " " << ctex.es->gmObjects[i].name << ": " << ctex.es->gmObjects[i].mainEventCount << " events: " << flushl;

    for (int ii = 0; ii < ctex.es->gmObjects[i].mainEventCount; ii++)
    if (ctex.es->gmObjects[i].mainEvents[ii].eventCount) //For every MainEvent that contains event code
    {
      //For each main event in that object, make a copy
      const int mev_id = ctex.es->gmObjects[i].mainEvents[ii].id;
      edbg << "  Parsing " << ctex.es->gmObjects[i].mainEvents[ii].eventCount << " sub-events:" << flushl;
      for (int iii = 0; iii < ctex.es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        //For each individual event (like begin_step) in the main event (Step), parse the code
        const int ev_id = ctex.es->gmObjects[i].mainEvents[ii].events[iii].id;
        parsed_event *p_ev = new parsed_event(main_context, mev_id, ev_id, p_obj, ctex.global, ctex.es->gmObjects[i].mainEvents[ii].events[iii].code);
        p_obj->events.push_back(p_ev); // Make sure each sub event knows its main event's event ID.
        
        // Parse the code
        int pres = p_ev->parse();
        if (pres) return 1;
        
        edbg << " Done." << flushl;
      }
    }
  }
  
  //Now we parse the rooms
  edbg << "Creating room creation code scope and parsing" << flushl;
  for (int i = 0; i < ctex.es->roomCount; i++)
  {
    parsed_room *p_room = ctex.parsed_rooms[ctex.es->rooms[i].id] = new parsed_room(ctex.es->rooms + i);
    if (ctex.es->rooms[i].creationCode and *ctex.es->rooms[i].creationCode) {
      p_room->creation_code = new parsed_code(main_context, NULL, ctex.global);
      int pres = p_room->creation_code->parse(ctex.es->rooms[i].creationCode);
      if (!pres) return E_ERROR_SYNTAX;
    }
    
    for (int ii = 0; ii < ctex.es->rooms[i].instanceCount; ii++)
    {
      const char *const ccode = ctex.es->rooms[i].instances[ii].creationCode;
      if (ccode and *ccode)
      {
        // TODO: NEWCOMPILER: Make sure this is wrapped in with(). Parse room creation code, parse instance creation codes.
        parsed_room::parsed_icreatecode *picc = p_room->instance_create_codes[ctex.es->rooms[i].instances[ii].id]
          = new parsed_room::parsed_icreatecode(main_context, ctex.parsed_objects[ctex.es->rooms[i].instances[ii].objectId], ctex.global);
        int pres = picc->parse(ccode);
        if (pres) return E_ERROR_SYNTAX;
      }
    }
  }
  
  //Next we link the scripts into the objects.
  edbg << "\"Linking\" scripts into the objects..." << flushl;
  for (po_i it = ctex.parsed_objects.begin(); it != ctex.parsed_objects.end(); ++it)
  {
    parsed_object* p_obj = it->second;
    for (script_it subscr = p_obj->sh.scripts.begin(); subscr != p_obj->sh.scripts.end(); ++subscr) //For each script called by each script
      p_obj->sh.copy_from(subscr->second.script->sh,  "script `" + subscr->first + "'",  "object `" + string(it->second->properties->name) + "'");
  }
  edbg << "\"Link\" complete." << flushl;
  
  // Sort through object calls finding max script arg countsae
  edbg << "Tabulating maximum numbers of arguments passed to each script" << flushl;
  for (po_i it = ctex.parsed_objects.begin(); it != ctex.parsed_objects.end(); ++it)
    for (script_it subscr = it->second->sh.scripts.begin(); subscr != it->second->sh.scripts.end(); ++subscr) //For each script called by this object
      if (subscr->second.script->global_args < subscr->second.arg_max)
        subscr->second.script->global_args = subscr->second.arg_max;
  edbg << "Finished" << flushl;
  return 0;
}
