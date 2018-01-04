/********************************************************************************\
**                                                                              **
**  Copyright (C) 2008-2011 Josh Ventura                                        **
**  Copyright (C) 2014 Seth N. Hetu                                             **
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
#include <string>     // std::string, std::to_string (C++11)
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

#include "compiler/compile_includes.h"
#include "settings.h"

extern string tostring(int);

int lang_CPP::compile_parseAndLink(EnigmaStruct *es,parsed_script *scripts[], vector<parsed_script*>& tlines, const std::set<std::string>& script_names)
{
  //First we just parse the scripts to add semicolons and collect variable names
  for (int i = 0; i < es->scriptCount; i++)
  {
    std::string newcode;
    int a = syncheck::syntaxcheck(es->scripts[i].code, newcode);
    if (a != -1) {
      user << "Syntax error in script `" << es->scripts[i].name << "'\n" << format_error(es->scripts[i].code,syncheck::syerr,a) << flushl;
      return E_ERROR_SYNTAX;
    }
    // Keep a parsed record of this script
    scr_lookup[es->scripts[i].name] = scripts[i] = new parsed_script;
    parser_main(newcode,&scripts[i]->pev, script_names);
    edbg << "Parsed `" << es->scripts[i].name << "': " << (int)(scripts[i]->obj.locals.size()) << " locals, " << (int)scripts[i]->obj.globals.size() << " globals" << flushl;
    
    // If the script accesses variables from outside its scope implicitly
    if (scripts[i]->obj.locals.size() or scripts[i]->obj.globallocals.size() or scripts[i]->obj.ambiguous.size()) {
      parsed_object temporary_object = *scripts[i]->pev.myObj;
      scripts[i]->pev_global = new parsed_event(&temporary_object);
      parser_main(string("with (self) {\n") + newcode + "\n/* */}",scripts[i]->pev_global, script_names);
      scripts[i]->pev_global->myObj = NULL;
    }
    fflush(stdout);
  }

  //Next we just parse the timeline scripts to add semicolons and collect variable names
  tline_lookup.clear();
  for (int i=0; i<es->timelineCount; i++) 
  {
    for (int j=0; j<es->timelines[i].momentCount; j++) 
    {
      std::string newcode;
      int a = syncheck::syntaxcheck(es->timelines[i].moments[j].code, newcode);
      if (a != -1) {
        user << "Syntax error in timeline `" << es->timelines[i].name <<", moment: " <<es->timelines[i].moments[j].stepNo << "'\n" << format_error(es->timelines[i].moments[j].code,syncheck::syerr,a) << flushl;
        return E_ERROR_SYNTAX;
      }

      //Add a parsed_script record. We can retrieve this later; its order is well-defined (timeline i, moment j) and can be calculated with a global counter.
      tlines.push_back(new parsed_script());

      // Keep a parsed record of this timeline
      tline_lookup[es->timelines[i].name].push_back(tlines.back());
      parser_main(newcode, &tlines.back()->pev, script_names);
      edbg << "Parsed `" << es->timelines[i].name <<", moment: " <<es->timelines[i].moments[j].stepNo << "': " << (int)(tlines.back()->obj.locals.size()) << " locals, " << (int)tlines.back()->obj.globals.size() << " globals" << flushl;

      // If the timeline accesses variables from outside its scope implicitly
      if (tlines.back()->obj.locals.size() or tlines.back()->obj.globallocals.size() or tlines.back()->obj.ambiguous.size()) {
        parsed_object temporary_object = *tlines.back()->pev.myObj;
        tlines.back()->pev_global = new parsed_event(&temporary_object);
        parser_main(string("with (self) {\n") + newcode + "\n/* */}",tlines.back()->pev_global, script_names);
        tlines.back()->pev_global->myObj = NULL;
      }
      fflush(stdout);
    }
  }

  edbg << "\"Linking\" scripts" << flushl;

  //Next we traverse the scripts for dependencies.
  //The problem is, script0 may call script1, etc., which is complicated by timelines (which may also call scripts).
  //For now, we simply perform log2(|scripts|+|timelines|) passes, which ensures that all mult-level scripts bubble to the top.
  const unsigned nec_iters = es->scriptCount+es->timelineCount > 0 ? lrint(ceilf(log2(es->scriptCount+es->timelineCount))) : 0;

  edbg << "`Linking' " << es->scriptCount << " scripts and " <<es->timelineCount <<" timelines in " << nec_iters << " passes...\n";
  for (unsigned _necit = 0; _necit < nec_iters; _necit++) //We will iterate the list of scripts just enough times to copy everything
  {
    for (int _im = 0; _im < es->scriptCount; _im++) //For each script
    {
      parsed_script* curscript = scripts[_im]; //At this point, what we have is this:     for each script as curscript
      for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) //For each function called by each script
      {
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end()) { //At this point, what we have is this:     for each script called by curscript
          curscript->obj.copy_calls_from(subscr->second->obj);
          curscript->obj.copy_tlines_from(subscr->second->obj);
        }
      }

      for (parsed_object::tlineit it = curscript->obj.tlines.begin(); it != curscript->obj.tlines.end(); it++) //For each tline called by each script
      {
        map<string, vector<parsed_script*> >::iterator timit = tline_lookup.find(it->first); //Check if it's a timeline.
        if (timit != tline_lookup.end()) { //If we've got ourselves a timeline
          for (vector<parsed_script*>::iterator momit = timit->second.begin(); momit!=timit->second.end(); momit++) {
            curscript->obj.copy_calls_from((*momit)->obj);
            curscript->obj.copy_tlines_from((*momit)->obj);
          }
        }
      }
    }

    int lookup_id = 0;
    for (int _im = 0; _im < es->timelineCount; _im++) //For each timeline-script
    {
      for (int j=0; j<es->timelines[_im].momentCount; j++,lookup_id++)
      {
        parsed_script* curscript = tlines[lookup_id]; //At this point, what we have is this:     for each script as curscript
        for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) //For each function called by each timeline
        {
          map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
          if (subscr != scr_lookup.end()) { //At this point, what we have is this:     for each script called by curscript
            curscript->obj.copy_calls_from(subscr->second->obj);
          }
        }

        for (parsed_object::tlineit it = curscript->obj.tlines.begin(); it != curscript->obj.tlines.end(); it++) //For each tline called by each tline
        {
          map<string, vector<parsed_script*> >::iterator timit = tline_lookup.find(it->first); //Check if it's a timeline.
          if (timit != tline_lookup.end()) { //If we've got ourselves a timeline
            for (vector<parsed_script*>::iterator momit = timit->second.begin(); momit!=timit->second.end(); momit++) {
              curscript->obj.copy_calls_from((*momit)->obj);
              curscript->obj.copy_tlines_from((*momit)->obj);
            }
          }
        }
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


  edbg << "Completing timeline \"Link\"" << flushl;

  //TODO: Linking timelines might not be strictly necessary, because scripts can now find their children through the timelines they call.
  int lookup_id = 0;
  for (int _im = 0; _im < es->timelineCount; _im++) //For each script
  {
    for (int j=0; j<es->timelines[_im].momentCount; j++,lookup_id++)
    {
      string curscrname = es->timelines[_im].name;
      parsed_script* curscript = tlines[lookup_id]; //At this point, what we have is this:     for each script as curscript
      edbg << "Linking `" << curscrname << " moment: " <<es->timelines[_im].moments[j].stepNo << "':\n";
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
    }
    cout << endl;
  }
  edbg << "Done." << flushl;

  //Done with scripts and timelines.



  edbg << es->gmObjectCount << " Objects:\n";
  for (int i = 0; i < es->gmObjectCount; i++)
  {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    parsed_object* pob = parsed_objects[es->gmObjects[i].id] =
      new parsed_object(
        es->gmObjects[i].name, es->gmObjects[i].id, es->gmObjects[i].spriteId, es->gmObjects[i].maskId,
        es->gmObjects[i].parentId,
        es->gmObjects[i].visible, es->gmObjects[i].solid,
        es->gmObjects[i].depth, es->gmObjects[i].persistent
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
        string newcode = es->gmObjects[i].mainEvents[ii].events[iii].code;

        //Syntax check the code

        // Print debug info
          edbg << "Check `" << es->gmObjects[i].name << "::" << event_get_function_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id) << "...";

        // Check the code
        int sc = syncheck::syntaxcheck(es->gmObjects[i].mainEvents[ii].events[iii].code, newcode);
        if (sc != -1)
        {
          // Error. Report it.
          user << "Syntax error in object `" << es->gmObjects[i].name << "', " << event_get_human_name(es->gmObjects[i].mainEvents[ii].id,es->gmObjects[i].mainEvents[ii].events[iii].id) << " event:"
               << es->gmObjects[i].mainEvents[ii].events[iii].id << ":\n" << format_error(es->gmObjects[i].mainEvents[ii].events[iii].code,syncheck::syerr,sc) << flushl;
          return E_ERROR_SYNTAX;
        }

        edbg << " Done. Parse...";

        //Add this to our objects map
        pev.myObj = pob; //Link to its calling object.
        parser_main(newcode,&pev,script_names, setting::compliance_mode!=setting::COMPL_STANDARD); //Format it to C++

        edbg << " Done." << flushl;
      }
    }
  }

  //Now we parse the rooms
  edbg << "Creating room creation code scope and parsing" << flushl;
  for (int i = 0; i < es->roomCount; i++)
  {
    parsed_room *pr = parsed_rooms[es->rooms[i].id] = new parsed_room;
    parsed_event &pev = pr->events[0]; //Make sure each sub event knows its main event's event ID.
    pev.mainId = 0, pev.id = 0, pev.myObj = pr;

    std::string newcode;
    int sc = syncheck::syntaxcheck(es->rooms[i].creationCode, newcode);
    if (sc != -1) {
      user << "Syntax error in room creation code for room " << es->rooms[i].id << " (`" << es->rooms[i].name << "'):\n" << format_error(es->rooms[i].creationCode,syncheck::syerr,sc) << flushl;
      return E_ERROR_SYNTAX;
    }
    parser_main(newcode,&pev,script_names);

    for (int ii = 0; ii < es->rooms[i].instanceCount; ii++)
    {
      if (es->rooms[i].instances[ii].creationCode and *(es->rooms[i].instances[ii].creationCode))
      {
        newcode = "";
        int a = syncheck::syntaxcheck(es->rooms[i].instances[ii].creationCode, newcode);
        if (a != -1) {
          user << "Syntax error in instance creation code for instance " << es->rooms[i].instances[ii].id <<" in room " << es->rooms[i].id << " (`" << es->rooms[i].name << "'):\n" << format_error(es->rooms[i].instances[ii].creationCode,syncheck::syerr,a) << flushl;
          return E_ERROR_SYNTAX;
        }

        pr->instance_create_codes[es->rooms[i].instances[ii].id].object_index = es->rooms[i].instances[ii].objectId;
        parsed_event* icce = pr->instance_create_codes[es->rooms[i].instances[ii].id].pe = new parsed_event(-1,-1,parsed_objects[es->rooms[i].instances[ii].objectId]);
        parser_main(string("with (") + to_string(es->rooms[i].instances[ii].id) + ") {" + newcode + "\n/* */}", icce, script_names);
      }
    }

    //PreCreate code
    for (int ii = 0; ii < es->rooms[i].instanceCount; ii++)
    {
      if (es->rooms[i].instances[ii].preCreationCode and *(es->rooms[i].instances[ii].preCreationCode))
      {
        std::string newcode;
        int a = syncheck::syntaxcheck(es->rooms[i].instances[ii].preCreationCode, newcode);
        if (a != -1) {
          cout << "Syntax error in instance preCreation code for instance " << es->rooms[i].instances[ii].id <<" in room " << es->rooms[i].id << " (`" << es->rooms[i].name << "'):" << endl << syncheck::syerr << flushl;
          return E_ERROR_SYNTAX;
        }

        pr->instance_precreate_codes[es->rooms[i].instances[ii].id].object_index = es->rooms[i].instances[ii].objectId;
        parsed_event* icce = pr->instance_precreate_codes[es->rooms[i].instances[ii].id].pe = new parsed_event(-1,-1,parsed_objects[es->rooms[i].instances[ii].objectId]);
        parser_main(string("with (") + tostring(es->rooms[i].instances[ii].id) + ") {" + newcode + "\n/* */}", icce, script_names);
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
      if (subscr != scr_lookup.end()) { //If we've got ourselves a script
        t->copy_calls_from(subscr->second->obj);
        t->copy_tlines_from(subscr->second->obj);
      }
    }
    for (parsed_object::funcit it = t->funcs.begin(); it != t->funcs.end(); it++) //For each function called by each script
    {
      map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
      if (subscr != scr_lookup.end()) { //If we've got ourselves a script

        t->copy_from(subscr->second->obj,  "script `"+it->first+"'",  "object `"+i->second->name+"'");
		}
    }
  }

  //Next we link the timelines into the objects that might call them.
  edbg << "\"Linking\" timelines into the objects..." << flushl;
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
  {
    parsed_object* t = i->second;
    for (parsed_object::tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) //For each function called by each timeline
    {
      map<string, vector<parsed_script*> >::iterator timit = tline_lookup.find(it->first); //Check if it's a timeline.
      if (timit != tline_lookup.end()) { //If we've got ourselves a timeline
        //Iterate through its moments:
        for (vector<parsed_script*>::iterator momit = timit->second.begin(); momit!=timit->second.end(); momit++) {
          t->copy_calls_from((*momit)->obj);
          t->copy_tlines_from((*momit)->obj);
        }
      }
    }
    for (parsed_object::tlineit it = t->tlines.begin(); it != t->tlines.end(); it++) //For each function called by each timeline
    {
      map<string, vector<parsed_script*> >::iterator timit = tline_lookup.find(it->first); //Check if it's a timeline.
      if (timit != tline_lookup.end()) { //If we've got ourselves a timeline
        //Iterate through its moments:
        for (vector<parsed_script*>::iterator momit = timit->second.begin(); momit!=timit->second.end(); momit++) {
          t->copy_from((*momit)->obj,  "script `"+it->first+"'",  "object `"+i->second->name+"'");
        }
      }
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


int lang_CPP::link_globals(parsed_object *global, EnigmaStruct *es,parsed_script *scripts[], vector<parsed_script*>& tlines)
{
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
    global->copy_from(*i->second,"object `"+i->second->name+"'","the Global Scope");
  //TODO: because parsed_room inherits parsed_object it tries to use that as the name but it looks
  //like it never gets initialized, this is obviously a bug because this output never tells us the room name always just `'
  for (pr_i i = parsed_rooms.begin(); i != parsed_rooms.end(); i++)
    global->copy_from(*i->second,"room `"+i->second->name+"'","the Global Scope");
  for (int i = 0; i < es->scriptCount; i++)
    global->copy_from(scripts[i]->obj,"script `"+scripts[i]->obj.name+"'","the Global Scope");
  for (int i = 0; i < int(tlines.size()); i++)
    global->copy_from(tlines[i]->obj,"script `"+tlines[i]->obj.name+"'","the Global Scope");
  return 0;
}

//Converts ambiguous types to locals if the globalvar does not exist
int lang_CPP::link_ambiguous(parsed_object *global, EnigmaStruct *es,parsed_script *scripts[], vector<parsed_script*>& tlines)
{
  for (po_i i = parsed_objects.begin(); i != parsed_objects.end(); i++)
  {
    parsed_object* t = i->second;
    for (parsed_object::ambit it = t->ambiguous.begin(); it != t->ambiguous.end(); it++)
    {
      parsed_object::globit g = global->globals.find(it->first);
      if (g == global->globals.end())
        t->locals[it->first] = it->second, cout << "Determined `" << it->first << "' to be local for object `" << t->name << "'" << endl;
      else
        cout << "Determined `" << it->first << "' to be global for object `" << t->name << "'" << endl;
    }
  }
  for (pr_i i = parsed_rooms.begin(); i != parsed_rooms.end(); i++)
  {
    parsed_object* t = i->second;
    for (parsed_object::ambit it = t->ambiguous.begin(); it != t->ambiguous.end(); it++)
    {
      parsed_object::globit g = global->globals.find(it->first);
      if (g == global->globals.end())
        t->locals[it->first] = it->second, cout << "Determined `" << it->first << "' to be local for object `" << t->name << "'" << endl;
      else
        cout << "Determined `" << it->first << "' to be global for object `" << t->name << "'" << endl;
    }
  }
  for (int i = 0; i < es->scriptCount; i++)
  {
    parsed_object &t = scripts[i]->obj;
    for (parsed_object::ambit it = t.ambiguous.begin(); it != t.ambiguous.end(); it++)
    {
      parsed_object::globit g = global->globals.find(it->first);
      if (g == global->globals.end())
        t.locals[it->first] = it->second, cout << "Determined `" << it->first << "' to be local for script `" << es->scripts[i].name << "'" << endl;
      else
        cout << "Determined `" << it->first << "' to be global for script `" << es->scripts[i].name << "'" << endl;
    }
  }


  int tlineID = 0;
  for (int i=0; i<es->timelineCount; i++)
  {
    for (int j=0; j<es->timelines[i].momentCount; j++)
    {
      parsed_object &t = tlines[tlineID++]->obj;
      for (parsed_object::ambit it = t.ambiguous.begin(); it != t.ambiguous.end(); it++)
      {
        parsed_object::globit g = global->globals.find(it->first);
        if (g == global->globals.end())
          t.locals[it->first] = it->second, cout << "Determined `" << it->first << "' to be local for timeline `" << es->timelines[i].name << "', moment `" <<j <<"'" << endl;
        else
          cout << "Determined `" << it->first << "' to be global for timeline `" << es->timelines[i].name << "', moment `" <<j <<"'" << endl;
      }
    }
  }

  return 0;
}
