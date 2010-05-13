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

using namespace std;

#include "../../externs/externs.h"
#include "../../syntax/syncheck.h"
#include "../../parser/parser.h"

#include "../../backend/EnigmaStruct.h" //LateralGM interface structures
#include "../../parser/object_storage.h"
#include "../compile_common.h"

#include <math.h> //log2 to calculate passes.

#define flushl (fflush(stdout), "\n")
#define flushs (fflush(stdout), " ")

int compile_parseAndLink(EnigmaStruct *es,parsed_script *scripts[])
{
  //First we just parse the scripts to add semicolons and collect variable names
  for (int i = 0; i < es->scriptCount; i++)
  {
    int a = syncheck::syntacheck(es->scripts[i].code);
    if (a != -1) {
      cout << "Syntax error in script `" << es->scripts[i].name << "'" << endl << syncheck::error << flushl;
      return E_ERROR_SYNTAX;
    }
    scr_lookup[es->scripts[i].name] = scripts[i] = new parsed_script;
    parser_main(es->scripts[i].code,&scripts[i]->pev);
    cout << "Parsed `" << es->scripts[i].name << "': " << scripts[i]->obj.locals.size() << " locals, " << scripts[i]->obj.globals.size() << " globals" << flushl;
    fflush(stdout);
  }
  
  cout << "\"Linking\" scripts" << flushl;
  
  //Next we traverse the scripts for dependencies.
  unsigned nec_iters = 0;
  if (es->scriptCount > 0)
    nec_iters = lrint(ceilf(log2(es->scriptCount)));
  cout << "`Linking' " << es->scriptCount << " scripts in " << nec_iters << " passes...\n";
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
  
  cout << "Completing script \"Link\"" << flushl;
  
  for (int _im = 0; _im < es->scriptCount; _im++) //For each script
  {
    string curscrname = es->scripts[_im].name;
    parsed_script* curscript = scripts[_im]; //At this point, what we have is this:     for each script as curscript
    cout << "Linking `" << curscrname << "':\n";
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
  cout << "Done." << flushl;
  
  
  cout << es->gmObjectCount << " Objects:" << endl;
  for (int i = 0; i < es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    parsed_object* pob = parsed_objects[es->gmObjects[i].id] = new parsed_object(es->gmObjects[i].name,es->gmObjects[i].id,es->gmObjects[i].spriteId);
    
    cout << " " << es->gmObjects[i].name << ": " << es->gmObjects[i].mainEventCount << " events: " << flushl;
    
    for (int ii = 0; ii < es->gmObjects[i].mainEventCount; ii++)
    if (es->gmObjects[i].mainEvents[ii].eventCount) //For every MainEvent that contains event code
    {
      //For each main event in that object, make a copy
      const int mev_id = es->gmObjects[i].mainEvents[ii].id;
      cout << "  Event[" << es->gmObjects[i].mainEvents[ii].id << "]: ";
      
      cout << "  Parsing " << es->gmObjects[i].mainEvents[ii].eventCount << " sub-events:" << flushl;
      for (int iii = 0; iii < es->gmObjects[i].mainEvents[ii].eventCount; iii++)
      {
        //For each individual event (like begin_step) in the main event (Step), parse the code
        const int sev_id = es->gmObjects[i].mainEvents[ii].events[iii].id;
        parsed_event &pev = pob->events[ev_count++]; //Make sure each sub event knows its main event's event ID.
        pev.mainId = mev_id, pev.id = sev_id;
        
        //Copy the code into a string, and its attributes elsewhere
        string code = es->gmObjects[i].mainEvents[ii].events[iii].code;
        
        //Syntax check the code
        cout << "Check `" << es->gmObjects[i].name << "::" << event_get_enigma_main_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id) << "..." << flushs;
        int sc = syncheck::syntacheck(code);
        if (sc != -1)
        {
          cout << "Syntax error in object `" << es->gmObjects[i].name << "', event " << mev_id << ":"
               << es->gmObjects[i].mainEvents[ii].events[iii].id << ":\n" << format_error(code,syncheck::error,sc) << flushl;
          return E_ERROR_SYNTAX;
        }
        cout << "Done. Starting parse..." << flushs;
        
        //Add this to our objects map
        pev.myObj = pob; //link to its parent
        parser_main(code,&pev);
        
        cout << "Done." << flushl;
      }
    }
    fflush(stdout);
  }
  
  //Next we link the scripts into the objects.
  cout << "\"Linking\" scripts into the objects..." << flushl;
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
  cout << "\"Link\" complete." << flushl;
  return 0;
}
