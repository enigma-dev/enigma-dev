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
#include "../../backend/ideprint.h"

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../../parser/object_storage.h"
#include "../compile_common.h"
#include "../event_reader/event_parser.h"

#include <math.h> //log2 to calculate passes.

int compile_parseAndLink(EnigmaStruct *es,parsed_script *scripts[])
{
  //First we just parse the scripts to add semicolons and collect variable names
  for (int i = 0; i < es->scriptCount; i++)
  {
    int a = syncheck::syntacheck(es->scripts[i].code);
    if (a != -1) {
      user << "Syntax error in script `" << es->scripts[i].name << "'\n" << syncheck::syerr << flushl;
      return E_ERROR_SYNTAX;
    }
    // Keep a parsed record of this script
    scr_lookup[es->scripts[i].name] = scripts[i] = new parsed_script;
    parser_main(es->scripts[i].code,&scripts[i]->pev);
    edbg << "Parsed `" << es->scripts[i].name << "': " << scripts[i]->obj.locals.size() << " locals, " << scripts[i]->obj.globals.size() << " globals" << flushl;
    
    // If the script accesses variables from outside its scope implicitly
    if (scripts[i]->obj.locals.size() or scripts[i]->obj.globallocals.size()) {
      parsed_object temporary_object = *scripts[i]->pev.myObj;
      scripts[i]->pev_global = new parsed_event(&temporary_object);
      parser_main(string("with (self) {\n") + es->scripts[i].code + "\n/* */}",scripts[i]->pev_global);
      scripts[i]->pev_global->myObj = NULL;
    }
    fflush(stdout);
  }
  
  edbg << "\"Linking\" scripts" << flushl;
  
  //Next we traverse the scripts for dependencies.
  unsigned nec_iters = 0;
  if (es->scriptCount > 0)
    nec_iters = lrint(ceilf(log2(es->scriptCount)));
  edbg << "`Linking' " << es->scriptCount << " scripts in " << nec_iters << " passes...\n";
  for (unsigned _necit = 0; _necit < nec_iters; _necit++) //We will iterate the list of scripts just enough times to copy everything
  {
    for (int _im = 0; _im < es->scriptCount; _im++) //For each script
    {
      parsed_script* curscript = scripts[_im]; //At this point, what we have is this:     for each script as curscript
      for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) //For each function called by each script
      {
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end()) //At this point, what we have is this:     for each script called by curscript
          curscript->obj.copy_calls_from(subscr->second->obj);
      }
    }
  }
  
  edbg << "Completing script \"Link\"" << flushl;
  
  for (int _im = 0; _im < es->scriptCount; _im++) //For each script
  {
    string curscrname = es->scripts[_im].name;
    parsed_script* curscript = scripts[_im]; //At this point, what we have is this:     for each script as curscript
    edbg << "Linking `" << curscrname << "':\n";
    for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) //For each function called by each script
    {
      cout << string(it->first) << "::";
      map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
      if (subscr != scr_lookup.end()) //At this point, what we have is this:     for each script called by curscript
      {
        cout << "is_script::";
        curscript->obj.copy_from(subscr->second->obj,  "script `"+it->first+"'",  "script `"+curscrname + "'");
      }
    }
    cout << endl;
  }
  edbg << "Done." << flushl;
  
  
  edbg << es->gmObjectCount << " Objects:\n";
  for (int i = 0; i < es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    parsed_object* pob = parsed_objects[es->gmObjects[i].id] = 
      new parsed_object(
        es->gmObjects[i].name, es->gmObjects[i].id, es->gmObjects[i].spriteId,
        es->gmObjects[i].parentId,
        es->gmObjects[i].visible, es->gmObjects[i].solid,
        es->gmObjects[i].depth
      );
    
    edbg << " " << es->gmObjects[i].name << ": " << es->gmObjects[i].mainEventCount << " events: " << flushl;
    
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
    if (es->gmObjects[i].mainEvents[ii].eventCount) //For every MainEvent that contains event code
    {
      //For each main event in that object, make a copy
      const int mev_id = es->gmObjects[i].mainEvents[ii].id;
      edbg << "  Event[" << es->gmObjects[i].mainEvents[ii].id << "]: ";
      
      edbg << "  Parsing " << es->gmObjects[i].mainEvents[ii].eventCount << " sub-events:" << flushl;
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        //For each individual event (like begin_step) in the main event (Step), parse the code
        const int sev_id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        parsed_event &pev = pob->events[ev_count++]; //Make sure each sub event knows its main event's event ID.
        pev.mainId = mev_id, pev.id = sev_id;
        
        //Copy the code into a string, and its attributes elsewhere
        string code = es->gmObjects[i].mainEvents[ii].events[iii].code;
        
        //Syntax check the code
        
        // Print debug info
          edbg << "Check `" << es->gmObjects[i].name << "::" << event_get_function_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id) << "...";
        
        // Check the code
        int sc = syncheck::syntacheck(code);
        if (sc != -1)
        {
          // Error. Report it.
          user << "Syntax error in object `" << es->gmObjects[i].name << "', " << event_get_human_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id) << " event:"
               << es->gmObjects[i].mainEvents[ii].events[iii].id << ":\n" << format_error(code,syncheck::syerr,sc) << flushl;
          return E_ERROR_SYNTAX;
        }
        
        edbg << "Done. Starting parse...";
        
        //Add this to our objects map
        pev.myObj = pob; //Link to its calling object.
        parser_main(code,&pev); //Format it to C++
        
        edbg << "Done." << flushl;
      }
    }
  }
  
  //Next we link the scripts into the objects.
  edbg << "\"Linking\" scripts into the objects..." << flushl;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
  {
    parsed_object* t = i->second;
    for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by each script
    {
      map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
      if (subscr != scr_lookup.end()) //If we've got ourselves a script
        t->copy_calls_from(subscr->second->obj);
    }
    for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by each script
    {
      map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
      if (subscr != scr_lookup.end()) //If we've got ourselves a script
        t->copy_from(subscr->second->obj,  "script `"+it->first+"'",  "object `"+i->second->name+"'");
    }
  }
  edbg << "\"Link\" complete." << flushl;
  
  // Sort through object calls finding max script arg counts
  edbg << "Tabulating maximum argument passes to each script" << flushl;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    for (parsed_object::funcit it = i->second->funcs.begin(); it != i->second->funcs.end(); it++) //For each function called by this object
  {
    map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
    if (subscr != scr_lookup.end() and subscr->second->globargs < it->second)
      subscr->second->globargs = it->second,
      edbg << "  Object `" << i->second->name << "' calls " << it->first << " with " << it->second << " parameters." << flushl;
  }
  edbg << "Finished" << flushl;
  
  return 0;
}


int link_globals(parsed_object *global, EnigmaStruct *es,parsed_script *scripts[])
{
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    global->copy_from(*i->second,"object `"+i->second->name+"'","the Global Scope");
  for (int i = 0; i < es->scriptCount; i++)
    global->copy_from(scripts[i]->obj,"script `"+scripts[i]->obj.name+"'","the Global Scope");
  return 0;
}
