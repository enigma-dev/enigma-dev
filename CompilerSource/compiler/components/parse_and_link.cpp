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

#include "backend/GameData.h"
#include "parser/object_storage.h"
#include "compiler/compile_common.h"
#include "event_reader/event_parser.h"

#include <math.h> //log2 to calculate passes.

#include <languages/lang_CPP.h>

#include "compiler/compile_includes.h"
#include "settings.h"

//extern string tostring(int);

int lang_CPP::compile_parseAndLink(const GameData &game, CompileState &state) {
  auto &scripts = state.parsed_scripts;
  auto &tlines = state.parsed_tlines;
  auto &scr_lookup = state.script_lookup;
  std::map<string, std::vector<parsed_script*> > tline_lookup;
  
  set<string> script_names;
  for (const auto &script : game.scripts)
    script_names.insert(script.name);
  
  // First we just parse the scripts to add semicolons and collect variable names
  scripts.resize(game.scripts.size());
  for (size_t i = 0; i < game.scripts.size(); i++) {
    std::string newcode;
    int a = syncheck::syntaxcheck(game.scripts[i].code(), newcode);
    if (a != -1) {
      user << "Syntax error in script `" << game.scripts[i].name << "'\n"
           << format_error(game.scripts[i].code(), syncheck::syerr, a) << flushl;
      return E_ERROR_SYNTAX;
    }
    // Keep a parsed record of this script
    scr_lookup[game.scripts[i].name] = scripts[i] = new parsed_script;
    parser_main(newcode, &scripts[i]->pev, script_names);
    edbg << "Parsed `" << game.scripts[i].name << "': " << scripts[i]->obj.locals.size() << " locals, " << scripts[i]->obj.globals.size() << " globals" << flushl;

    // If the script accesses variables from outside its scope implicitly
    if (scripts[i]->obj.locals.size() or scripts[i]->obj.globallocals.size() or scripts[i]->obj.ambiguous.size()) {
      parsed_object temporary_object = *scripts[i]->pev.myObj;
      scripts[i]->pev_global = new parsed_event(&temporary_object);
      parser_main(string("with (self) {\n") + newcode + "\n/* */}",scripts[i]->pev_global, script_names);
      scripts[i]->pev_global->myObj = NULL;
    }
    fflush(stdout);
  }

  // Next we just parse the timeline scripts to add semicolons and collect variable names
  for (const auto &timeline : game.timelines)
  {
    for (const auto &moment : timeline.moments())
    {
      std::string newcode;
      int a = syncheck::syntaxcheck(moment.code(), newcode);
      if (a != -1) {
        user << "Syntax error in timeline `" << timeline.name
             << ", moment: " << moment.step() << "'\n"
             << format_error(moment.code(), syncheck::syerr, a) << flushl;
        return E_ERROR_SYNTAX;
      }

      //Add a parsed_script record. We can retrieve this later; its order is well-defined (timeline i, moment j) and can be calculated with a global counter.
      tlines.push_back(new parsed_script());

      // Keep a parsed record of this timeline
      tline_lookup[timeline.name].push_back(tlines.back());
      parser_main(newcode, &tlines.back()->pev, script_names);
      edbg << "Parsed `" << timeline.name << ", moment: "
           << moment.step() << "': "
           << tlines.back()->obj.locals.size() << " locals, "
           << tlines.back()->obj.globals.size() << " globals" << flushl;

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
  const unsigned nec_iters = game.scripts.size()+game.timelines.size() > 0 ? lrint(ceilf(log2(game.scripts.size()+game.timelines.size()))) : 0;

  edbg << "`Linking' " << game.scripts.size() << " scripts and " <<game.timelines.size() <<" timelines in " << nec_iters << " passes...\n";
  for (unsigned _necit = 0; _necit < nec_iters; _necit++) //We will iterate the list of scripts just enough times to copy everything
  {
    for (size_t _im = 0; _im < game.scripts.size(); _im++) {  // For each script
      parsed_script* curscript = scripts[_im];  // At this point, what we have is this:     for each script as curscript
      for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) //For each function called by each script
      {
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end()) { //At this point, what we have is this:     for each script called by curscript
          curscript->obj.copy_calls_from(subscr->second->obj);
          curscript->obj.copy_tlines_from(subscr->second->obj);
        }
      }

      for (parsed_object::tlineit it = curscript->obj.tlines.begin(); it != curscript->obj.tlines.end(); it++) { //For each tline called by each script
        map<string, vector<parsed_script*> >::iterator timit = tline_lookup.find(it->first); //Check if it's a timeline.
        if (timit != tline_lookup.end()) { //If we've got ourselves a timeline
          for (vector<parsed_script*>::iterator momit = timit->second.begin(); momit!=timit->second.end(); momit++) {
            curscript->obj.copy_calls_from((*momit)->obj);
            curscript->obj.copy_tlines_from((*momit)->obj);
          }
        }
      }
    }

    for (parsed_script* curscript : tlines) {
      for (parsed_object::funcit it = curscript->obj.funcs.begin(); it != curscript->obj.funcs.end(); it++) {  //For each function called by each timeline
        map<string,parsed_script*>::iterator subscr = scr_lookup.find(it->first); //Check if it's a script
        if (subscr != scr_lookup.end()) { //At this point, what we have is this:     for each script called by curscript
          curscript->obj.copy_calls_from(subscr->second->obj);
        }
      }

      for (parsed_object::tlineit it = curscript->obj.tlines.begin(); it != curscript->obj.tlines.end(); it++) {  // For each tline called by each tline
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

  edbg << "Completing script \"Link\"" << flushl;

  for (size_t script_ind = 0; script_ind < game.scripts.size(); ++script_ind) {
    parsed_script* curscript = scripts[script_ind];
    string curscrname = game.scripts[script_ind].name;
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
  for (const auto &timeline : game.timelines) {
    for (const auto &moment : timeline.moments()) {
      string curscrname = timeline.name;
      parsed_script* curscript = tlines[lookup_id++]; //At this point, what we have is this:     for each script as curscript
      edbg << "Linking `" << curscrname << " moment: " <<moment.step() << "':\n";
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



  edbg << game.objects.size() << " Objects:\n";
  for (const auto &object : game.objects) {
    //For every object in Ism's struct, make our own
    unsigned ev_count = 0;
    state.parsed_objects.push_back(
      new parsed_object(
        object.name, object.id(), object.sprite_name(), object.mask_name(),
        object.parent_name(),
        object.visible(), object.solid(),
        object.depth(), object.persistent()
      ));
    parsed_object* pob = state.parsed_objects.back();

    edbg << " " << object.name << ": " << object.events().size() << " events: " << flushl;

    for (const auto& event : object.events()) {
      int ev_id;
        if (event.has_name()) {
          edbg << "  Event[" << event.type() << ", " << event.name() << "] ";
          user << "FATAL: Unsupported: Event names. Translation required using "
                  "events.res. Translation is present but there is no logic to "
                  "use it.";

          // TODO: Using the event type from events.res, translate the name
          // string to a number and a valid identifier for use in this routine.
          // It may be wise to store this information in a normalized table
          // within GameData, similar to how fonts store glyph metrics.
          // Also, remember to update calls to event_get_human_name.

          // ev_id = translate_event_name(event.name());
          return E_ERROR_COMPILER_LOGIC;
        } else {
          edbg << "  Event[" << event.type() << ", " << event.number() << "] ";
          ev_id = event.number();
        }

        //For each individual event (like begin_step) in the main event (Step), parse the code
        parsed_event &pev = pob->events[ev_count++]; //Make sure each sub event knows its main event's event ID.
        pev.mainId = event.type(), pev.id = event.number();

        //Copy the code into a string, and its attributes elsewhere
        string newcode = event.code();

        //Syntax check the code

        // Print debug info
        edbg << "Check `" << object.name << "::" << event_get_function_name(event.type(), ev_id) << "...";

        // Check the code
        int sc = syncheck::syntaxcheck(event.code(), newcode);
        if (sc != -1) {
          // Error. Report it.
          user << "Syntax error in object `" << object.name << "', " << event_get_human_name(event.type(), ev_id) << " event:"
               << ev_id << ":\n" << format_error(event.code(), syncheck::syerr, sc) << flushl;
          return E_ERROR_SYNTAX;
        }

        edbg << " Done. Parse...";

        //Add this to our objects map
        pev.myObj = pob; //Link to its calling object.
        parser_main(newcode,&pev,script_names, setting::compliance_mode!=setting::COMPL_STANDARD); //Format it to C++

        edbg << " Done." << flushl;
    }
  }
  
  // Index parsed objects by name for lookup from instance object_types.
  map<string, parsed_object*> parsed_objects;
  for (parsed_object *obj : state.parsed_objects)
    parsed_objects[obj->name] = obj;

  //Now we parse the rooms
  edbg << "Creating room creation code scope and parsing" << flushl;
  for (const auto &room : game.rooms) {
    parsed_room *pr;
    state.parsed_rooms.push_back(pr = new parsed_room);
    parsed_event &pev = pr->events[0]; //Make sure each sub event knows its main event's event ID.
    pev.mainId = 0, pev.id = 0, pev.myObj = pr;

    std::string newcode;
    int sc = syncheck::syntaxcheck(room.creation_code(), newcode);
    if (sc != -1) {
      user << "Syntax error in room creation code for room " << room.id()
           << " (`" << room.name << "'):\n"
           << format_error(room.creation_code(),syncheck::syerr,sc) << flushl;
      return E_ERROR_SYNTAX;
    }
    parser_main(newcode,&pev,script_names);

    for (const auto &instance : room.instances()) {
      if (!instance.creation_code().empty()) {
        newcode = "";
        int a = syncheck::syntaxcheck(instance.creation_code(), newcode);
        if (a != -1) {
          user << "Syntax error in instance creation code for instance "
               << instance.id() << " in room " << room.id() << " (`" << room.name << "'):\n"
               << format_error(instance.creation_code(), syncheck::syerr, a) << flushl;
          return E_ERROR_SYNTAX;
        }

        pr->instance_create_codes[instance.id()].object_name = instance.object_type();
        parsed_event* icce = pr->instance_create_codes[instance.id()].pe = new parsed_event(-1,-1,parsed_objects[instance.object_type()]);
        parser_main(string("with (") + to_string(instance.id()) + ") {" + newcode + "\n/* */}", icce, script_names);
      }
    }

    //PreCreate code
    for (const auto &instance : room.instances()) {
      if (!instance.initialization_code().empty()) {
        std::string newcode;
        int a = syncheck::syntaxcheck(instance.initialization_code(), newcode);
        if (a != -1) {
          cout << "Syntax error in instance initialization code for instance "
               << instance.id() <<" in room " << room.id() << " (`" << room.name
               << "'):\n" << format_error(instance.initialization_code(), syncheck::syerr, a)
               << flushl;
          return E_ERROR_SYNTAX;
        }

        pr->instance_precreate_codes[instance.id()].object_name = instance.object_type();
        parsed_event* icce = pr->instance_precreate_codes[instance.id()].pe =
            new parsed_event(-1,-1,parsed_objects[instance.object_type()]);
        parser_main(string("with (") + tostring(instance.id()) + ") {" + newcode + "\n/* */}", icce, script_names);
      }
    }
  }



  //Next we link the scripts into the objects.
  edbg << "\"Linking\" scripts into the objects..." << flushl;
  for (auto i = parsed_objects.begin(); i != parsed_objects.end(); i++)
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
  for (auto i = parsed_objects.begin(); i != parsed_objects.end(); i++) {
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
  for (parsed_object *obj : state.parsed_objects) {
    for (auto func_entry : obj->funcs) {  // For each function called by this object
      auto subscr = scr_lookup.find(func_entry.first); //Check if it's a script
      if (subscr != scr_lookup.end() and subscr->second->globargs < func_entry.second) {
        subscr->second->globargs = func_entry.second;
        edbg << "  Object `" << obj->name << "' calls " << func_entry.first
             << " with " << func_entry.second << " parameters." << flushl;
      }
    }
  }
  edbg << "Finished" << flushl;

  return 0;
}


int lang_CPP::link_globals(const GameData &game, CompileState &state) {
  for (parsed_object *obj : state.parsed_objects)
    state.global_object.copy_from(*obj, "object `" + obj->name + "'", "the Global Scope");
  //TODO: because parsed_room inherits parsed_object it tries to use that as the name but it looks
  //like it never gets initialized, this is obviously a bug because this output never tells us the room name always just `'
  for (parsed_room *room : state.parsed_rooms)
    state.global_object.copy_from(*room, "room `" + room->name + "'", "the Global Scope");
  for (parsed_script *script : state.parsed_scripts)
    state.global_object.copy_from(script->obj, "script `" + script->obj.name + "'", "the Global Scope");
  for (parsed_script *tline : state.parsed_tlines)
    state.global_object.copy_from(tline->obj, "timeline `" + tline->obj.name + "'", "the Global Scope");
  return 0;
}

static inline void link_ambigous(parsed_object* t, parsed_object *global, string desc) {
  for (parsed_object::ambit it = t->ambiguous.begin(); it != t->ambiguous.end(); it++) {
    parsed_object::globit g = global->globals.find(it->first);
    if (g == global->globals.end())
      t->locals[it->first] = it->second, cout << "Determined `" << it->first << "' to be local for " << desc << endl;
    else
      cout << "Determined `" << it->first << "' to be global for " << desc << "'" << endl;
  }
}

//Converts ambiguous types to locals if the globalvar does not exist
int lang_CPP::link_ambiguous(const GameData &game, CompileState &state)
{
  for (parsed_object *obj : state.parsed_objects) {
    link_ambigous(obj, &state.global_object, "object " + obj->name);
  }
  for (parsed_room *room : state.parsed_rooms) {
    link_ambigous(room, &state.global_object, "room " + room->name);
  }
  for (const auto &script_entry : state.script_lookup) {
    link_ambigous(&script_entry.second->obj, &state.global_object,
                  "script " + script_entry.first);
  }
  // XXX: Didn't sorlok already merge these? either the code in link_globals
  // is wrong, or this block is overly complicated (iterating parsed_timelines
  // and linking their immediate objects should work).
  for (const auto &tl_entry : state.timeline_lookup) {
    const auto &timeline = tl_entry.second;
    for (const auto &moment : timeline.moments) {
      link_ambigous(&moment.script->obj, &state.global_object,
          "timeline " + tl_entry.first + ", moment " + to_string(moment.step));
    }
  }

  return 0;
}
